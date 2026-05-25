#ifndef APAC_THREADPOOL_H
#define APAC_THREADPOOL_H

#include "../header/apac_internal.h"

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)
	
	typedef HANDLE					apac_thrd_t;
	typedef SRWLOCK					apac_mutex_t;
    typedef CONDITION_VARIABLE      apac_cond_t;
    typedef SYNCHRONIZATION_BARRIER	apac_barrier_t;
	typedef DWORD					apac_thrd_err_t;

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

	typedef pthread_t				apac_thrd_t;
	typedef pthread_cond_t			apac_cond_t;
	typedef pthread_mutex_t			apac_mutex_t;
	typedef pthread_barrier_t		apac_barrier_t;
	typedef int						apac_thrd_err_t;

#else
	#error "Unknown Platform and CPU Architecture!"
#endif

// ALL IS GOOD
#define APAC_THRD_OK			(apac_thrd_err_t)0
// RAN OUT-OF-MEMORY
#define APAC_THRD_OOM			(apac_thrd_err_t)1
// FAILED TO CREATE THREAD HANDLE
#define APAC_THRD_CREATE_FAIL	(apac_thrd_err_t)2
// THREAD RESOURCE BUSY
#define APAC_THRD_BUSY			(apac_thrd_err_t)3
// THREADPOOL SHUTTING DOWN
#define APAC_THRD_POOL_SHUT		(apac_thrd_err_t)4 

typedef apac_thrd_ret_t	
		(APAC_THRD_CALL* apac_thrd_func_t)(apac_thrd_arg_t);

typedef struct apac_work_t
{
	apac_thrd_func_t func;
	apac_thrd_arg_t  arg;

} apac_work_t;

typedef struct apac_tpool_t
{
	apac_mutex_t lock;
	apac_cond_t queue_empty;		// signal if queue has no work
	apac_cond_t	queue_not_empty;	// signal if queue is fully filled 
	apac_cond_t queue_not_full;		// signal if queue has spot

	apac_thrd_t* workers;
	apac_work_t* task_queue;		// circular FIFO buffer

	size_t	max_thrds, active_thrds; 
	size_t	queue_curr_size, 
			queue_max_size, 
			queue_head;

	bool shutdown;

} apac_tpool_t;

// ============================================================================
// Barrier
// ============================================================================

apac_thrd_err_t
apac_barrier_init(
	apac_barrier_t* b,
	unsigned count
);

apac_thrd_err_t
apac_barrier_wait(
	apac_barrier_t* b
);

apac_thrd_err_t
apac_barrier_destroy(
	apac_barrier_t* b
);

// ============================================================================
// Thread Pool
// ============================================================================

apac_thrd_err_t
apac_tpool_init(
	apac_tpool_t* pool,
	size_t thrd_count,
	size_t work_queue_size
);

apac_thrd_err_t
apac_tpool_destroy(
	apac_tpool_t* pool
);

apac_thrd_err_t
apac_tpool_submit(
	apac_tpool_t* pool,
	apac_thrd_func_t func,
	apac_thrd_arg_t arg
);

apac_thrd_err_t
apac_tpool_wait(
	apac_tpool_t* pool
);

apac_thrd_err_t
apac_tpool_set_size(
	apac_tpool_t* pool,
	size_t new_max_thrds,
	size_t new_work_queue_size
);

size_t
apac_tpool_get_queue_capacity(
	apac_tpool_t* pool
);

size_t
apac_tpool_get_max_thrd_count(
	apac_tpool_t* pool
);

#endif