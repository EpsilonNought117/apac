#ifndef APAC_INTERNAL_H
#define APAC_INTERNAL_H

#include "../../include/apac.h"

/* ---------------------------------------------------------------------------------------------- */
/*                              REQUIRED C STANDARD LIBRARY HEADERS                               */
/* ---------------------------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <inttypes.h>
#include <stdint.h>

/* ---------------------------------------------------------------------------------------------- */
/*                            COMPILER AND PLATFORM SPECIFIC HEADERS                              */
/* ---------------------------------------------------------------------------------------------- */

#if defined(APAC_WIN_X64) || defined(APAC_WIN_ARM64)

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	#include <intrin.h>

	#if defined(APAC_WIN_X64)
		#include <immintrin.h>
	#endif

	/* --- MSVC always uses C11 threads --- */

	#if (_MSC_VER < 1939)
		#error "C11 <threads.h> requires MSVC 19.39 (Visual Studio 2022 17.9) or later."
	#endif

    #include <stdatomic.h>
	#include <threads.h>

	#define APAC_USE_C11_THREADS	1
	#define APAC_THRD_LOCAL			_Thread_local

	#define CLZ(value, count)																		\
			do																						\
			{																						\
				unsigned long idx = 0;																\
				(count) = _BitScanReverse64(&idx, (value)) ? (apn_dig_t)63 - idx : (apn_dig_t)64;	\
			} while (0)

	#define CTZ(value, count)														\
			do																		\
			{																		\
				unsigned long idx = 0;												\
				(count) = _BitScanForward64(&idx, (value)) ? idx : (apn_dig_t)64;	\
			} while (0)

	#define ROTL(value, count)	do { value = _rotl64((value), (count)); } while (0)

#elif defined(APAC_LINUX_X64) || defined(APAC_LINUX_ARM64)	|| \
	  defined(APAC_MACOS_X64) || defined(APAC_MACOS_ARM64)

	#include <unistd.h>
	#include <time.h>
	#include <sched.h>
    #include <stdatomic.h>
	#include <pthread.h>

	#if defined(APAC_LINUX_X64)

		#include <cpuid.h>
		#include <immintrin.h>

	#elif defined(APAC_LINUX_ARM64)
	
	#endif

	#define APAC_USE_PTHREADS		1
	#define APAC_THRD_LOCAL			__thread	/* GCC/Clang extension */

	#define CLZ(value, count)																\
			do																				\
			{																				\
				(count) = (value) ? (apn_dig_t)__builtin_clzll((value)) : (apn_dig_t)64;	\
			} while (0)

	#define CTZ(value, count)																\
			do																				\
			{																				\
				(count) = (value) ? (apn_dig_t)__builtin_ctzll((value)) : (apn_dig_t)64;	\
			} while(0)

	#define ROTL(value, count)											\
			do															\
			{															\
				value = (												\
					(((apn_dig_t)(value)) << ((count) & 63)) 	   	| 	\
					(((apn_dig_t)(value)) >> (64 - ((count) & 63)))		\
				);														\
			} while (0)
#endif

/* ---------------------------------------------------------------------------------------------- */
/*                                        MACRO DEFINITIONS                                       */
/* ---------------------------------------------------------------------------------------------- */

#if defined(APAC_WIN_X64)       ||  \
    defined(APAC_LINUX_X64)     ||  \
    defined(APAC_MACOS_X64)     ||  \
    defined(APAC_WIN_ARM64)     ||  \
    defined(APAC_LINUX_ARM64)   ||  \
    defined(APAC_MACOS_ARM64)

	#define APAC_64BIT_PLATFORM	1

    #define PRI_APN_SIZE        "llu"

    #define APN_SIZE_MAX        ULLONG_MAX
    #define APN_DIG_MAX         ULLONG_MAX

    #define APN_DIG_BITS        64U
    #define APN_DIG_HIGH_BIT    (1ULL << 63)

#endif

/* ---------------------------------------------------------------------------------------------- */
/*                                  ALWAYS ENABLED ASSERT-&-ABORT                                 */
/* ---------------------------------------------------------------------------------------------- */

#define APAC_ALWAYS_ASSERT(expr)                        \
        do                                              \
        {                                               \
            if (!(expr))                                \
            {                                           \
                fprintf(                                \
                    stderr,                             \
                    "\n\nAPAC ASSERTION FAILED! \n"     \
                    "ASSERTION: %s              \n"     \
                    "FILE: %s\nLINE: %d         \n"     \
                    "ABORTING ...             \n\n",    \
                    #expr, __FILE__, __LINE__           \
                );                                      \
                abort();                                \
            }                                           \
        } while (0)

/* ---------------------------------------------------------------------------------------------- */
/*                                 DEBUG-BUILD ONLY ASSERT-&-ABORT                                */
/* ---------------------------------------------------------------------------------------------- */

#if !defined(APAC_DISABLE_ASSERT)
    #define APAC_ASSERT(expr)   APAC_ALWAYS_ASSERT(expr)
#else
    #define APAC_ASSERT(expr)   do { /* nothing */ } while (0)
#endif

/* ---------------------------------------------------------------------------------------------- */
/*                                 DEBUG-BUILD ONLY OVERLAP CHECKS                                */
/* ---------------------------------------------------------------------------------------------- */

#if !defined(APAC_DISABLE_ASSERT)

    /* These checks are only useful on platforms with a flat address space per process. */

    #if defined(APAC_WIN_X64)       ||  \
        defined(APAC_LINUX_X64)     ||  \
        defined(APAC_MACOS_X64)     ||  \
        defined(APAC_WIN_ARM64)     ||  \
        defined(APAC_LINUX_ARM64)   ||  \
        defined(APAC_MACOS_ARM64)

        #define APAC_NO_OVERLAP(op1, size1, op2, size2)                             \
                APAC_ALWAYS_ASSERT(                                                 \
                    ((uintptr_t)(op1) + (size1)) <= (uintptr_t)(op2) ||             \
                    ((uintptr_t)(op2) + (size2)) <= (uintptr_t)(op1)                \
                )

        #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)                  \
                APAC_ALWAYS_ASSERT(                                                 \
                    ((uintptr_t)(op1) + (size1)) <= ((uintptr_t)(op2) + (size2)) || \
                    ((uintptr_t)(op2) + (size2)) <= (uintptr_t)(op1)                \
                )

        #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)                  \
                APAC_ALWAYS_ASSERT(                                                 \
                    ((uintptr_t)(op2) + (size2)) <= ((uintptr_t)(op1) + (size1)) || \
                    ((uintptr_t)(op1) + (size1)) <= (uintptr_t)(op2)                \
                )
    #endif

#else

    #define APAC_NO_OVERLAP(op1, size1, op2, size2)             do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)  do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)  do { /* nothing */ } while (0)

#endif

/* ---------------------------------------------------------------------------------------------- */
/*                                      GLOBAL MEMORY ALLOCATOR                                   */
/* ---------------------------------------------------------------------------------------------- */

typedef struct apac_alloc_t
{
    apac_malloc_t custom_malloc;
    apac_realloc_t custom_realloc;
    apac_free_t custom_free;
    void* ctx;

} apac_alloc_t;

extern apac_alloc_t apac_allocator;

/* ---------------------------------------------------------------------------------------------- */
/*                                         UTILITY FUNCTIONS                                      */
/* ---------------------------------------------------------------------------------------------- */

uint64_t apac_cpu_timer(void);

int apac_pin_thread_to_core(uint32_t core_id);

void apac_disable_dfs(void);

void apac_restore_dfs(void);

/* ---------------------------------------------------------------------------------------------- */
/*                                   RUNTIME CPU DISPATCH STRUCT                                  */
/* ---------------------------------------------------------------------------------------------- */

typedef struct apac_cpu_params
{
    apn_size_t karatsuba_mul_threshold;
    apn_size_t karatsuba_sqr_threshold;
    apn_size_t dnc_div_threshold;

    apn_dig_t(*apn_add_n_ptr)(apn_dig_t*, const apn_dig_t*, const apn_dig_t*, apn_size_t);
    apn_dig_t(*apn_sub_n_ptr)(apn_dig_t*, const apn_dig_t*, const apn_dig_t*, apn_size_t);
    apn_dig_t(*apn_add_one_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t, apn_dig_t);
    apn_dig_t(*apn_sub_one_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t, apn_dig_t);
    void (*apn_neg_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t);

    void (*apn_mul_one_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t, apn_dig_t);
    apn_dig_t(*apn_addmul_one_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t, apn_dig_t);
    apn_dig_t(*apn_submul_one_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t, apn_dig_t);

    apn_dig_t(*apn_lshift_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t, apn_dig_t);
    apn_dig_t(*apn_rshift_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t, apn_dig_t);

    void (*apn_mul_bc_ptr)(apn_dig_t*, const apn_dig_t*, const apn_dig_t*, apn_size_t, apn_size_t);
    void (*apn_sqr_bc_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t);

    void (*apn_cpy_ptr)(apn_dig_t*, const apn_dig_t*, apn_size_t);
    void (*apn_set_ptr)(apn_dig_t*, apn_size_t, apn_dig_t);

    int (*apn_cmp_ptr)(const apn_dig_t*, const apn_dig_t*, apn_size_t);
    int (*apn_is_zero_ptr)(const apn_dig_t*, apn_size_t);

} apac_cpu_params;

extern apac_cpu_params curr_cpu;

/* ---------------------------------------------------------------------------------------------- */
/*                                  ALGORITHM SWITCHING THRESHOLDS                                */
/* ---------------------------------------------------------------------------------------------- */

#define KARATSUBA_MUL_THRESHOLD (curr_cpu.karatsuba_mul_threshold)
#define KARATSUBA_SQR_THRESHOLD	(curr_cpu.karatsuba_sqr_threshold)
#define DNC_DIV_THRESHOLD	    (curr_cpu.dnc_div_threshold)

#endif