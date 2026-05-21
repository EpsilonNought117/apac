#ifndef APAC_THREADPOOL_H
#define APAC_THREADPOOL_H

#include "../../include/apac.h"

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

typedef struct apac_work_t
{
	apac_thrd_func_t func;
	apac_thrd_arg_t  arg;

} apac_work_t;

struct apac_tpool_t
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
};

struct apac_wtgrp_t
{
	apac_mutex_t lock;
	apac_cond_t  cond;

	size_t count;
};

// ============================================================================
// Mutex
// ============================================================================

apac_thrd_err_t
apac_mutex_init(
	apac_mutex_t* m
);

apac_thrd_err_t
apac_mutex_lock(
	apac_mutex_t* m
);

apac_thrd_err_t
apac_mutex_unlock(
	apac_mutex_t* m
);

apac_thrd_err_t
apac_mutex_destroy(
	apac_mutex_t* m
);

// ============================================================================
// Condition Variable
// ============================================================================

apac_thrd_err_t
apac_cond_init(
	apac_cond_t* c
);

apac_thrd_err_t
apac_cond_signal(
	apac_cond_t* c
);

apac_thrd_err_t
apac_cond_broadcast(
	apac_cond_t* c
);

apac_thrd_err_t
apac_cond_wait(
	apac_cond_t* c,
	apac_mutex_t* m
);

apac_thrd_err_t
apac_cond_destroy(
	apac_cond_t* c
);

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

#endif