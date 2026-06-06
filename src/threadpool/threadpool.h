#ifndef APAC_THREADPOOL_H
#define APAC_THREADPOOL_H

#include "../header/apac_internal.h"

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)
	
	typedef HANDLE					apac_thrd_t;
	typedef SRWLOCK					apac_mutex_t;
    typedef CONDITION_VARIABLE      apac_cond_t;

	#define APAC_THRD_CALL  		WINAPI
	typedef DWORD           		apac_thrd_ret_t;
	typedef LPVOID          		apac_thrd_arg_t;

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

	typedef pthread_t				apac_thrd_t;
	typedef pthread_cond_t			apac_cond_t;
	typedef pthread_mutex_t			apac_mutex_t;

	#define APAC_THRD_CALL          /* nothing */
    typedef void*           		apac_thrd_ret_t;    
    typedef void*           		apac_thrd_arg_t;

#else
	#error "Unknown Platform and CPU Architecture!"
#endif

typedef struct
{
    /* Worker threads */

    apac_thrd_t* workers;
    ap_size_t max_thrds;

    /* Current parallel-for */

    apac_pfor_func_t func;
    void* arg;

    ap_size_t begin;
    ap_size_t end;

    apac_sched_t sched;

    /* Dynamic scheduling */

    ap_size_t chunk_size;
    ap_size_t next;

    /* Completion tracking */

    ap_size_t active_thrds;

    /* Synchronization */

    apac_mutex_t lock;

    apac_cond_t work_ready;
    apac_cond_t work_done;

    bool has_work;
    bool shutdown;

} apac_tpool_t;

// ============================================================================
// Global Vars
// ============================================================================

static apac_tpool_t tpool;

static bool is_tpool_init = false;

#endif