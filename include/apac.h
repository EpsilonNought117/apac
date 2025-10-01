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
        
        #if defined(_M_X64) || defined(_M_AMD64)
            #include <immintrin.h>
            #include <intrin.h>
        #endif   
        
        #if (_MSC_VER >= 1935)
            #include <threads.h>
        #else
            #error "Minimum MSVC v19.35 needed to compile multi-threaded routines on Windows!"
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

    #if defined(__x86_64) || defined(__amd64) || defined(__x86_64__) || defined(__amd64__)
        #include <x86intrin.h>
        #include <cpuid.h>
        #include <immintrin.h>
    #endif

    #include <threads.h>

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

#if !defined(APAC_DISABLE_ASSERT)	
	#ifndef APAC_REPORT_ERR
	#define APAC_REPORT_ERR(x) \
    	fprintf(stderr, "APAC ERROR %s:%d %s\n", __FILE__, __LINE__, x);
	#endif

	#ifndef APAC_ASSERT
	#define APAC_ASSERT(x)              \
        do                              \
        {                               \
            if (!(x))                   \
            {                           \
                APAC_REPORT_ERR(#x)     \
                abort(-1);              \
            }                           \
        } while (0)
	#endif
#else
	#define APAC_REPORT_ERR(x)  
	#define APAC_ASSERT(x)      
#endif

#ifndef APAC_ALWAYS_ASSERT
#define APAC_ALWAYS_ASSERT(x)                                                           \
    do                                                                                  \
    {                                                                                   \
        if (!(x))                                                                       \
        {                                                                               \
            fprintf(stderr, "APAC ERROR %s:%d %s\n", __FILE__, __LINE__, #x);           \
            abort(-1);                                                                  \
        }                                                                               \
    } while (0)
#endif

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

typedef uint64_t apn_seg;
typedef uint64_t apn_size;

typedef struct apac_cpu_params
{
	apn_size karatsuba_mul_balanced_threshold;
	apn_size karatsuba_mul_unbalanced_threshold;
	apn_size karatsuba_sqr_threshold;

	apn_seg(*apn_add_n_ptr)(apn_seg*, const apn_seg*, const apn_seg*, apn_size);
	apn_seg(*apn_sub_n_ptr)(apn_seg*, const apn_seg*, const apn_seg*, apn_size);
    apn_seg(*apn_add_one_ptr)(apn_seg*, const apn_seg*, apn_size, apn_seg);
    apn_seg(*apn_sub_one_ptr)(apn_seg*, const apn_seg*, apn_size, apn_seg);

    apn_seg(*apn_addmul_one_ptr)(apn_seg*, const apn_seg*, apn_size, apn_seg);
	void (*apn_mul_bc_ptr)(apn_seg*, const apn_seg*, const apn_seg*, apn_size, apn_size);
	void (*apn_sqr_bc_ptr)(apn_seg*, const apn_seg*, apn_size);
	
    void (*apn_neg_ptr)(apn_seg*, const apn_seg*, apn_size);
	void (*apn_cpy_ptr)(apn_seg*, const apn_seg*, apn_size);
    void (*apn_set_ptr)(apn_seg*, apn_size, apn_seg);

}   apac_cpu_params;

extern apac_cpu_params curr_cpu;

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

APAC_API apn_seg apn_add_n(
    apn_seg* result,
    const apn_seg* op1,
    const apn_seg* op2,
    apn_size size
);

APAC_API apn_seg apn_add(
    apn_seg* result,
    const apn_seg* op1,
    const apn_seg* op2,
    apn_size size1,
    apn_size size2
);

APAC_API apn_seg apn_add_one(
    apn_seg* result,
    const apn_seg* op1,
    apn_size size,
    apn_seg val
);

APAC_API apn_seg apn_sub_n(
    apn_seg* result,
    const apn_seg* op1,
    const apn_seg* op2,
    apn_size size
);

APAC_API apn_seg apn_sub(
    apn_seg* result,
    const apn_seg* op1,
    const apn_seg* op2,
    apn_size size1,
    apn_size size2
);

APAC_API apn_seg apn_sub_one(
    apn_seg* result,
    const apn_seg* op1,
    apn_size size,
    apn_seg val
);

APAC_API void apn_cpy(
    apn_seg* result,
    const apn_seg* op1,
    apn_size size
);

APAC_API void apn_neg(
    apn_seg* result,
    const apn_seg* op1,
    apn_size size
);

APAC_API void apn_mul_n(
    apn_seg* result,
    const apn_seg* op1,
    const apn_seg* op2,
    apn_size size
);

APAC_API void apn_mul(
    apn_seg* result,
    const apn_seg* op1,
    const apn_seg* op2,
    apn_size size1,
    apn_size size2
);

APAC_API apn_seg apn_addmul_one(
    apn_seg* result,
    const apn_seg* op1,
    apn_size size,
    apn_seg val
);

APAC_API void apn_sqr(
    apn_seg* result,
    const apn_seg* op1,
    apn_size size
);

APAC_API void apn_set(
    apn_seg* result,
    apn_size size,
    apn_seg val
);

APAC_API int apn_cmp(
    const apn_seg* op1,
    const apn_seg* op2,
    apn_size size
);

#endif