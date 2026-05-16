#include "threadpool.h"

static APAC_THREAD_RETVAL
apac_thread_init(
    apac_thread_t* thread,
    apac_mt_func_t func,
    void* arg
)
{
#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

    unsigned tid;

    *thread = (HANDLE)_beginthreadex(
        NULL,
        0,
        func,
        arg,
        CREATE_SUSPENDED,
        &tid
    );

    return (uintptr_t)(*thread);

#else

    /*
        pthread has no CREATE_SUSPENDED equivalent.

        Thread function must begin blocked on an
        internal condition variable / semaphore.

        So here we only create the thread.
    */

    return pthread_create(
        thread,
        NULL,
        func,
        arg
    );

#endif
}