#ifndef APAC_INTERNAL_H
#define APAC_INTERNAL_H

#include "../../include/apac.h"

/****************************************************************************************************/
/***************************      REQUIRED C STANDARD LIBRARY HEADERS      **************************/
/****************************************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <inttypes.h>
#include <stdint.h>

/****************************************************************************************************/
/***********************   COMPILER SPECIFIC HEADERS, TYPEDEFS AND MACROS    ************************/
/****************************************************************************************************/

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

    #include <intrin.h>

	#if defined(APAC_X64_WIN)

		#include <immintrin.h>
	
	#elif defined(APAC_ARM64_WIN)

        #include <arm64_neon.h>

	#endif

	#define APAC_THRD_LOCAL __declspec(thread)

	#define CLZ(value, count)																	\
			do																					\
			{																					\
				unsigned long idx = 0;															\
				(count) = _BitScanReverse64(&idx, (value)) ? (ap_dig_t)63 - idx : (ap_dig_t)64;	\
			} while (0)

	#define CTZ(value, count)														\
			do																		\
			{																		\
				unsigned long idx = 0;												\
				(count) = _BitScanForward64(&idx, (value)) ? idx : (ap_dig_t)64;	\
			} while (0)

	#define ROTL(value, count)	do { value = _rotl64((value), (count)); } while (0)

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

	#include <unistd.h>
    #include <time.h>
    #include <sched.h>

    #if defined(_POSIX_THREADS) && (_POSIX_THREADS > 0)
        #include <pthread.h>
    #else
        #error "PThreads are needed for working with libapac!"
    #endif

	#if defined(APAC_X64_UNIX)

		#include <cpuid.h>
		#include <immintrin.h>

	#elif defined(APAC_ARM64_UNIX)

		#include <arm_neon.h>  
	
	#endif

    #define APAC_THRD_LOCAL __thread            /* GCC/Clang extension */

	#define CLZ(value, count)															\
			do																			\
			{																			\
				(count) = (value) ? (ap_dig_t)__builtin_clzll((value)) : (ap_dig_t)64;	\
			} while (0)

	#define CTZ(value, count)															\
			do																			\
			{																			\
				(count) = (value) ? (ap_dig_t)__builtin_ctzll((value)) : (ap_dig_t)64;	\
			} while(0)

	#define ROTL(value, count)											\
			do															\
			{															\
				value = (												\
					(((uint64_t)(value)) << ((count) & 63)) 	   	| 	\
					(((uint64_t)(value)) >> (64 - ((count) & 63)))		\
				);														\
			} while (0)

#else

	#error "Unknown Compiler, OS Platform and CPU ISA!"

#endif


#if (defined(APAC_X64_WIN)      ||  \
     defined(APAC_X64_UNIX)     ||  \
     defined(APAC_ARM64_WIN)    ||  \
     defined(APAC_ARM64_UNIX)       \
    )

	#define APAC_64BIT_PLATFORM	1

    #define PRI_APN_SIZE        "llu"

    #define APN_SIZE_MAX        ULLONG_MAX
    #define APN_DIG_MAX         ULLONG_MAX

    #define APN_DIG_BITS        64U
    #define APN_DIG_HIGH_BIT    (1ULL << 63)

#endif

#define APAC_PRAGMA(x) _Pragma(#x)

#if defined(_MSC_VER)

    #define APAC_UNROLL(x)

#elif defined(__GNUC__)

    #define APAC_UNROLL(x) APAC_PRAGMA(GCC unroll x)

#elif defined(__clang__)

    #define APAC_UNROLL(x) APAC_PRAGMA(unroll x)
    
#else

    #define APAC_UNROLL(x)

#endif

/****************************************************************************************************/
/*********************************      ERROR HANDLING MACROS      **********************************/
/****************************************************************************************************/

/* ==========================================================================
 * Always-Enabled Assertion
 * ========================================================================== */

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

/* ==========================================================================
 * Debug-Only Assertion
 * ========================================================================== */

#ifndef APAC_DISABLE_ASSERT
    #define APAC_ASSERT(expr)   APAC_ALWAYS_ASSERT(expr)
#else
    #define APAC_ASSERT(expr)   do { /* nothing */ } while (0)
#endif

/* ==========================================================================
 * Debug-Only Overlap Checks
 * ========================================================================== */

/*
    These checks are only useful on platforms with a flat address space per process.
*/
#if !defined(APAC_DISABLE_ASSERT)   &&  \
    (defined(APAC_X64_WIN)          ||  \
     defined(APAC_X64_UNIX)         ||  \
     defined(APAC_ARM64_WIN)        ||  \
     defined(APAC_ARM64_UNIX)           \
    )

    #define APAC_NO_OVERLAP(op1, size1, op2, size2)                             \
            APAC_ALWAYS_ASSERT(                                                 \
                ((uintptr_t)(op1) + (size1)) <= (uintptr_t)(op2) ||             \
                ((uintptr_t)(op2) + (size2)) <= (uintptr_t)(op1)                \
            )

    #define APAC_FULL_ALIAS_ONLY(op1, size1, op2, size2)                        \
            APAC_ALWAYS_ASSERT(                                                 \
                (                                                               \
                    (uintptr_t)(op1) == (uintptr_t)(op2) &&                     \
                    (uintptr_t)(op1) + (size1) == (uintptr_t)(op2) + (size2)    \
                ) ||                                                            \
                (                                                               \
                    ((uintptr_t)(op1) + (size1)) <= (uintptr_t)(op2) ||         \
                    ((uintptr_t)(op2) + (size2)) <= (uintptr_t)(op1)            \
                )                                                               \
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

#else

    #define APAC_NO_OVERLAP(op1, size1, op2, size2)             do { /* nothing */ } while (0)
    #define APAC_FULL_ALIAS_ONLY(op1, size1, op2, size2)        do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)  do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)  do { /* nothing */ } while (0)

#endif

/****************************************************************************************************/
/*********************************         MISCELLANEOUS          ***********************************/
/****************************************************************************************************/

/* ==========================================================================
 * Global Allocator Struct
 * ========================================================================== */

typedef struct apac_alloc_t
{
    apac_malloc_t custom_malloc;
    apac_realloc_t custom_realloc;
    apac_free_t custom_free;
    void* ctx;

} apac_alloc_t;

extern apac_alloc_t apac_allocator;

/* ==========================================================================
 * Utility Functions for Testing and Benchmarking
 * ========================================================================== */

uint64_t apac_cpu_timer(void);

uint64_t apac_os_timer(void);

int apac_pin_thread_to_core(uint32_t core_id);

void apac_disable_dfs(void);

void apac_restore_dfs(void);

/* ==========================================================================
 * Runtime CPU Detection and Low-Level Function Dispatch Struct
 * ========================================================================== */

typedef struct apac_cpu_params
{
    ap_size_t karatsuba_mul_threshold;
    ap_size_t karatsuba_sqr_threshold;
    ap_size_t dnc_div_threshold;

    ap_dig_t(*apn_add_n_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t);
    ap_dig_t(*apn_sub_n_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t);
    ap_dig_t(*apn_add_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t(*apn_sub_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    void (*apn_neg_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t);

    ap_dig_t(*apn_addmul_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t(*apn_submul_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);

    ap_dig_t(*apn_lshift_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t(*apn_rshift_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);

    void (*apn_mul_bc_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t, ap_size_t);
    void (*apn_sqr_bc_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t);

    void (*apn_cpy_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t);
    void (*apn_set_ptr)(ap_dig_t*, ap_size_t, ap_dig_t);

    int (*apn_cmp_ptr)(const ap_dig_t*, const ap_dig_t*, ap_size_t);
    int (*apn_is_zero_ptr)(const ap_dig_t*, ap_size_t);

} apac_cpu_params;

extern apac_cpu_params curr_cpu;

/* ==========================================================================
 * Algorithm Switching Threshold Macros
 * ========================================================================== */

#define KARATSUBA_MUL_THRESHOLD (curr_cpu.karatsuba_mul_threshold)
#define KARATSUBA_SQR_THRESHOLD	(curr_cpu.karatsuba_sqr_threshold)
#define DNC_DIV_THRESHOLD	    (curr_cpu.dnc_div_threshold)

/****************************************************************************************************/
/*********************************     APN INTERNAL FUNCTIONS     ***********************************/
/****************************************************************************************************/

ap_dig_t*
apn_reduce_mod_p_u64(
    ap_dig_t* arr,
    ap_size_t n,
    ap_dig_t prime,
    ap_dig_t magic,
    ap_dig_t shift
);

double*
apn_reduce_mod_p_f64(
    ap_dig_t* arr,
    ap_size_t n,
    ap_dig_t prime,
    ap_dig_t magic,
    ap_dig_t shift
);

#endif