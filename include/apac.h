#ifndef APAC_H
#define APAC_H

/* ---------------------------------------------------------------------------------------------- */
/*                                          BECAUSE GNU (TvT)                                     */
/* ---------------------------------------------------------------------------------------------- */

#if defined(__GNUC__)
    #define _GNU_SOURCE
#endif

/* ---------------------------------------------------------------------------------------------- */
/*                             PLATFORM SPECIFIC MACROS AND COMMON HEADER                         */
/* ---------------------------------------------------------------------------------------------- */

#include <stddef.h> // for size_t

#if defined(_WIN32)

    #if defined(MINGW)

        #error "MinGW Toolchain compilers are not supported. Please use MSVC, Clang-cl or Clang instead."

    #endif

    #if defined(_MSC_VER)

        #if defined(_M_X64) || defined(_M_AMD64)

            #define APAC_WIN_X64        1

        #elif defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__) || defined(__arm64__)

            #define APAC_WIN_ARM64      1

        #else

            #error "Unsupported CPU Architecture on Windows."

        #endif

    #else

        #error "Unsupported Compiler on Windows. Please use MSVC, Clang-cl or Clang instead."

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

#elif defined(__APPLE__) && defined(__MACH__)

    #if defined(__apple_build_version__)

        #if defined(__x86_64) || defined(__amd64) || \
            defined(__x86_64__) || defined(__amd64__)

            #define APAC_MACOS_X64      1

        #elif defined(__aarch64__) || defined(__arm64__)

            #define APAC_MACOS_ARM64    1

        #else

            #error "Unsupported CPU Architecture on macOS."

        #endif

    #else

        #error "Unsupported Compiler on macOS. Only Apple Clang is supported."

    #endif

    #if defined(BUILD_SHARED_LIB)
        // Export symbols when building the dylib
        #define APAC_API __attribute__((visibility("default")))
    #else
        // Static library, no visibility attributes needed
        #define APAC_API
    #endif

#elif defined(__linux__) || defined(__linux)

    #if defined(__GNUC__) || defined(__clang__)

        #if defined(__x86_64)   || defined(__amd64)     || \
            defined(__x86_64__) || defined(__amd64__)

            #define APAC_LINUX_X64      1

        #elif defined(__aarch64__) || defined(__arm64__)

            #define APAC_LINUX_ARM64    1

        #else

            #error "Unsupported CPU Architecture on Linux."

        #endif
        
    #else

        #error "Unsupported Compiler on Linux. Please use GCC or Clang instead."

    
    #endif

    #if defined(BUILD_SHARED_LIB)
        // Export symbols when building the shared object
        #define APAC_API __attribute__((visibility("default")))
    #else
        // Static library, no visibility attributes needed
        #define APAC_API
    #endif

#else

    #error "Unsupported Platform. This project supports Windows, macOS and Linux only."

#endif

/* ---------------------------------------------------------------------------------------------- */
/*                                        TYPEDEFS AND ENUMS                                      */
/* ---------------------------------------------------------------------------------------------- */

#if defined(APAC_WIN_X64)       ||  \
    defined(APAC_LINUX_X64)     ||  \
    defined(APAC_MACOS_X64)     ||  \
    defined(APAC_WIN_ARM64)     ||  \
    defined(APAC_LINUX_ARM64)   ||  \
    defined(APAC_MACOS_ARM64)
    
    typedef unsigned long long  apn_dig_t;      // digit type for arbitrary precision natural numbers
    typedef unsigned long long  apn_size_t;     // count of digits in a arbitrary precision natural number

#else

    #error "Unknown Platform and CPU Architecture!"

#endif

/* Signature for a custom allocator's `malloc` function. */
typedef void* (*apac_malloc_t)(void* ctx, size_t new_size);

/* Signature for a custom allocator's `realloc` function. */
typedef void* (*apac_realloc_t)(void* ctx, void* old_buffer, size_t new_size);

/* Signature for a custom allocator's `free` function. */
typedef void (*apac_free_t)(void* ctx, void* old_buffer);

typedef enum apac_err_t
{
    APAC_OK = 0,        // all ok
    APAC_OOM,           // ran out of memory
    APAC_DIV_BY_ZERO,   // tried to divide by zero

} apac_err_t;

typedef enum apac_num_base_t
{
    BASE10 = 10,        // Decimal
    BASE16 = 16         // Hexadecimal

} apac_num_base_t;

/* -------------------------------------------------------------------------------------------------- */
/*                                        MISCELLANEOUS FUNCTIONS                                     */
/* -------------------------------------------------------------------------------------------------- */

APAC_API void*
apac_malloc(size_t new_size); // wrapper for convenience

APAC_API void*
apac_realloc(void* old_buffer, size_t new_size); // wrapper for convenience

APAC_API void
apac_free(void* old_buffer);   // wrapper for convenience

APAC_API void
apac_init_allocators(
    apac_malloc_t    custom_malloc,
    apac_realloc_t  custom_realloc,
    apac_free_t        custom_free,
    void*                  context
);

APAC_API void 
apac_get_cpu_spec(void);

/****************************************************************************************************/
/*********************************          APN FUNCTIONS         ***********************************/
/****************************************************************************************************/

APAC_API apn_dig_t 
apn_add_n(
    apn_dig_t* result,
    const apn_dig_t* op1,
    const apn_dig_t* op2,
    apn_size_t size
);

APAC_API apn_dig_t 
apn_add_one(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size,
    apn_dig_t val
);

APAC_API apn_dig_t 
apn_add(
    apn_dig_t* result,
    const apn_dig_t* op1,
    const apn_dig_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

APAC_API apn_dig_t 
apn_sub_n(
    apn_dig_t* result,
    const apn_dig_t* op1,
    const apn_dig_t* op2,
    apn_size_t size
);

APAC_API apn_dig_t 
apn_sub_one(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size,
    apn_dig_t val
);

APAC_API apn_dig_t 
apn_sub(
    apn_dig_t* result,
    const apn_dig_t* op1,
    const apn_dig_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

APAC_API void 
apn_cpy(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size
);

APAC_API void 
apn_neg(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size
);

APAC_API apac_err_t 
apn_mul_n(
    apn_dig_t* result,
    const apn_dig_t* op1,
    const apn_dig_t* op2,
    apn_size_t size
);

APAC_API apac_err_t 
apn_mul(
    apn_dig_t* result,
    const apn_dig_t* op1,
    const apn_dig_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

APAC_API void
apn_mul_one(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size,
    apn_dig_t val
);

APAC_API apn_dig_t 
apn_addmul_one(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size,
    apn_dig_t val
);

APAC_API apn_dig_t 
apn_submul_one(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size,
    apn_dig_t val
);

APAC_API apac_err_t 
apn_sqr(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size
);

APAC_API void 
apn_set(
    apn_dig_t* result,
    apn_size_t size,
    apn_dig_t val
);

APAC_API int 
apn_cmp(
    const apn_dig_t* op1,
    const apn_dig_t* op2,
    apn_size_t size
);

APAC_API apac_err_t 
apn_div(
    apn_dig_t* quotient,
    apn_dig_t* remainder,
    const apn_dig_t* dividend,
    const apn_dig_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_divd_frac,
    apn_size_t size_dvsr
);

APAC_API apn_dig_t 
apn_div_one(
    apn_dig_t* quotient,
    const apn_dig_t* dividend,
    apn_dig_t divisor,
    apn_size_t size_divd,
    apn_size_t size_divd_frac
);

APAC_API apn_dig_t 
apn_rshift(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size,
    apn_dig_t bit_cnt
);

APAC_API apn_dig_t 
apn_lshift(
    apn_dig_t* result,
    const apn_dig_t* op1,
    apn_size_t size,
    apn_dig_t bit_cnt
);

APAC_API int 
apn_is_zero(
    const apn_dig_t* op1,
    apn_size_t size
);

APAC_API apn_size_t 
apn_clamp(
    const apn_dig_t* op1,
    apn_size_t size
);

APAC_API void 
apn_seed_prng(
    apn_dig_t seed
);

APAC_API void 
apn_set_random(
    apn_dig_t* op1,
    apn_size_t size1
);

#endif