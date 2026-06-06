#include "threadpool.h"

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
    return (ap_size_t)_InterlockedCompareExchange64(
        (volatile long long*)p,
        0,
        0
    );
}

static inline void
apac_atomic_store(ap_size_t* p, ap_size_t val)
{
    _InterlockedExchange64(
        (volatile long long*)p,
        (long long)val
    );
}

static inline ap_size_t
apac_atomic_fetch_add(ap_size_t* p, ap_size_t val)
{
    return (ap_size_t)_InterlockedExchangeAdd64(
        (volatile long long*)p,
        (long long)val
    );
}

static inline ap_size_t
apac_atomic_fetch_sub(ap_size_t* p, ap_size_t val)
{
    return (ap_size_t)_InterlockedExchangeAdd64(
        (volatile long long*)p,
        -(long long)val
    );
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
        __ATOMIC_SEQ_CST
    );
}

static inline void
apac_atomic_store(ap_size_t* p, ap_size_t val)
{
    __atomic_store_n(
        p,
        val,
        __ATOMIC_SEQ_CST
    );
}

static inline ap_size_t
apac_atomic_fetch_add(ap_size_t* p, ap_size_t val)
{
    return __atomic_fetch_add(
        p,
        val,
        __ATOMIC_SEQ_CST
    );
}

static inline ap_size_t
apac_atomic_fetch_sub(ap_size_t* p, ap_size_t val)
{
    return __atomic_fetch_sub(
        p,
        val,
        __ATOMIC_SEQ_CST
    );
}

#else
    #error "Unsupported Platform!"
#endif

static apac_thrd_ret_t APAC_THRD_CALL
apac_tpool_worker(apac_thrd_arg_t arg)
{
    ap_size_t tid = (ap_size_t)(uintptr_t)arg;   // ← extract index; was (void)empty

    while (true)
    {
        apac_mutex_lock(&tpool.lock);

        while (!tpool.has_work && !tpool.shutdown)
        {
            apac_cond_wait(&tpool.work_ready, &tpool.lock);
        }
        
        if (tpool.shutdown)
        {
            apac_mutex_unlock(&tpool.lock);
            break;
        }

        apac_mutex_unlock(&tpool.lock);

        if (tpool.sched == APAC_SCHED_DYNAMIC)
        {
            while (true)
            {
                ap_size_t start = apac_atomic_fetch_add(&tpool.next, tpool.chunk_size);

                if (start >= tpool.end)
                    break;

                ap_size_t stop = start + tpool.chunk_size;
                if (stop > tpool.end) stop = tpool.end;

                tpool.func(start, stop, tpool.arg);
            }
        }
        else  // APAC_SCHED_STATIC
        {
            ap_size_t total = tpool.end - tpool.begin;
            ap_size_t n     = tpool.max_thrds;
            ap_size_t base  = total / n;
            ap_size_t rem   = total % n;

            ap_size_t my_start = tpool.begin + tid * base;
            ap_size_t my_count = (tid == n - 1) ? base + rem : base;

            if (my_count > 0)
            {
                tpool.func(my_start, my_start + my_count, tpool.arg);
            }
        }

        apac_mutex_lock(&tpool.lock);

        if (apac_atomic_fetch_sub(&tpool.active_thrds, 1) == 1)
        {
            tpool.has_work = false;
            apac_cond_signal(&tpool.work_done);
        }

        apac_mutex_unlock(&tpool.lock);
    }

    return 0;
}

apac_err
apac_tpool_init(ap_size_t thrd_count)
{
    APAC_ASSERT(thrd_count != 0);
    APAC_ASSERT(thrd_count <= SIZE_MAX / sizeof(apac_thrd_t));

    ap_size_t i = 0;

    if (is_tpool_init)
    {
        return APAC_OK;
    }

    apac_err result = APAC_OK;

    tpool.max_thrds     = thrd_count;
    tpool.active_thrds  = 0;
    tpool.next          = 0;

    tpool.has_work      = false;
    tpool.shutdown      = false;

    tpool.workers       = NULL;

    apac_mutex_init(&tpool.lock);
    apac_cond_init(&tpool.work_ready);
    apac_cond_init(&tpool.work_done);

    tpool.workers = apac_malloc(sizeof(apac_thrd_t) * thrd_count);

    if (!tpool.workers)
    {
        result = APAC_OOM;
        goto fail_cleanup;
    }

    for (i = 0; i < thrd_count; i++)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        tpool.workers[i] = CreateThread(
            NULL, 
            0,
            apac_tpool_worker,
            (LPVOID)(uintptr_t)i,
            0, 
            NULL
        );

        if (!tpool.workers[i])
        {
            result = APAC_THRD_CREATE_FAIL;
            goto fail_cleanup;
        }

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        int ret_val = pthread_create(
            &tpool.workers[i], NULL,
            apac_tpool_worker,
            (void*)(uintptr_t)i
        );

        if (ret_val != 0)
        {
            result = APAC_THRD_CREATE_FAIL;
            goto fail_cleanup;
        }
#endif
    }

    is_tpool_init = true;

    return APAC_OK;

fail_cleanup:

    apac_mutex_lock(&tpool.lock);

    tpool.shutdown = true;

    apac_cond_broadcast(&tpool.work_ready);

    apac_mutex_unlock(&tpool.lock);

    while (i-- > 0)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        WaitForSingleObject(tpool.workers[i], INFINITE);
        CloseHandle(tpool.workers[i]);

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        pthread_join(tpool.workers[i], NULL);

#endif
    }

    if (tpool.workers)
    {
        apac_free(tpool.workers);
    }

    apac_cond_destroy(&tpool.work_done);
    apac_cond_destroy(&tpool.work_ready);
    apac_mutex_destroy(&tpool.lock);

    return result;
}

// NOT thread-safe with respect to concurrent callers.
// Only one thread may call apac_pllfor_loop at a time.
apac_err
apac_pllfor_loop(
    ap_size_t begin,
    ap_size_t end,
    ap_size_t chunk_size,
    apac_sched_t sched,
    apac_pfor_func_t func,
    void* arg
)
{
    APAC_ASSERT(is_tpool_init);
    APAC_ASSERT(func != NULL);
    APAC_ASSERT(begin <= end);

    apac_mutex_lock(&tpool.lock);

    tpool.func       = func;
    tpool.arg        = arg;

    tpool.begin      = begin;
    tpool.end        = end;

    tpool.sched      = sched;

    tpool.chunk_size = chunk_size;
    tpool.next       = begin;

    apac_atomic_store(&tpool.active_thrds, tpool.max_thrds);

    tpool.has_work = true;

    apac_cond_broadcast(&tpool.work_ready);

    while (tpool.has_work)
    {
        apac_cond_wait(&tpool.work_done, &tpool.lock);
    }

    apac_mutex_unlock(&tpool.lock);

    return APAC_OK;
}

apac_err
apac_tpool_destroy(void)
{
    if (!is_tpool_init)
    {
        return APAC_OK;
    }

    apac_mutex_lock(&tpool.lock);

    tpool.shutdown = true;

    apac_cond_broadcast(&tpool.work_ready);

    apac_mutex_unlock(&tpool.lock);

    for (ap_size_t i = 0; i < tpool.max_thrds; i++)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        WaitForSingleObject(tpool.workers[i], INFINITE);
        CloseHandle(tpool.workers[i]);

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        pthread_join(tpool.workers[i], NULL);

#endif
    }

    apac_free(tpool.workers);

    apac_cond_destroy(&tpool.work_done);
    apac_cond_destroy(&tpool.work_ready);

    apac_mutex_destroy(&tpool.lock);

    is_tpool_init = false;

    return APAC_OK;
}

ap_size_t
apac_tpool_get_size(void)
{
    return tpool.max_thrds;
}
