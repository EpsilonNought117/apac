#ifndef APAC_THREADPOOL_H
#define APAC_THREADPOOL_H

#include "../../include/apac.h"

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

	typedef HANDLE					apac_thread_t;
	#define APAC_THREAD_RETVAL      uintptr_t

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

	typedef pthread_t				apac_thread_t;
	#define APAC_THREAD_RETVAL      int

#else
	#error "Unknown Platform and CPU Architecture!"
#endif

#endif