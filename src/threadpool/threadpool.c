#include "threadpool.h"

// ============================================================================
// LOW-LEVEL PLATFORM SYNCHRONIZATION WRAPPERS
// ============================================================================

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

// ----------------------------------------------------------------------------
// Mutex (SRWLOCK)
// ----------------------------------------------------------------------------

inline apac_thrd_err_t
apac_mutex_init(apac_mutex_t* m)
{
    InitializeSRWLock(m);
    return ERROR_SUCCESS;
}

inline apac_thrd_err_t
apac_mutex_lock(apac_mutex_t* m)
{
    AcquireSRWLockExclusive(m);
    return ERROR_SUCCESS;
}

inline apac_thrd_err_t
apac_mutex_unlock(apac_mutex_t* m)
{
    ReleaseSRWLockExclusive(m);
    return ERROR_SUCCESS;
}

inline apac_thrd_err_t
apac_mutex_destroy(apac_mutex_t* m)
{
    (void)m;
    return ERROR_SUCCESS;
}

// ----------------------------------------------------------------------------
// Condition Variable
// ----------------------------------------------------------------------------

inline apac_thrd_err_t
apac_cond_init(apac_cond_t* c)
{
    InitializeConditionVariable(c);
    return ERROR_SUCCESS;
}

inline apac_thrd_err_t
apac_cond_signal(apac_cond_t* c)
{
    WakeConditionVariable(c);
    return ERROR_SUCCESS;
}

inline apac_thrd_err_t
apac_cond_broadcast(apac_cond_t* c)
{
    WakeAllConditionVariable(c);
    return ERROR_SUCCESS;
}

inline apac_thrd_err_t
apac_cond_destroy(apac_cond_t* c)
{
    (void)c;
    return ERROR_SUCCESS;
}

inline apac_thrd_err_t
apac_cond_wait(apac_cond_t* c, apac_mutex_t* m)
{
    return SleepConditionVariableSRW(c, m, INFINITE, 0) 
        ? ERROR_SUCCESS : GetLastError();
}

// ----------------------------------------------------------------------------
// Barrier
// ----------------------------------------------------------------------------

inline apac_thrd_err_t
apac_barrier_init(apac_barrier_t* b, unsigned count)
{
    return InitializeSynchronizationBarrier(
        b,
        count,
        -1
    ) ? ERROR_SUCCESS : GetLastError();
}

inline apac_thrd_err_t
apac_barrier_wait(apac_barrier_t* b)
{
    return EnterSynchronizationBarrier(
        b,
        SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY
    ) ? ERROR_SUCCESS : GetLastError();
}

inline apac_thrd_err_t
apac_barrier_destroy(apac_barrier_t* b)
{
    DeleteSynchronizationBarrier(b);
    return ERROR_SUCCESS;
}

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

// ----------------------------------------------------------------------------
// Mutex
// ----------------------------------------------------------------------------

inline apac_thrd_err_t
apac_mutex_init(apac_mutex_t* m)
{
    return pthread_mutex_init(m, NULL);
}

inline apac_thrd_err_t
apac_mutex_lock(apac_mutex_t* m)
{
    return pthread_mutex_lock(m);
}

inline apac_thrd_err_t
apac_mutex_unlock(apac_mutex_t* m)
{
    return pthread_mutex_unlock(m);
}

inline apac_thrd_err_t
apac_mutex_destroy(apac_mutex_t* m)
{
    return pthread_mutex_destroy(m);
}

// ----------------------------------------------------------------------------
// Condition Variable
// ----------------------------------------------------------------------------

inline apac_thrd_err_t
apac_cond_init(apac_cond_t* c)
{
    return pthread_cond_init(c, NULL);
}

inline apac_thrd_err_t
apac_cond_signal(apac_cond_t* c)
{
    return pthread_cond_signal(c);
}

inline apac_thrd_err_t
apac_cond_broadcast(apac_cond_t* c)
{
    return pthread_cond_broadcast(c);
}

inline apac_thrd_err_t
apac_cond_destroy(apac_cond_t* c)
{
    return pthread_cond_destroy(c);
}

inline apac_thrd_err_t
apac_cond_wait(apac_cond_t* c, apac_mutex_t* m)
{
    return pthread_cond_wait(c, m);
}

// ----------------------------------------------------------------------------
// Barrier
// ----------------------------------------------------------------------------

inline apac_thrd_err_t
apac_barrier_init(apac_barrier_t* b, unsigned count)
{
    return pthread_barrier_init(b, NULL, count);
}

inline apac_thrd_err_t
apac_barrier_wait(apac_barrier_t* b)
{
    int result = pthread_barrier_wait(b);

    return (result == 0 || result == PTHREAD_BARRIER_SERIAL_THREAD)
        ? 0 : result;
}

inline apac_thrd_err_t
apac_barrier_destroy(apac_barrier_t* b)
{
    return pthread_barrier_destroy(b);
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
        apac_mutex_lock(&pool->lock);

        while (pool->queue_curr_size == 0 && !pool->shutdown)
        {
            apac_cond_wait(&pool->queue_not_empty, &pool->lock);
        }

        if (pool->shutdown && pool->queue_curr_size == 0)
        {
            apac_mutex_unlock(&pool->lock);
            break; 
        }

        apac_work_t task = pool->task_queue[pool->queue_head]; // assumes task has valid func arg
        pool->queue_head = (pool->queue_head + 1) % pool->queue_max_size;
        pool->queue_curr_size--;

        apac_cond_signal(&pool->queue_not_full);

        pool->active_thrds++;

        apac_mutex_unlock(&pool->lock);

        task.func(task.arg); // assumes func is a valid func ptr

        apac_mutex_lock(&pool->lock);

        pool->active_thrds--;

        if (pool->queue_curr_size == 0 && pool->active_thrds == 0)
        {
            apac_cond_broadcast(&pool->queue_empty);
        }

        apac_mutex_unlock(&pool->lock);
    }

    return 0;
}

apac_thrd_err_t
apac_tpool_init(
    apac_tpool_t* pool, 
    size_t thrd_count, 
    size_t work_queue_size
)
{
    APAC_ASSERT(pool != NULL);
    APAC_ASSERT(thrd_count != 0);
    APAC_ASSERT(thrd_count <= SIZE_MAX / sizeof(apac_thrd_t));
    APAC_ASSERT(work_queue_size != 0);
    APAC_ASSERT(work_queue_size <= SIZE_MAX / sizeof(apac_work_t));

    // keep this function scoped so in case of failure 
    // at i, clear all threads from (i - 1)th to 0th
    size_t i = 0; 
    apac_thrd_err_t result = APAC_THRD_OK;

    pool->max_thrds         = thrd_count;
    pool->queue_curr_size   = 0;
    pool->queue_max_size    = work_queue_size;
    pool->queue_head        = 0;
    pool->active_thrds      = 0;
    pool->shutdown          = false;

    // NULL initialize so that default 
    // garbage doesn't mess up our 
    // cleanup code
    pool->workers       = NULL;
    pool->task_queue    = NULL;

    apac_mutex_init(&pool->lock);
    apac_cond_init(&pool->queue_empty);
    apac_cond_init(&pool->queue_not_empty);
    apac_cond_init(&pool->queue_not_full);

    pool->workers = apac_malloc(sizeof(apac_thrd_t) * thrd_count);
    if (!pool->workers) 
    {
        result = APAC_THRD_OOM;
        goto fail_cleanup; 
    }

    pool->task_queue = apac_malloc(sizeof(apac_work_t) * pool->queue_max_size);
    if (!pool->task_queue) 
    {
        result = APAC_THRD_OOM;
        goto fail_cleanup; 
    }
    
    for (i = 0; i < thrd_count; i++)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        HANDLE handle = CreateThread(NULL, 0, apac_tpool_worker, (void*)pool, 0, NULL);

        if (handle == NULL)
        {
            result = APAC_THRD_CREATE_FAIL;
            goto fail_cleanup;
        }

        pool->workers[i] = handle;

#else

        int handle = pthread_create(&pool->workers[i], NULL, apac_tpool_worker, (void*)pool);

        if (handle != 0)
        {
            result = APAC_THRD_CREATE_FAIL;
            goto fail_cleanup;
        }
#endif
    }

    return result;

fail_cleanup:

    apac_mutex_lock(&pool->lock);

    pool->shutdown = true;

    // partially ready threads might be waiting on this signal
    apac_cond_broadcast(&pool->queue_not_empty);

    apac_mutex_unlock(&pool->lock);
    
    while (i-- > 0)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        WaitForSingleObject(pool->workers[i], INFINITE);
        CloseHandle(pool->workers[i]);

#else

        pthread_join(pool->workers[i], NULL);

#endif
    }

    if (pool->task_queue) { apac_free(pool->task_queue); }
    if (pool->workers) { apac_free(pool->workers); }

    apac_cond_destroy(&pool->queue_not_full);
    apac_cond_destroy(&pool->queue_not_empty);
    apac_cond_destroy(&pool->queue_empty);
    apac_mutex_destroy(&pool->lock);

    return result;
}

apac_thrd_err_t
apac_tpool_submit(
    apac_tpool_t* pool,
    apac_thrd_func_t func,
    apac_thrd_arg_t arg
)
{
    APAC_ASSERT(pool != NULL);
    APAC_ASSERT(func != NULL);
    APAC_ASSERT(arg != NULL);

    apac_mutex_lock(&pool->lock);
    
    while ((pool->queue_curr_size == pool->queue_max_size) && !pool->shutdown)
    {
        apac_cond_wait(&pool->queue_not_full, &pool->lock);
    }

    // drop current work
    if (pool->shutdown)
    {
        apac_mutex_unlock(&pool->lock);
        return APAC_THRD_POOL_SHUT;
    }

    size_t queue_end = (pool->queue_head + pool->queue_curr_size) % pool->queue_max_size;
    pool->task_queue[queue_end].func = func;
    pool->task_queue[queue_end].arg = arg;
    pool->queue_curr_size++;
    
    apac_cond_signal(&pool->queue_not_empty);
    apac_mutex_unlock(&pool->lock);

    return APAC_THRD_OK;
}

apac_thrd_err_t
apac_tpool_wait(
    apac_tpool_t* pool
)
{
    APAC_ASSERT(pool != NULL);

    apac_mutex_lock(&pool->lock);

    while (pool->queue_curr_size != 0 || pool->active_thrds != 0)
    {
        apac_cond_wait(&pool->queue_empty, &pool->lock);
    }

    apac_mutex_unlock(&pool->lock);
    return APAC_THRD_OK;
}

apac_thrd_err_t
apac_tpool_destroy(
    apac_tpool_t* pool
)
{
    APAC_ASSERT(pool != NULL);

    apac_mutex_lock(&pool->lock);

    pool->shutdown = true;

    // wake:
    // - workers waiting for work
    // - producers waiting for space
    apac_cond_broadcast(&pool->queue_not_empty);
    apac_cond_broadcast(&pool->queue_not_full);

    apac_mutex_unlock(&pool->lock);

    // join all worker threads
    for (size_t i = 0; i < pool->max_thrds; i++)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        WaitForSingleObject(pool->workers[i], INFINITE);
        CloseHandle(pool->workers[i]);

#else

        pthread_join(pool->workers[i], NULL);

#endif
    }

    apac_free(pool->task_queue);
    apac_free(pool->workers);

    apac_cond_destroy(&pool->queue_not_full);
    apac_cond_destroy(&pool->queue_not_empty);
    apac_cond_destroy(&pool->queue_empty);

    apac_mutex_destroy(&pool->lock);

    return APAC_THRD_OK;
}

size_t
apac_tpool_get_queue_capacity(
    apac_tpool_t* pool
)
{
    APAC_ASSERT(pool != NULL);

    return pool->queue_max_size;
}

size_t
apac_tpool_get_max_thrd_count(
    apac_tpool_t* pool
)
{
    APAC_ASSERT(pool != NULL);

    return pool->max_thrds;
}

apac_thrd_err_t
apac_tpool_set_size(
    apac_tpool_t* pool,
    size_t new_max_thrds,
    size_t new_work_queue_size
)
{
    APAC_ASSERT(pool != NULL);

    APAC_ASSERT(new_max_thrds != 0);
    APAC_ASSERT(new_max_thrds <= SIZE_MAX / sizeof(apac_thrd_t));

    APAC_ASSERT(new_work_queue_size != 0);
    APAC_ASSERT(new_work_queue_size <= SIZE_MAX / sizeof(apac_work_t));

    size_t i = 0;
    apac_thrd_err_t result = APAC_THRD_OK;

    apac_mutex_lock(&pool->lock);

    // resize only allowed when completely idle
    if (pool->queue_curr_size != 0 ||
        pool->active_thrds != 0)
    {
        apac_mutex_unlock(&pool->lock);
        return APAC_THRD_BUSY;
    }

    // stop workers
    pool->shutdown = true;

    apac_cond_broadcast(&pool->queue_not_empty);
    apac_cond_broadcast(&pool->queue_not_full);

    apac_mutex_unlock(&pool->lock);

    // join old workers
    for (size_t i = 0; i < pool->max_thrds; i++)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        WaitForSingleObject(pool->workers[i], INFINITE);
        CloseHandle(pool->workers[i]);

#else

        pthread_join(pool->workers[i], NULL);

#endif
    }

    // allocate new resources first so old ones remain
    // untouched if allocation fails
    apac_thrd_t* new_workers = apac_malloc(sizeof(apac_thrd_t) * new_max_thrds);

    if (!new_workers)
    {
        result = APAC_THRD_OOM;
        goto fail;
    }

    apac_work_t* new_task_queue = apac_malloc(sizeof(apac_work_t) * new_work_queue_size);

    if (!new_task_queue)
    {
        apac_free(new_workers);
        result = APAC_THRD_OOM;
        goto fail;
    }

    // old resources no longer needed
    apac_free(pool->workers);
    apac_free(pool->task_queue);

    pool->workers = new_workers;
    pool->task_queue = new_task_queue;

    pool->max_thrds = new_max_thrds;

    pool->queue_max_size = new_work_queue_size;
    pool->queue_curr_size = 0;
    pool->queue_head = 0;

    pool->active_thrds = 0;
    pool->shutdown = false;

    for (i = 0; i < new_max_thrds; i++)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        HANDLE handle = CreateThread(
            NULL,
            0,
            apac_tpool_worker,
            (void*)pool,
            0,
            NULL
        );

        if (handle == NULL)
        {
            result = APAC_THRD_CREATE_FAIL;
            goto fail_partial_workers;
        }

        pool->workers[i] = handle;

#else

        int handle = pthread_create(
            &pool->workers[i],
            NULL,
            apac_tpool_worker,
            (void*)pool
        );

        if (handle != 0)
        {
            result = APAC_THRD_CREATE_FAIL;
            goto fail_partial_workers;
        }

#endif
    }

    return result;

fail_partial_workers:

    apac_mutex_lock(&pool->lock);

    pool->shutdown = true;

    apac_cond_broadcast(&pool->queue_not_empty);

    apac_mutex_unlock(&pool->lock);

    while (i-- > 0)
    {
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        WaitForSingleObject(pool->workers[i], INFINITE);
        CloseHandle(pool->workers[i]);

#else

        pthread_join(pool->workers[i], NULL);

#endif
    }

fail:

    return result;
}

apac_thrd_err_t
apac_wtgrp_init(
    apac_wtgrp_t* wg
)
{
    APAC_ASSERT(wg != NULL);

    wg->count = 0;

    apac_mutex_init(&wg->lock);
    apac_cond_init(&wg->cond);

    return APAC_THRD_OK;
}

apac_thrd_err_t
apac_wtgrp_destroy(
    apac_wtgrp_t* wg
)
{
    APAC_ASSERT(wg != NULL);

    apac_cond_destroy(&wg->cond);
    apac_mutex_destroy(&wg->lock);

    return APAC_THRD_OK;
}

apac_thrd_err_t
apac_wtgrp_add(
    apac_wtgrp_t* wg,
    size_t delta
)
{
    APAC_ASSERT(wg != NULL);

    apac_mutex_lock(&wg->lock);

    APAC_ASSERT(wg->count <= SIZE_MAX - delta);

    wg->count += delta;

    apac_mutex_unlock(&wg->lock);

    return APAC_THRD_OK;
}

apac_thrd_err_t
apac_wtgrp_done(
    apac_wtgrp_t* wg
)
{
    APAC_ASSERT(wg != NULL);

    apac_mutex_lock(&wg->lock);

    APAC_ASSERT(wg->count != 0);

    wg->count--;

    if (wg->count == 0)
    {
        apac_cond_broadcast(&wg->cond);
    }

    apac_mutex_unlock(&wg->lock);

    return APAC_THRD_OK;
}

apac_thrd_err_t
apac_wtgrp_wait(
    apac_wtgrp_t* wg
)
{
    APAC_ASSERT(wg != NULL);

    apac_mutex_lock(&wg->lock);

    while (wg->count != 0)
    {
        apac_cond_wait(&wg->cond, &wg->lock);
    }

    apac_mutex_unlock(&wg->lock);

    return APAC_THRD_OK;
}