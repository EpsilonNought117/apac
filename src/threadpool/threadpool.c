#include "threadpool.h"

// ============================================================================
// LOW-LEVEL PLATFORM SYNCHRONIZATION WRAPPERS
// ============================================================================

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

// ----------------------------------------------------------------------------
// Mutex (SRWLOCK)
// ----------------------------------------------------------------------------

static inline apac_thrd_err_t
apac_mutex_init(apac_mutex_t* m)
{
    InitializeSRWLock(m);
    return ERROR_SUCCESS;
}

static inline apac_thrd_err_t
apac_mutex_lock(apac_mutex_t* m)
{
    AcquireSRWLockExclusive(m);
    return ERROR_SUCCESS;
}

static inline apac_thrd_err_t
apac_mutex_unlock(apac_mutex_t* m)
{
    ReleaseSRWLockExclusive(m);
    return ERROR_SUCCESS;
}

static inline apac_thrd_err_t
apac_mutex_destroy(apac_mutex_t* m)
{
    (void)m;
    return ERROR_SUCCESS;
}

// ----------------------------------------------------------------------------
// Condition Variable
// ----------------------------------------------------------------------------

static inline apac_thrd_err_t
apac_cond_init(apac_cond_t* c)
{
    InitializeConditionVariable(c);
    return ERROR_SUCCESS;
}

static inline apac_thrd_err_t
apac_cond_signal(apac_cond_t* c)
{
    WakeConditionVariable(c);
    return ERROR_SUCCESS;
}

static inline apac_thrd_err_t
apac_cond_broadcast(apac_cond_t* c)
{
    WakeAllConditionVariable(c);
    return ERROR_SUCCESS;
}

static inline apac_thrd_err_t
apac_cond_destroy(apac_cond_t* c)
{
    (void)c;
    return ERROR_SUCCESS;
}

static inline apac_thrd_err_t
apac_cond_wait(apac_cond_t* c, apac_mutex_t* m)
{
    return SleepConditionVariableSRW(c, m, INFINITE, 0) 
        ? ERROR_SUCCESS : GetLastError();
}

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

// ----------------------------------------------------------------------------
// Mutex
// ----------------------------------------------------------------------------

static inline apac_thrd_err_t
apac_mutex_init(apac_mutex_t* m)
{
    return pthread_mutex_init(m, NULL);
}

static inline apac_thrd_err_t
apac_mutex_lock(apac_mutex_t* m)
{
    return pthread_mutex_lock(m);
}

static inline apac_thrd_err_t
apac_mutex_unlock(apac_mutex_t* m)
{
    return pthread_mutex_unlock(m);
}

static inline apac_thrd_err_t
apac_mutex_destroy(apac_mutex_t* m)
{
    return pthread_mutex_destroy(m);
}

// ----------------------------------------------------------------------------
// Condition Variable
// ----------------------------------------------------------------------------

static inline apac_thrd_err_t
apac_cond_init(apac_cond_t* c)
{
    return pthread_cond_init(c, NULL);
}

static inline apac_thrd_err_t
apac_cond_signal(apac_cond_t* c)
{
    return pthread_cond_signal(c);
}

static inline apac_thrd_err_t
apac_cond_broadcast(apac_cond_t* c)
{
    return pthread_cond_broadcast(c);
}

static inline apac_thrd_err_t
apac_cond_destroy(apac_cond_t* c)
{
    return pthread_cond_destroy(c);
}

static inline apac_thrd_err_t
apac_cond_wait(apac_cond_t* c, apac_mutex_t* m)
{
    return pthread_cond_wait(c, m);
}

#else
    #error "Unsupported Platform!"
#endif

static apac_thrd_ret_t APAC_THRD_CALL 
apac_tpool_worker(apac_thrd_arg_t pool_ptr)
{
    apac_tpool_t* pool = (apac_tpool_t*)pool_ptr;

    while (true)
    {
        if (apac_mutex_lock(&pool->lock) != APAC_OK) { goto thrd_fail; }

        while (pool->queue_curr_size == 0 && !pool->shutdown)
        {
            if (apac_cond_wait(&pool->notify, &pool->lock) != APAC_OK)
            {
                apac_mutex_unlock(&pool->lock); 
                // doesn't matter if the above call suceeds 
                // or fails, stuff has already gone cuckoo
                // at this point
                goto thrd_fail;
            }
        }

        if (pool->shutdown && pool->queue_curr_size == 0)
        {
            if (apac_mutex_unlock(&pool->lock) != APAC_OK)  { goto thrd_fail; }
            else                                            { break; }
        }

        apac_work_t task = pool->task_queue[pool->queue_head]; // assumes task has valid func arg
        pool->queue_head = (pool->queue_head + 1) % pool->queue_max_size;
        pool->queue_curr_size--;

        if (apac_cond_signal(&pool->queue_full) != APAC_OK) { goto thrd_fail; }

        pool->active_thrds++;
        if (apac_mutex_unlock(&pool->lock) != APAC_OK) { goto thrd_fail; }

        task.func(task.arg); // assumes func is a valid func ptr

        if (apac_mutex_lock(&pool->lock) != APAC_OK) { goto thrd_fail; }
        
        pool->active_thrds--;
        
        if (pool->queue_curr_size == 0 && pool->active_thrds == 0)
        {
            if (apac_cond_broadcast(&pool->wait) != APAC_OK) { goto thrd_fail; }
        }

        if (apac_mutex_unlock(&pool->lock) != APAC_OK) { goto thrd_fail; }
    }

    return APAC_OK;

thrd_fail:

    if (apac_mutex_lock(&pool->lock) != APAC_OK) return APAC_THRD_ERR;

    pool->shutdown = true;

    apac_cond_broadcast(&pool->notify);
    apac_cond_broadcast(&pool->wait);
    apac_cond_broadcast(&pool->queue_full);

    if (apac_mutex_unlock(&pool->lock) != APAC_OK) return APAC_THRD_ERR;

    return APAC_THRD_ERR;
}

#define THRD_INIT_CHECK(cond, init, label)  \
        do                                  \
        {                                   \
            init = cond;                    \
            APAC_ASSERT(init);              \
                                            \
            if (!init)                      \
                goto label;                 \
                                            \
        } while (0)         

apac_err
apac_tpool_init(
    apac_tpool_t* pool, 
    size_t thrd_count, 
    size_t work_queue_size
)
{
    APAC_ASSERT(pool != NULL);
    APAC_ASSERT(thrd_count != 0);
    APAC_ASSERT(thrd_count < SIZE_MAX / sizeof(apac_thrd_t));
    APAC_ASSERT(work_queue_size != 0);
    APAC_ASSERT(work_queue_size < SIZE_MAX / sizeof(apac_work_t));

    pool->thrd_count = thrd_count;
    pool->queue_curr_size = 0;
    pool->queue_max_size = work_queue_size;
    pool->queue_head = 0;
    pool->active_thrds = 0;
    pool->shutdown = false;

    // put NULL so initialized garbage 
    // doesn't mess up our cleanup code
    pool->workers = NULL;
    pool->task_queue = NULL;

    bool is_lock_init = false;
    bool is_notify_init = false;
    bool is_wait_init = false;
    bool is_queue_full_init = false;

    THRD_INIT_CHECK(apac_mutex_init(&pool->lock) == 0, is_lock_init, fail_cleanup);
    THRD_INIT_CHECK(apac_cond_init(&pool->notify) == 0, is_notify_init, fail_cleanup);
    THRD_INIT_CHECK(apac_cond_init(&pool->wait) == 0, is_wait_init, fail_cleanup);
    THRD_INIT_CHECK(apac_cond_init(&pool->queue_full) == 0, is_queue_full_init, fail_cleanup);

    THRD_INIT_CHECK(apac_malloc(sizeof(apac_thrd_t) * thrd_count), pool->workers, fail_cleanup);
    THRD_INIT_CHECK(apac_malloc(sizeof(apac_work_t) * pool->queue_max_size), pool->task_queue, fail_cleanup);
    
    size_t i = 0; // keep this function scoped
    // so in case of failure at i, clear all threads
    // from (i - 1)th to 0th

    for (i = 0; i < thrd_count; i++)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        HANDLE handle = CreateThread(NULL, 0, apac_tpool_worker, (void*)pool, 0, NULL);

        if (handle == NULL)
            goto fail_cleanup;

        pool->workers[i] = handle;

#else

        if (pthread_create(&pool->workers[i], NULL, apac_tpool_worker, (void*)pool))
            goto fail_cleanup;

#endif
    }

    return APAC_OK;

fail_cleanup:

    pool->shutdown = true;
    if (is_notify_init) apac_cond_broadcast(&pool->notify);

    while (i-- > 0)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        WaitForSingleObject(pool->workers[i], INFINITE);
        CloseHandle(pool->workers[i]);

#else

        pthread_join(pool->workers[i], NULL);

#endif
    }

    if (pool->task_queue)   apac_free(pool->task_queue);
    if (pool->workers)      apac_free(pool->workers);
    if (is_queue_full_init) apac_cond_destroy(&pool->queue_full);
    if (is_wait_init)       apac_cond_destroy(&pool->wait);
    if (is_notify_init)     apac_cond_destroy(&pool->notify);
    if (is_lock_init)       apac_mutex_destroy(&pool->lock);

    return APAC_THRD_ERR;
}

