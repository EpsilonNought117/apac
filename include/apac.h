#ifndef APAC_H
#define APAC_H

#if defined(__GNUC__)
    #define _GNU_SOURCE
#endif

/****************************************************************************************************/
/***************************      REQUIRED C STANDARD LIBRARY HEADERS      **************************/
/****************************************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <memory.h>
#include <inttypes.h>

/****************************************************************************************************/
/******************   COMPILER SPECIFIC HEADERS AND DLL/STATIC IMPORT/EXPORTS    ********************/
/****************************************************************************************************/

#if defined(_WIN32)

    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <process.h>

    #define APAC_THRD_LOCAL     __declspec(thread)
    #define APAC_THRD_CALL      WINAPI
    typedef unsigned            apac_thrd_ret_t;
    typedef void*               apac_thrd_arg_t;          

    #if defined(_MSC_VER)

        #if defined(_M_X64) || defined(_M_AMD64)

            #include <immintrin.h>
            #include <intrin.h>
            #define APAC_X64_WIN

        #elif defined(_M_ARM64) || defined(_M_ARM64EC)

			// TODO
            #define APAC_ARM64_WIN

		#else
			#error "Unsupported Architecture on Windows and MSVC!"	
		#endif

    #else
        #error "Unknown Compiler on Windows!"
    #endif

    #if defined(BUILD_SHARED_LIB)
        // Export symbols when building the DLL
        #define APAC_API __declspec(dllexport)
    #elif defined(LIBAPAC_SHARED)
        // Import symbols when using the DLL
        #define APAC_API __declspec(dllimport)
    #else
        // Static library, no import/export needed
        #define APAC_API
    #endif

#elif defined(__linux__) || defined(__linux)    ||  \
      defined(__unix__)  || defined(__unix)     ||  \
      defined(__APPLE__) || defined(__MACH__)

    #if defined(__GNUC__) || defined(__clang__)

        #include <unistd.h>

        #if defined(_POSIX_THREADS) && (_POSIX_THREADS > 0)
            #include <pthread.h>
        #else
            #error "PThreads are needed for working with libapac!"
        #endif

        #define APAC_THRD_LOCAL __thread            /* GCC/Clang extension */
        #define APAC_THRD_CALL                      /* nothing */
        typedef void*           apac_thrd_ret_t;    
        typedef void*           apac_thrd_arg_t;

        #if defined(__x86_64)   || defined(__amd64)   || \
            defined(__x86_64__) || defined(__amd64__)

            #include <cpuid.h>
            #include <immintrin.h>
            #define APAC_X64_UNIX
        
        #elif defined(__aarch64__) || defined(__arm64__)

            #include <arm_acle.h>                
            #define APAC_ARM64_UNIX

		#else
			#error "Unsupported Architecture on Linux/Unix/MacOS and GCC/Clang!"
        #endif

    #else
        #error "Unknown Compiler on Linux/Unix/MacOS!"
    #endif

    #if defined(BUILD_SHARED_LIB)
        // Export symbols when building the shared object/dylib
        #define APAC_API __attribute__((visibility("default")))
    #else
        // Static library, no visibility attributes needed
        #define APAC_API
    #endif

#else

    #error "Unknown Platform!"

#endif

/****************************************************************************************************/
/*************************    PLATFORM SPECIFIC DEFINES AND TYPEDEFS    *****************************/
/****************************************************************************************************/

/* ==========================================================================
 * Typedefs and Related Macros
 * ========================================================================== */

#if (defined(APAC_X64_WIN)      ||  \
     defined(APAC_X64_UNIX)     ||  \
     defined(APAC_ARM64_WIN)    ||  \
     defined(APAC_ARM64_UNIX)       \
    )

    #define APAC_64BIT_PLATFORM

    typedef uint64_t                    ap_dig_t;
    typedef size_t                     ap_size_t;
    typedef struct apac_tpool_t     apac_tpool_t;
    
    #define PRI_APN_PTR         "p"
    #define PRI_APN_SIZE        "zu"
    #define APN_SIZE_MAX        SIZE_MAX
    #define APN_DIG_MAX         UINT64_MAX
    #define PRI_APN_DIGU        PRIu64
    #define PRI_APN_DIGX        PRIx64

    #define APN_DIG_BITS        64U
    #define APN_DIG_HIGH_BIT    (1ULL << 63)

    #define APN_POS             (1)
    #define APN_NEG             (-1)
    #define APN_ZERO            (0)

#else

    #error "Unknown Platform and CPU Architecture!"

#endif

/* ==========================================================================
 * Return Error Values Enum
 * ========================================================================== */

typedef enum apac_err
{
    APAC_OK = 0,

    // Memory
    APAC_OOM,

    // Arithmetic
    APAC_DIV_BY_ZERO,

    // Threading / Synchronization
    APAC_THRD_ERR

} apac_err;

typedef enum apac_str_base
{

    BASE10 = 10,
    BASE16 = 16

} apac_str_base;

/****************************************************************************************************/
/*********************************      ERROR HANDLING MACROS      **********************************/
/****************************************************************************************************/

/* ==========================================================================
 * Always-Enabled Assertion
 * ========================================================================== */

#define APAC_ALWAYS_ASSERT(expr)                                \
        do                                                      \
        {                                                       \
            if (!(expr))                                        \
            {                                                   \
                fprintf(                                        \
                    stderr,                                     \
                    "\n\nAPAC ASSERTION FAILED! \n"             \
                    "ASSERTION: %s              \n"             \
                    "FILE: %s\nLINE: %d         \n"             \
                    "ABORTING ...             \n\n",            \
                    #expr, __FILE__, __LINE__                   \
                );                                              \
                exit(EXIT_FAILURE);                             \
            }                                                   \
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
 * Utility Functions for Testing and Benchmarking
 * ========================================================================== */

uint64_t apac_cpu_timer(void);

uint64_t apac_os_timer(void);

int apac_pin_thread_to_core(uint32_t core_id);

void apac_disable_dfs(void);

void apac_restore_dfs(void);

/* ==========================================================================
 * Memory Allocation Functions and Global Allocator Struct
 * ========================================================================== */

typedef void* (*apac_malloc_t)(size_t new_size, void* ctx);
typedef void* (*apac_realloc_t)(void* old_arr, size_t new_size, void* ctx);
typedef void (*apac_free_t)(void* old_arr, void* ctx);

typedef struct apac_alloc_t apac_alloc_t;

APAC_API void apac_init_global_allocator(  
    apac_malloc_t malloc_ptr,
    apac_realloc_t realloc_ptr,
    apac_free_t free_ptr,
    void* ctx_ptr
);

APAC_API void* apac_malloc(size_t new_size);
APAC_API void* apac_realloc(void* old_arr, size_t new_size);
APAC_API void  apac_free(void* old_arr);

extern apac_alloc_t apac_allocator;

/* ==========================================================================
 * Runtime CPU Detection and Low-Level Function Dispatch Struct
 * ========================================================================== */

typedef struct apac_cpu_params
{
    ap_size_t karatsuba_mul_threshold;
    ap_size_t toomcook3_mul_threshold;

    ap_size_t karatsuba_sqr_threshold;
    ap_size_t toomcook3_sqr_threshold;

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

    ap_dig_t(*apn_lshift_add_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t(*apn_lshift_sub_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);

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
#define TOOMCOOK3_MUL_THRESHOLD (curr_cpu.toomcook3_mul_threshold)
#define KARATSUBA_SQR_THRESHOLD	(curr_cpu.karatsuba_sqr_threshold)
#define TOOMCOOK3_SQR_THRESHOLD (curr_cpu.toomcook3_sqr_threshold)
#define DNC_DIV_THRESHOLD	    (curr_cpu.dnc_div_threshold)

/* ==========================================================================
 * CPU detection and Default Initialization Functions 
 * ========================================================================== */

APAC_API void apac_get_cpu_spec(void);

APAC_API void apac_init(void);

/****************************************************************************************************/
/*********************************      THREAD-POOL FUNCTIONS     ***********************************/
/****************************************************************************************************/



/****************************************************************************************************/
/*********************************          APN FUNCTIONS         ***********************************/
/****************************************************************************************************/

APAC_API ap_dig_t apn_add_n(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size
);

APAC_API ap_dig_t apn_add_one(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size,
    ap_dig_t val
);

APAC_API ap_dig_t apn_add(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2
);

APAC_API ap_dig_t apn_sub_n(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size
);

APAC_API ap_dig_t apn_sub_one(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size,
    ap_dig_t val
);

APAC_API ap_dig_t apn_sub(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2
);

APAC_API void apn_cpy(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size
);

APAC_API void apn_neg(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size
);

APAC_API apac_err apn_mul_n(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size
);

APAC_API apac_err apn_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2
);

APAC_API ap_dig_t apn_addmul_one(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size,
    ap_dig_t val
);

APAC_API ap_dig_t apn_submul_one(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size,
    ap_dig_t val
);

APAC_API apac_err apn_sqr(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size
);

APAC_API void apn_set(
    ap_dig_t* result,
    ap_size_t size,
    ap_dig_t val
);

APAC_API int apn_cmp(
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size
);

APAC_API apac_err apn_div(
    ap_dig_t* quotient,
    ap_dig_t* remainder,
    const ap_dig_t* dividend,
    const ap_dig_t* divisor,
    ap_size_t size_divd,
    ap_size_t size_dvsr
);

APAC_API ap_dig_t apn_div_one(
    ap_dig_t* quotient,
    const ap_dig_t* dividend,
    ap_dig_t divisor,
    ap_size_t size_divd
);

APAC_API ap_dig_t apn_rshift(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size,
    ap_dig_t bit_cnt
);

APAC_API ap_dig_t apn_lshift(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size,
    ap_dig_t bit_cnt
);

APAC_API ap_dig_t apn_lshift_add(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size,
    ap_dig_t bit_cnt
);

APAC_API ap_dig_t apn_lshift_sub(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size,
    ap_dig_t bit_cnt
);

APAC_API int apn_is_zero(
    const ap_dig_t* op1,
    ap_size_t size
);

APAC_API ap_size_t apn_clamp(
    const ap_dig_t* op1,
    ap_size_t size
);

APAC_API void apn_seed_prng(
    ap_dig_t seed
);

APAC_API void apn_set_random(
    ap_dig_t* op1,
    ap_size_t size1
);

APAC_API void apn_print(
    FILE* fp,
    ap_dig_t* op1,
    ap_size_t size,
    apac_str_base base
);

#endif