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

    #define APAC_THREAD_LOCAL __declspec(thread)

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
        
        #define APAC_THREAD_LOCAL __thread

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

    typedef uint64_t            ap_dig_t;
    typedef size_t              ap_size_t;
    typedef int64_t             ap_sign_t;

    #define PRI_APN_PTR         "p"
    #define PRI_APN_SIZE        "zu"
    #define APN_SIZE_MAX        SIZE_MAX
    #define APN_DIG_MAX         UINT64_MAX
    #define PRI_APN_DIGU        PRIu64
    #define PRI_APN_DIGX        PRIx64

    #define APN_DIG_BITS        64U
    #define APN_DIG_HIGH_BIT    (1ULL << 63)

    #define APZ_POS         ((ap_sign_t)1)
    #define APZ_NEG         ((ap_sign_t)-1)
    #define APZ_ZERO        ((ap_sign_t)0)

    #define APZ_MAX_SIZE    ((ap_size_t)1 << 52)

#else

    #error "Unknown Platform and CPU Architecture!"

#endif

/* ==========================================================================
 * Return Error Values Enum
 * ========================================================================== */

typedef enum apac_err
{
    APAC_OK,
    APAC_OOM,
    APAC_DIV_BY_ZERO

} apac_err;

/****************************************************************************************************/
/*********************************         MISCELLANEOUS          ***********************************/
/****************************************************************************************************/

/* ==========================================================================
 * Memory Allocation Function Pointers' Typedefs
 * ========================================================================== */

typedef void* (*apac_malloc_t)(size_t new_size, void* ctx);
typedef void* (*apac_realloc_t)(void* old_arr, size_t new_size, void* ctx);
typedef void (*apac_free_t)(void* old_arr, void* ctx);

typedef struct apac_alloc_t
{
    apac_malloc_t custom_malloc;
    apac_realloc_t custom_realloc;
    apac_free_t custom_free;
    void* ctx;

} apac_alloc_t;

APAC_API void apac_init_allocator(  
    apac_malloc_t malloc_ptr,
    apac_realloc_t realloc_ptr,
    apac_free_t free_ptr,
    void* ctx_ptr
);

/* ==========================================================================
 * CPU detection and Default Initialization Functions 
 * ========================================================================== */

APAC_API void apac_get_cpu_spec(void);

APAC_API void apac_init(void);

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

APAC_API void apn_set_to_random(
    ap_dig_t* op1,
    ap_size_t size1,
    ap_dig_t seed_val
);

/****************************************************************************************************/
/*********************************          APZ FUNCTIONS         ***********************************/
/****************************************************************************************************/

typedef struct apz_t
{
    ap_dig_t* data;
    ap_size_t max, used;
    ap_sign_t is_neg;

} apz_t;

typedef enum apac_str_base
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