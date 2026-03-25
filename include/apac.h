#ifndef APAC_H
#define APAC_H

/****************************************************************************************************/
/***************************      REQUIRED C STANDARD LIBRARY HEADERS      **************************/
/****************************************************************************************************/

#if defined(__GNUC__)
    #define _GNU_SOURCE
#endif

#include <stdio.h>
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
        #include <time.h>
        #include <sched.h>
        #include <pthread.h>

        #if defined(__x86_64)   || defined(__amd64)   || \
            defined(__x86_64__) || defined(__amd64__)

            #include <x86intrin.h>
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
/*********************************      ERROR HANDLING MACROS      **********************************/
/****************************************************************************************************/


#if (defined(APAC_X64_WIN)      ||  \
     defined(APAC_X64_UNIX)     ||  \
     defined(APAC_ARM64_WIN)    ||  \
     defined(APAC_ARM64_UNIX)       \
    )

    typedef uint64_t            ap_dig_t;
    typedef size_t              ap_size_t;

    #define PRI_APN_PTR         "p"
    #define PRI_APN_SIZE        "zu"
    #define APN_SIZE_MAX        SIZE_MAX
    #define APN_DIG_MAX         UINT64_MAX
    #define PRI_APN_DIGU        PRIu64
    #define PRI_APN_DIGX        PRIx64

    #define APN_DIG_BITS        64U
    #define APN_DIG_HIGH_BIT    (1ULL << 63)

#else
    #error "Unknown Platform and CPU Architecture!"
#endif

/****************************************************************************************************/
/*********************************      ERROR HANDLING MACROS      **********************************/
/****************************************************************************************************/

/* ==========================================================================
 * Error codes
 * ========================================================================== */

typedef enum
{
    APAC_OK,
    APAC_OOM,
    APAC_DIV_BY_ZERO

} apac_err;


/* ==========================================================================
 * Always-enabled assertion
 * ========================================================================== */

#define APAC_ALWAYS_ASSERT(expr)                            \
    do                                                      \
    {                                                       \
        if (!(expr))                                        \
        {                                                   \
            fprintf(                                        \
                stderr,                                     \
                "\n\nAPAC ASSERTION FAILED!\n"              \
                "ASSERTION: %s\n"                           \
                "FILE: %s\nLINE: %d\n",                     \
                #expr, __FILE__, __LINE__                   \
            );                                              \
            fprintf(stderr, "EXITING ...\n\n");             \
            exit(EXIT_FAILURE);                             \
        }                                                   \
    } while (0)


/* ==========================================================================
 * Debug-only assertion
 * ========================================================================== */

#ifndef APAC_DISABLE_ASSERT
    #define APAC_ASSERT(expr) APAC_ALWAYS_ASSERT(expr)
#else
    #define APAC_ASSERT(expr)   do { /* nothing */ } while (0)
#endif

/* ==========================================================================
 * Debug-only overlap checks
 * ========================================================================== */

/*
    These checks are only useful on platforms with flat address space per process.
*/
#if !defined(APAC_DISABLE_ASSERT)   &&  \
    (defined(APAC_X64_WIN)          ||  \
     defined(APAC_X64_UNIX)         ||  \
     defined(APAC_ARM64_WIN)        ||  \
     defined(APAC_ARM64_UNIX)           \
    )

    #define APAC_NO_OVERLAP(op1, size1, op2, size2)                 \
            APAC_ALWAYS_ASSERT(                                     \
                ((uintptr_t)(op1 + size1) <= (uintptr_t)(op2)) ||   \
                ((uintptr_t)(op2 + size2) <= (uintptr_t)(op1))      \
            )

    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)              \
            APAC_ALWAYS_ASSERT(                                             \
                ((uintptr_t)(op1 + size1) <= (uintptr_t)(op2 + size2)) ||   \
                ((uintptr_t)(op2 + size2) <= (uintptr_t)(op1))              \
            )

    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)              \
            APAC_ALWAYS_ASSERT(                                             \
                ((uintptr_t)(op2 + size2) <= (uintptr_t)(op1 + size1)) ||   \
                ((uintptr_t)(op1 + size1) <= (uintptr_t)(op2))              \
            )

#else

    #define APAC_NO_OVERLAP(op1, size1, op2, size2)             do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)  do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)  do { /* nothing */ } while (0)

#endif

/* ==========================================================================
 * Error logging
 * ========================================================================== */

#define APAC_LOG_ERR(msg) fprintf(stderr, "APAC ERROR: %s\n", msg)

/****************************************************************************************************/
/*********************************         MISCELLANEOUS          ***********************************/
/****************************************************************************************************/

/* ==========================================================================
 * Memory management hooks
 * ========================================================================== */

APAC_API extern void* (*apac_malloc)(size_t);
APAC_API extern void* (*apac_realloc)(void*, size_t);
APAC_API extern void (*apac_free)(void*);

APAC_API void apac_set_mem_funcs(
    void* (*ptr1)(size_t),
    void* (*ptr2)(void*, size_t),
    void (*ptr3)(void*)
);

/* ==========================================================================
 * CPU detection and initialization
 * ========================================================================== */

APAC_API void apac_get_cpu_spec(void);
APAC_API void apac_init(void);

/* ==========================================================================
 * CPU dispatch parameters
 * ========================================================================== */

typedef struct apac_cpu_params
{
    ap_size_t karatsuba_mul_balanced_threshold;
    ap_size_t karatsuba_mul_unbalanced_threshold;
    ap_size_t karatsuba_sqr_threshold;
    ap_size_t dnc_div_threshold;

    ap_dig_t (*apn_add_n_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t);
    ap_dig_t (*apn_sub_n_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t);
    ap_dig_t (*apn_add_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t (*apn_sub_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    void (*apn_neg_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t);

    ap_dig_t (*apn_addmul_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t (*apn_submul_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);

    ap_dig_t (*apn_lshift_lt64_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t (*apn_rshift_lt64_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);

    void (*apn_mul_bc_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t, ap_size_t);
    void (*apn_sqr_bc_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t);

    void (*apn_cpy_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t);
    void (*apn_set_ptr)(ap_dig_t*, ap_size_t, ap_dig_t);

    int (*apn_cmp_ptr)(const ap_dig_t*, const ap_dig_t*, ap_size_t);
    int (*apn_is_zero_ptr)(const ap_dig_t*, ap_size_t);

} apac_cpu_params;

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

APAC_API int apn_is_zero(
    const ap_dig_t* op1,
    ap_size_t size
);

APAC_API ap_size_t apn_clamp(
    const ap_dig_t* op1,
    ap_size_t size
);

/****************************************************************************************************/
/*********************************          APZ FUNCTIONS         ***********************************/
/****************************************************************************************************/

#define APZ_POS         ((int8_t)1)
#define APZ_NEG         ((int8_t)-1)
#define APZ_ZERO        ((int8_t)0)

#define APZ_MAX_SIZE    ((ap_size_t)1 << 52)

typedef struct
{
    ap_dig_t* data;
    ap_size_t max, used;
    int8_t is_neg;

} apz_t;

typedef enum
{

    BASE10 = 10,
    BASE16 = 16

} apac_str_base;

APAC_API apac_err apz_init(
    apz_t* op,
    ap_size_t size
);

APAC_API apac_err apz_free(
    apz_t* op
);

APAC_API apac_err apz_resize(
    apz_t* op,
    ap_size_t new_size
);

APAC_API apac_err apz_set(
    apz_t* op,
    ap_dig_t val
);

#endif