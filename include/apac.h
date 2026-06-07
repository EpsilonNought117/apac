#ifndef APAC_H
#define APAC_H

#if defined(__GNUC__)
    #define _GNU_SOURCE
#endif

#include <stddef.h> // for size_t

/****************************************************************************************************/
/******************   COMPILER SPECIFIC HEADERS AND DLL/STATIC IMPORT/EXPORTS    ********************/
/****************************************************************************************************/

#if defined(_WIN32)

    #if defined(_MSC_VER)

        #if defined(_M_X64) || defined(_M_AMD64)

            #define APAC_X64_WIN

        #elif defined(_M_ARM64) || defined(_M_ARM64EC)

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

        #if defined(__x86_64)   || defined(__amd64)   || \
            defined(__x86_64__) || defined(__amd64__)

            #define APAC_X64_UNIX
        
        #elif defined(__aarch64__) || defined(__arm64__)
                      
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
/**********************************    MACROS AND TYPEDEFS    ***************************************/
/****************************************************************************************************/

/* ==========================================================================
 * Typedefs
 * ========================================================================== */

#if (defined(APAC_X64_WIN)      ||  \
     defined(APAC_X64_UNIX)     ||  \
     defined(APAC_ARM64_WIN)    ||  \
     defined(APAC_ARM64_UNIX)       \
    )

    typedef unsigned long long  ap_dig_t;
    typedef unsigned long long  ap_size_t;

#else
    #error "Unknown Platform and CPU Architecture!"
#endif

/* ==========================================================================
 * Relevant Enums
 * ========================================================================== */

typedef enum apac_err
{
    APAC_OK = 0,

    APAC_OOM,

    APAC_DIV_BY_ZERO,

    APAC_THRD_CREATE_FAIL

} apac_err;

typedef enum apac_str_base
{

    BASE10 = 10,
    BASE16 = 16

} apac_str_base;

typedef enum apac_sched_t
{
    APAC_SCHED_STATIC,
    APAC_SCHED_DYNAMIC

} apac_sched_t;

/****************************************************************************************************/
/*********************************    MISCELLANEOUS FUNCTIONS     ***********************************/
/****************************************************************************************************/

// ============================================================================
// Parallel-For Related Functions and Definitions
// ============================================================================

typedef void (*apac_pfor_func_t)(
    ap_size_t begin,
    ap_size_t end,
    void* arg
);

apac_err
apac_pfor_init(ap_size_t thrd_count);

apac_err
apac_pfor_destroy(void);

size_t
apac_pfor_get_size(void);

apac_err
apac_pfor_do(
    ap_size_t begin,
    ap_size_t end,
    ap_size_t chunk_size,
    apac_sched_t sched,
    apac_pfor_func_t func,
    void* arg
);

/* ==========================================================================
 * Memory Allocation Functions and Global Allocator Struct
 * ========================================================================== */

typedef void* (*apac_malloc_t)(size_t new_size, void* ctx);
typedef void* (*apac_realloc_t)(void* old_arr, size_t new_size, void* ctx);
typedef void (*apac_free_t)(void* old_arr, void* ctx);

APAC_API void apac_init_allocator(  
    apac_malloc_t malloc_ptr,
    apac_realloc_t realloc_ptr,
    apac_free_t free_ptr,
    void* ctx_ptr
);

APAC_API void* apac_malloc(size_t new_size);
APAC_API void* apac_realloc(void* old_arr, size_t new_size);
APAC_API void  apac_free(void* old_arr);

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
    ap_size_t size_divd_frac,
    ap_size_t size_dvsr
);

APAC_API ap_dig_t apn_div_one(
    ap_dig_t* quotient,
    const ap_dig_t* dividend,
    ap_dig_t divisor,
    ap_size_t size_divd,
    ap_size_t size_divd_frac
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

APAC_API void apn_seed_prng(
    ap_dig_t seed
);

APAC_API void apn_set_random(
    ap_dig_t* op1,
    ap_size_t size1
);

#endif