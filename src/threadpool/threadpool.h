#ifndef APAC_THREADPOOL_H
#define APAC_THREADPOOL_H

#include "../../include/apac.h"

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

	#include <process.h>
	
	typedef HANDLE					apac_thrd_t;
	typedef CRITICAL_SECTION        apac_mutex_t;
    typedef CONDITION_VARIABLE      apac_cond_t;
    typedef SYNCHRONIZATION_BARRIER	apac_barrier_t;

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

	typedef pthread_t			apac_thrd_t;
	typedef pthread_cond_t		apac_cond_t;
	typedef pthread_mutex_t		apac_mutex_t;
	typedef pthread_barrier_t	apac_barrier_t;

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
	apac_work_t* task_queue;

	size_t thrd_count, queue_size;
	size_t queue_head, queue_tail;
	size_t active_thrds;
	bool shutdown;
};

#endif