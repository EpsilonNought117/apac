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

#if defined(__GNUC__)  || defined(__clang__)
	#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__)
		#include <immintrin.h>
		#include <cpuid.h>
	#endif
#else
	#error "Unknown compiler found."
#endif


// --- DLL/SO Export/Import ----------------------------------------------------

#if defined(_WIN32) || defined(_WIN64)
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
#else
	#if defined(BUILD_SHARED_LIB) || defined(LIBAPAC_SHARED)
		// On Unix-like systems, use visibility attributes for shared libraries
		#define APAC_API __attribute__((visibility("default")))
	#else
		#define APAC_API
	#endif
#endif


/****************************************************************************************************/
/*****************************      ERROR HANDLING FOR DEBUG MODE      ******************************/
/****************************************************************************************************/

#if !defined(APAC_DISABLE_ASSERT)	
	#ifndef APAC_REPORT_ERR
	#define APAC_REPORT_ERR(x) \
    	printf(stderr, "APAC ERROR %s:%d %s\n", __FILE__, __LINE__, x);
	#endif

	#ifndef APAC_ASSERT
	#define APAC_ASSERT(x)			\
			if (!(x))               \
			{                       \
				APAC_REPORT_ERR(#x) \
				abort();            \
			}
	#endif
#else
	#define APAC_REPORT_ERR(x) ((void)0)
	#define APAC_ASSERT(x)     ((void)0)
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

APAC_API void apacInit(void);

APAC_API void apacGetCPUSpec(void);

typedef uint64_t apn_seg;
typedef uint64_t apn_size;

/****************************************************************************************************/
/**********************************          CPU FUNCTIONS       ************************************/
/****************************************************************************************************/

typedef struct apac_cpu_params
{
	apn_size karatsuba_mul_balanced_threshold;
	apn_size karatsuba_mul_unbalanced_threshold;
	apn_size karatsuba_sqr_threshold;

	apn_seg(*apn_add_n_ptr)(apn_seg*, const apn_seg*, const apn_seg*, apn_size);
	apn_seg(*apn_sub_n_ptr)(apn_seg*, const apn_seg*, const apn_seg*, apn_size);
	void (*apn_mul_bc_ptr)(apn_seg*, const apn_seg*, const apn_seg*, apn_size, apn_size);
	apn_seg(*apn_addmul_one_ptr)(apn_seg*, const apn_seg*, apn_size, apn_seg);
	void (*apn_sqr_bc_ptr)(apn_seg*, const apn_seg*, apn_size);
	void (*apn_neg_ptr)(apn_seg*, const apn_seg*, apn_size);
	void (*apn_cpy_ptr)(apn_seg*, const apn_seg*, apn_size);
	void (*apn_set_ptr)(apn_seg*, apn_size, apn_seg);
	int (*apn_cmp_ptr)(const apn_seg*, const apn_seg*, apn_size);

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
* 3) THE RESULT MUST HAVE APPROPRIATE NUMBER OF LIMBS ACCORDING TO OPERATION.
*
* 4) THEY DO NOT PERFORM ANY BOUNDS CHECKING.
*
* 5) LITTLE TO NO WORK IS DONE APART FROM THE ACTUAL COMPUTATION NEEDED.
*
* 6) NO SIZE ARGUMENT SHOULD BE ZERO, ALWAYS PASS AT LEAST SIZE 1 FOR ALL SIZE ARGUMENTS.
*
*/

/*

1) result must have "size" limbs
2) returns the carry out

*/
APAC_API apn_seg apn_add_n(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size);

/*

1) size1 must be greater than or equal to size2.
2) result must have size1 limbs
3) returns the carry out

*/
APAC_API apn_seg apn_add(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size1, apn_size size2);

/*

1) result must have "size" limbs
2) returns the carry out (unlikely in avg case)

*/
APAC_API apn_seg apn_add_one(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);

/*

1) result must have "size" limbs
2) returns the borrow out

*/
APAC_API apn_seg apn_sub_n(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size);

/*

1) size1 must be greater than or equal to size2.
2) result must have size1 limbs
3) return the borrow out

*/
APAC_API apn_seg apn_sub(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size1, apn_size size2);

/*

1) result must have "size" limbs
2) returns the borrow out

*/
APAC_API apn_seg apn_sub_one(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);

/*

1) result must have "size" number of limbs

*/
APAC_API void apn_cpy(apn_seg* result, const apn_seg* op1, apn_size size);

/*

1) result must have "size" number of limbs
2) Performs 2's complement of op1 and stores in result
3) Overlap is allowed between result and op1

*/
APAC_API void apn_neg(apn_seg* result, const apn_seg* op1, apn_size size);

/*

1) No overlap permitted between result and either of the input operands
2) result must have at least 2 * size number of limbs
3) Input operands can overlap. If they are the same, use apn_sqr

*/
APAC_API void apn_mul_n(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size);

/*

1) No overlap permitted between result and either of the input operands
2) result must have at least (size1 + size2) number of limbs
3) size1 must be greater than or equal to size2
4) Input operands can overlap as they are read only

*/
APAC_API void apn_mul(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size1, apn_size size2);

/*

1) No overlap permitted between result and input operand (op1)
2) result must have at least (size + 1) number of limbs
3) op1 and val (single limb) can overlap

*/
APAC_API apn_seg apn_addmul_one(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);

/*

1) No overlap permitted between result and input operand
2) result must have (2 * size) number of limbs

*/
APAC_API void apn_sqr(apn_seg* result, const apn_seg* op1, apn_size size);

/*

1) Result must have size number of limbs

*/
APAC_API void apn_set(apn_seg* result, apn_size size, apn_seg val);

/*

1) returns (int) 1, 0, -1 based on whether op1 > op2, op1 = op2 or op1 < op2

*/
APAC_API int apn_cmp(const apn_seg* op1, const apn_seg* op2, apn_size size);

#endif