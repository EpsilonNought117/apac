#ifndef APAC_H
#define APAC_H

/****************************************************************************************************/
/********************************      REQUIRED STANDARD HEADERS      *******************************/
/****************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/****************************************************************************************************/
/******************   COMPILER SPECIFIC HEADERS AND DLL/STATIC IMPORT/EXPORTS    ********************/
/****************************************************************************************************/

#if defined(_WIN32)

    #if defined(_MSC_VER)

        #include <Windows.h> // for win32 threads

        #if defined(_M_X64) || defined(_M_AMD64)
            #include <immintrin.h>
            #include <intrin.h>
        #endif   

    #else
        #error "Unknown compiler on Windows!"
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

#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)

    #include  <pthread.h>

    #if defined(__x86_64) || defined(__amd64) || defined(__x86_64__) || defined(__amd64__)
        #include <x86intrin.h>
        #include <cpuid.h>
        #include <immintrin.h>
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

typedef enum
{
    APAC_OK,
    APAC_OOM,
    APAC_DIV_BY_ZERO

}   apac_err;

#ifndef APAC_ALWAYS_ASSERT
#define APAC_ALWAYS_ASSERT(x)                                                               \
    do                                                                                      \
    {                                                                                       \
        if (!(x))                                                                           \
        {                                                                                   \
            fprintf(stderr, "ASSERTION FAILED!");                                           \
            fprintf(stderr, "ASSERTION: %s\nFILE: %s\nLINE: %d\n", #x, __FILE__, __LINE__); \
            fprintf(stderr, "ABORTING ...");                                                \
            abort();                                                                        \
        }                                                                                   \
    } while (0)
#endif

#if !defined(APAC_DISABLE_ASSERT)
    #ifndef APAC_ASSERT
    #define APAC_ASSERT(x) APAC_ALWAYS_ASSERT(x)
    #endif
#else
    #define APAC_ASSERT(x)      
#endif

#define APAC_REPORT_ERR(x)  fprintf(stderr, "APAC ERROR: %s\n", #x); 

/****************************************************************************************************/
/*********************************         MISCELLANEOUS          ***********************************/
/****************************************************************************************************/

APAC_API extern void* (*apac_malloc)(size_t);
APAC_API extern void* (*apac_realloc)(void*, size_t);
APAC_API extern void (*apac_free)(void*);

APAC_API void apacSetMemFuncs(
	void* (*ptr1)(size_t),
	void* (*ptr2)(void*, size_t),
	void (*ptr3)(void*)
);

APAC_API void apacGetCPUSpec(void);

typedef uint64_t apn_seg_t;
typedef uint64_t apn_size_t;

typedef struct apac_cpu_params
{
	apn_size_t karatsuba_mul_balanced_threshold;
	apn_size_t karatsuba_mul_unbalanced_threshold;
	apn_size_t karatsuba_sqr_threshold;

	apn_seg_t(*apn_add_n_ptr)(apn_seg_t*, const apn_seg_t*, const apn_seg_t*, apn_size_t);
	apn_seg_t(*apn_sub_n_ptr)(apn_seg_t*, const apn_seg_t*, const apn_seg_t*, apn_size_t);
    apn_seg_t(*apn_add_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);
    apn_seg_t(*apn_sub_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);

    apn_seg_t(*apn_addmul_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);
	void (*apn_mul_bc_ptr)(apn_seg_t*, const apn_seg_t*, const apn_seg_t*, apn_size_t, apn_size_t);
	void (*apn_sqr_bc_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t);
	
    void (*apn_neg_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t);
	void (*apn_cpy_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t);
    void (*apn_set_ptr)(apn_seg_t*, apn_size_t, apn_seg_t);

}   apac_cpu_params;

/****************************************************************************************************/
/*********************************          APN FUNCTIONS         ***********************************/
/****************************************************************************************************/

/**
* -------------------- IMPORTANT NOTES --------------------
*
* 1) THESE FUNCTIONS DO NOT PERFORM ANY MEMORY ALLOCATIONS IN OPERANDS OR RESULT.
*
* 2) ASSERTS ARE USED IN DEBUG MODE FOR CATCHING INVALID ARGUMENT ERRORS.
* 
* 3) THEY DO NOT PERFORM ANY BOUNDS CHECKING.
* 
* 4) THE POINTER INPUTS SHOULD NOT BE NULL.
* 
* 5) THE SIZE INPUTS SHOULD NOT BE ZERO.
* 
* 6) IN CASES OF TWO INPUT SIZES, SIZE-1 IS ALWAYS GREATER THAN SIZE-2
*
*/

APAC_API apn_seg_t apn_add_n(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

APAC_API apn_seg_t apn_add(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

APAC_API apn_seg_t apn_add_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

APAC_API apn_seg_t apn_sub_n(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

APAC_API apn_seg_t apn_sub(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

APAC_API apn_seg_t apn_sub_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

APAC_API void apn_cpy(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size
);

APAC_API void apn_neg(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size
);

APAC_API void apn_mul_n(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

APAC_API void apn_mul(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

APAC_API apn_seg_t apn_addmul_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

APAC_API void apn_sqr(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size
);

APAC_API void apn_set(
    apn_seg_t* result,
    apn_size_t size,
    apn_seg_t val
);

APAC_API int apn_cmp(
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

#endif