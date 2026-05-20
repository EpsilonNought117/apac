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

typedef apac_thrd_ret_t (APAC_THRD_CALL *apac_thrd_func_t)(apac_thrd_arg_t);

typedef struct apac_work_t
{
	apac_thrd_func_t func;
	apac_thrd_arg_t  arg;

} apac_work_t;

struct apac_tpool_t
{
	apac_mutex_t lock;
	apac_cond_t notify, wait, queue_full;

	apac_thrd_t* workers;
	apac_work_t* task_queue; // circular buffer

	size_t thrd_count, active_thrds;	// queue max size is 8 * thrd_count
	size_t queue_curr_size, queue_max_size, queue_head;
	bool shutdown;
};

#endif