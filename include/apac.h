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
/***********************    COMPILER SPECIFIC HEADERS AND IMPORT/EXPORTS     ************************/
/****************************************************************************************************/

#if defined(_M_X64) || defined(_M_AMD64)
	#include <intrin.h>
#endif

#if defined(BUILD_SHARED_LIB)
	// Export symbols when building the DLL
	#define APAC_API __declspec(dllexport)
#elif defined(LIBAPAC_DLL)
	// Import symbols when using the DLL
	#define APAC_API __declspec(dllimport)
#else
	// Static library, no import/export needed
	#define APAC_API
#endif

/****************************************************************************************************/
/*****************************      ERROR HANDLING FOR DEBUG MODE      ******************************/
/****************************************************************************************************/

typedef enum apac_err
{
	APAC_OK,
	APAC_OOM,
	APAC_MATH_ERR

}   apac_err;

#if !defined(APAC_DISABLE_ASSERT)

#ifndef APAC_REPORT_ERR
#define APAC_REPORT_ERR(x) \
        fprintf(stderr, "APAC ERROR %s:%d %s\n", __FILE__, __LINE__, x);
#endif

#ifndef APAC_ASSERT
#define APAC_ASSERT(x)      \
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

typedef uint64_t  u64;
typedef uint32_t  u32;
typedef uint8_t    u8;
typedef int64_t   i64;
typedef int32_t   i32;
typedef int8_t     i8;

/****************************************************************************************************/
/**********************************          CPU FUNCTIONS       ************************************/
/****************************************************************************************************/

typedef struct apac_cpu_params
{
	u64 karatsuba_mul_n_threshold;
	u64 karatsuba_mul_threshold;
	
	u8(*apn_add_n_ptr)(u64*, const u64*, const u64*, u64);
	u8(*apn_sub_n_ptr)(u64*, const u64*, const u64*, u64);
	void (*apn_mul_bc_ptr)(u64*, const u64*, const u64*, u64, u64);
	void (*apn_neg_ptr)(u64*, const u64*, u64);
	void (*apn_cpy_ptr)(u64*, const u64*, u64);
	void (*apn_set_ptr)(u64*, u64, u64);

}   apac_cpu_params;

extern apac_cpu_params curr_cpu;

/****************************************************************************************************/
/*********************************          APN FUNCTIONS         ***********************************/
/****************************************************************************************************/

/**
*                   IMPORTANT NOTES
*
* 1) THESE FUNCTIONS DO NOT PERFORM ANY MEMORY ALLOCATIONS.
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
APAC_API u8 apn_add_n(u64* result, const u64* op1, const u64* op2, u64 size);

/*
	1) size1 must be greater than or equal to size2.
	2) result must have size1 limbs
	3) returns the carry out
*/
APAC_API u8 apn_add(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/*
	1) result must have "size" limbs
	2) returns the carry out (unlikely in avg case)
*/
APAC_API u8 apn_add_one(u64* result, const u64* op1, u64 size, u64 val);

/*
	1) result must have "size" limbs
	2) returns the borrow out
*/
APAC_API u8 apn_sub_n(u64* result, const u64* op1, const u64* op2, u64 size);

/*
	1) size1 must be greater than or equal to size2.
	2) result must have size1 limbs
	3) return the borrow out
*/
APAC_API u8 apn_sub(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/*
	1) result must have "size" limbs
	2) returns the borrow out
*/
APAC_API u8 apn_sub_one(u64* result, const u64* op1, u64 size, u64 val);

/*
	1) result must have "size" number of limbs
*/
APAC_API void apn_cpy(u64* result, const u64* op1, u64 size);

/*
	1) result must have "size" number of limbs
	2) Performs 2's complement of op1 and stores in result
	3) Overlap is allowed between result and op1
*/
APAC_API void apn_neg(u64* result, const u64* op1, u64 size);

/*
	1) No overlap permitted between result and either of the operands
	2) result must have 2 * size number of limbs
	3) Operands can overlap. If they are the same, use apn_sqr
*/
APAC_API void apn_mul_n(u64* result, const u64* op1, const u64* op2, u64 size);

/*
	1) No overlap permitted between result and either of the operands
	2) result must have (size1 + size2) number of limbs
	3) size1 must be greater than or equal to size2
*/
APAC_API void apn_mul(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/*
	1) No overlap permitted between result and operand
	2) result must have (2 * size) number of limbs
*/
APAC_API void apn_sqr(u64* result, const u64* op1, u64 size);

/*
	1) Result must have size number of limbs
*/
APAC_API void apn_set(u64* result, u64 size, u64 val);

/*
	1) returns 1, 0, -1 based on whether op1 > op2, op1 = op2 or op1 < op2
*/
APAC_API i8 apn_cmp(const u64* op1, const u64* op2, u64 size1, u64 size2);

#endif