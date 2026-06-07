#include "parallel_for.h"

// ============================================================================
// LOW-LEVEL PLATFORM SYNCHRONIZATION WRAPPERS
// ============================================================================

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

// ----------------------------------------------------------------------------
// Mutex (SRWLOCK)
// ----------------------------------------------------------------------------

static inline void
apac_mutex_init(apac_mutex_t* m)
{
    InitializeSRWLock(m);
}

static inline void
apac_mutex_lock(apac_mutex_t* m)
{
    AcquireSRWLockExclusive(m);
}

static inline void
apac_mutex_unlock(apac_mutex_t* m)
{
    ReleaseSRWLockExclusive(m);
}

static inline void
apac_mutex_destroy(apac_mutex_t* m)
{
    (void)m;
}

// ----------------------------------------------------------------------------
// Condition Variable
// ----------------------------------------------------------------------------

static inline void
apac_cond_init(apac_cond_t* c)
{
    InitializeConditionVariable(c);
}

static inline void
apac_cond_signal(apac_cond_t* c)
{
    WakeConditionVariable(c);
}

static inline void
apac_cond_broadcast(apac_cond_t* c)
{
    WakeAllConditionVariable(c);
}

static inline void
apac_cond_destroy(apac_cond_t* c)
{
    (void)c;
}

static inline DWORD
apac_cond_wait(apac_cond_t* c, apac_mutex_t* m)
{
    return SleepConditionVariableSRW(c, m, INFINITE, 0) ? 
           ERROR_SUCCESS : GetLastError();
}

// ----------------------------------------------------------------------------
// Atomic Instructions
// ----------------------------------------------------------------------------

static inline ap_size_t
apac_atomic_load(const ap_size_t* p)
{
#if defined(APAC_ARM64_WIN)
    return (ap_size_t)_InterlockedCompareExchange64_nf(
        (volatile long long*)p,
        0,
        0
    );
#else
    return (ap_size_t)_InterlockedCompareExchange64(
        (volatile long long*)p,
        0,
        0
    );
#endif
}

static inline void
apac_atomic_store(ap_size_t* p, ap_size_t val)
{
#if defined(APAC_ARM64_WIN)
    _InterlockedExchange64_nf(
        (volatile long long*)p,
        (long long)val
    );
#else
    _InterlockedExchange64(
        (volatile long long*)p,
        (long long)val
    );
#endif
}

static inline ap_size_t
apac_atomic_fetch_add(ap_size_t* p, ap_size_t val)
{
#if defined(APAC_ARM64_WIN)
    return (ap_size_t)_InterlockedExchangeAdd64_nf(
        (volatile long long*)p,
        (long long)val
    );
#else
    return (ap_size_t)_InterlockedExchangeAdd64(
        (volatile long long*)p,
        (long long)val
    );
#endif
}

static inline ap_size_t
apac_atomic_fetch_sub(ap_size_t* p, ap_size_t val)
{
#if defined(APAC_ARM64_WIN)
    return (ap_size_t)_InterlockedExchangeAdd64_nf(
        (volatile long long*)p,
        -(long long)val
    );
#else
    return (ap_size_t)_InterlockedExchangeAdd64(
        (volatile long long*)p,
        -(long long)val
    );
#endif
}

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

// ----------------------------------------------------------------------------
// Mutex
// ----------------------------------------------------------------------------

static inline int
apac_mutex_init(apac_mutex_t* m)
{
    return pthread_mutex_init(m, NULL);
}

static inline int
apac_mutex_lock(apac_mutex_t* m)
{
    return pthread_mutex_lock(m);
}

static inline int
apac_mutex_unlock(apac_mutex_t* m)
{
    return pthread_mutex_unlock(m);
}

static inline int
apac_mutex_destroy(apac_mutex_t* m)
{
    return pthread_mutex_destroy(m);
}

// ----------------------------------------------------------------------------
// Condition Variable
// ----------------------------------------------------------------------------

static inline int
apac_cond_init(apac_cond_t* c)
{
    return pthread_cond_init(c, NULL);
}

static inline int
apac_cond_signal(apac_cond_t* c)
{
    return pthread_cond_signal(c);
}

static inline int
apac_cond_broadcast(apac_cond_t* c)
{
    return pthread_cond_broadcast(c);
}

static inline int
apac_cond_destroy(apac_cond_t* c)
{
    return pthread_cond_destroy(c);
}

static inline int
apac_cond_wait(apac_cond_t* c, apac_mutex_t* m)
{
    return pthread_cond_wait(c, m);
}

// ----------------------------------------------------------------------------
// Atomic Instructions
// ----------------------------------------------------------------------------

static inline ap_size_t
apac_atomic_load(const ap_size_t* p)
{
    return __atomic_load_n(
        p,
        __ATOMIC_RELAXED
    );
}

static inline void
apac_atomic_store(ap_size_t* p, ap_size_t val)
{
    __atomic_store_n(
        p,
        val,
        __ATOMIC_RELAXED
    );
}

static inline ap_size_t
apac_atomic_fetch_add(ap_size_t* p, ap_size_t val)
{
    return __atomic_fetch_add(
        p,
        val,
        __ATOMIC_RELAXED
    );
}

static inline ap_size_t
apac_atomic_fetch_sub(ap_size_t* p, ap_size_t val)
{
    return __atomic_fetch_sub(
        p,
        val,
        __ATOMIC_RELAXED
    );
}

#else
    #error "Unsupported Platform!"
#endif

static apac_thrd_ret_t APAC_THRD_CALL
apac_pfor_worker(apac_thrd_arg_t arg)
{
    ap_size_t tid = (ap_size_t)(uintptr_t)arg;   // ← extract index; was (void)empty

    while (true)
    {
        apac_mutex_lock(&pfor.lock);

        while (!pfor.has_work && !pfor.shutdown)
        {
            apac_cond_wait(&pfor.work_ready, &pfor.lock);
        }
        
        if (pfor.shutdown)
        {
            apac_mutex_unlock(&pfor.lock);
            break;
        }

        apac_mutex_unlock(&pfor.lock);

        if (pfor.sched == APAC_SCHED_DYNAMIC)
        {
            while (true)
            {
                ap_size_t start = apac_atomic_fetch_add(&pfor.next, pfor.chunk_size);

                if (start >= pfor.end)
                    break;

                ap_size_t stop = start + pfor.chunk_size;
                if (stop > pfor.end) stop = pfor.end;

                pfor.func(start, stop, pfor.arg);
            }
        }
        else  // APAC_SCHED_STATIC
        {
            ap_size_t total = pfor.end - pfor.begin;
            ap_size_t n     = pfor.max_thrds;
            ap_size_t base  = total / n;
            ap_size_t rem   = total % n;

            ap_size_t my_start = pfor.begin + tid * base;
            ap_size_t my_count = (tid == n - 1) ? base + rem : base;

            if (my_count > 0)
            {
                pfor.func(my_start, my_start + my_count, pfor.arg);
            }
        }

        apac_mutex_lock(&pfor.lock);

        if (apac_atomic_fetch_sub(&pfor.active_thrds, 1) == 1)
        {
            pfor.has_work = false;
            apac_cond_signal(&pfor.work_done);
        }

        apac_mutex_unlock(&pfor.lock);
    }

    return 0;
}

apac_err
apac_pfor_init(ap_size_t thrd_count)
{
    APAC_ASSERT(thrd_count != 0);
    APAC_ASSERT(thrd_count <= SIZE_MAX / sizeof(apac_thrd_t));

    ap_size_t i = 0;

    if (is_pfor_init)
    {
        return APAC_OK;
    }

    apac_err result = APAC_OK;

    pfor.max_thrds     = thrd_count;
    pfor.active_thrds  = 0;
    pfor.next          = 0;

    pfor.has_work      = false;
    pfor.shutdown      = false;

    pfor.workers       = NULL;

    apac_mutex_init(&pfor.lock);
    apac_cond_init(&pfor.work_ready);
    apac_cond_init(&pfor.work_done);

    pfor.workers = apac_malloc(sizeof(apac_thrd_t) * thrd_count);

    if (!pfor.workers)
    {
        result = APAC_OOM;
        goto fail_cleanup;
    }

    for (i = 0; i < thrd_count; i++)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        pfor.workers[i] = CreateThread(
            NULL, 
            0,
            apac_pfor_worker,
            (LPVOID)(uintptr_t)i,
            0, 
            NULL
        );

        if (!pfor.workers[i])
        {
            result = APAC_THRD_CREATE_FAIL;
            goto fail_cleanup;
        }

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        int ret_val = pthread_create(
            &pfor.workers[i], NULL,
            apac_pfor_worker,
            (void*)(uintptr_t)i
        );

        if (ret_val != 0)
        {
            result = APAC_THRD_CREATE_FAIL;
            goto fail_cleanup;
        }
#endif
    }

    is_pfor_init = true;

    return APAC_OK;

fail_cleanup:

    apac_mutex_lock(&pfor.lock);

    pfor.shutdown = true;

    apac_cond_broadcast(&pfor.work_ready);

    apac_mutex_unlock(&pfor.lock);

    while (i-- > 0)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        WaitForSingleObject(pfor.workers[i], INFINITE);
        CloseHandle(pfor.workers[i]);

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        pthread_join(pfor.workers[i], NULL);

#endif
    }

    if (pfor.workers)
    {
        apac_free(pfor.workers);
    }

    apac_cond_destroy(&pfor.work_done);
    apac_cond_destroy(&pfor.work_ready);
    apac_mutex_destroy(&pfor.lock);

    return result;
}

// NOT thread-safe with respect to concurrent callers.
// Only one thread may call apac_pfor_do at a time.
apac_err
apac_pfor_do(
    ap_size_t begin,
    ap_size_t end,
    ap_size_t chunk_size,
    apac_sched_t sched,
    apac_pfor_func_t func,
    void* arg
)
{
    APAC_ASSERT(is_pfor_init);
    APAC_ASSERT(func != NULL);
    APAC_ASSERT(begin <= end);

    apac_mutex_lock(&pfor.lock);

    pfor.func       = func;
    pfor.arg        = arg;

    pfor.begin      = begin;
    pfor.end        = end;

    pfor.sched      = sched;

    pfor.chunk_size = chunk_size;
    pfor.next       = begin;

    apac_atomic_store(&pfor.active_thrds, pfor.max_thrds);

    pfor.has_work = true;

    apac_cond_broadcast(&pfor.work_ready);

    while (pfor.has_work)
    {
        apac_cond_wait(&pfor.work_done, &pfor.lock);
    }

    apac_mutex_unlock(&pfor.lock);

    return APAC_OK;
}

apac_err
apac_pfor_destroy(void)
{
    if (!is_pfor_init)
    {
        return APAC_OK;
    }

    apac_mutex_lock(&pfor.lock);

    pfor.shutdown = true;

    apac_cond_broadcast(&pfor.work_ready);

    apac_mutex_unlock(&pfor.lock);

    for (ap_size_t i = 0; i < pfor.max_thrds; i++)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        WaitForSingleObject(pfor.workers[i], INFINITE);
        CloseHandle(pfor.workers[i]);

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        pthread_join(pfor.workers[i], NULL);

#endif
    }

    apac_free(pfor.workers);

    apac_cond_destroy(&pfor.work_done);
    apac_cond_destroy(&pfor.work_ready);

    apac_mutex_destroy(&pfor.lock);

    is_pfor_init = false;

    return APAC_OK;
}

ap_size_t
apac_pfor_get_size(void)
{
    return pfor.max_thrds;
}
