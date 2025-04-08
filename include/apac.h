#ifndef APAC_H
#define APAC_H

/****************************************************************************************************/
/*********************************      REQUIRED STANDARD HEADERS     ********************************/
/****************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <intrin.h>
#include <immintrin.h>
#include <ammintrin.h>

/****************************************************************************************************/
/*********************************       SHARED AND STATIC DEFS      ********************************/
/****************************************************************************************************/

#if defined(BUILD_SHARED_LIBS)
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
/*********************************         MISCELLANEOUS          ***********************************/
/****************************************************************************************************/

typedef enum apac_err
{
    APAC_OK,
    APAC_OOM,
    APAC_MATH_ERR

}   apac_err;

#ifndef APAC_ASSERT
#define APAC_ASSERT(x) assert(x)
#endif

#ifndef APAC_REPORT_ERR
#define APAC_REPORT_ERR(x) fprintf(stderr, "APAC ERROR: %s\n", x)
#endif

// for setMemFuncs()

APAC_API extern void* (*apac_malloc)(size_t);
APAC_API extern void* (*apac_realloc)(void*, size_t);
APAC_API extern void (*apac_free)(void*);

APAC_API void setMemFuncs(
    void* (*ptr1)(size_t),
    void* (*ptr2)(void*, size_t),
    void (*ptr3)(void*)
);

APAC_API void apacInitDefault(void);

/****************************************************************************************************/
/*********************************        FETCH CPU FEATURES       **********************************/
/****************************************************************************************************/

// cpu feature flags

extern uint8_t adx_chk;
extern uint8_t bmi2_chk;
extern uint8_t avx2_chk;
extern uint8_t avx_chk;
extern uint8_t avx512f_chk;

APAC_API void getCPUSpec(void);

/****************************************************************************************************/
/*********************************          APN DEFINITIONS        **********************************/
/****************************************************************************************************/

typedef uint64_t u64;
typedef uint16_t u16;
typedef uint8_t   u8;
typedef int64_t  i64;

/****************************************************************************************************/
/*********************************          APN FUNCTIONS         ***********************************/
/****************************************************************************************************/

/**
* Adds two operand arrays of type u64 segment-wise and stores the result, returning the carry (0 or 1).
*
* Parameters:
*   - result: Output array storing the sum.
*   - op1: First input operand array.
*   - op2: Second input operand array.
*   - size: Number of segments in both operands.
*
* Returns:
*   - Carry out resulting from op1[segment] + op2[segment].
*
* Note:
*   - Does not perform memory allocation; result must have 'size' allocated segments.
*/
APAC_API u8 apn_add_n(u64* result, const u64* op1, const u64* op2, u64 size);

/**
* Adds two operand arrays of type u64 segment-wise and stores the result, returning the carry (0 or 1).
*
* Parameters:
*   - result: Output array storing the sum.
*   - op1: First input operand array.
*   - op2: Second input operand array.
*   - size1: Number of segments in op1.
*   - size2: Number of segments in op2.
*
* Returns:
*   - Carry out resulting from op1[segment] + op2[segment].
*
* Note:
*   - Does not perform memory allocation; result must have max(size1, size2) allocated segments.
*/
APAC_API u8 apn_add(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/**
* Adds a single u64 value to an operand array and stores the result, returning any carry (0 or 1).
*
* Parameters:
*   - result: Output array storing the sum.
*   - op1: Input operand array.
*   - size: Number of segments in op1.
*   - val: Single u64 value to add.
*
* Returns:
*   - Carry out resulting from op1[0] + val.
*
* Note:
*   - Does not perform memory allocation; result must have 'size' allocated segments.
*/
APAC_API u8 apn_add_one(u64* result, const u64* op1, u64 size, u64 val);

/**
* Subtracts two operand arrays of type u64 segment-wise and stores the result, returning the borrow (0 or 1).
*
* Parameters:
*   - result: Output array storing the difference.
*   - op1: First input operand array.
*   - op2: Second input operand array.
*   - size: Number of segments in both operands.
*
* Returns:
*   - Borrow out resulting from op1[segment] - op2[segment].
*
* Note:
*   - Does not perform memory allocation; result must have 'size' allocated segments.
*/
APAC_API u8 apn_sub_n(u64* result, const u64* op1, const u64* op2, u64 size);

/**
* Subtracts two operand arrays of type u64 segment-wise and stores the result, returning the borrow (0 or 1).
*
* Parameters:
*   - result: Output array storing the difference.
*   - op1: First input operand array.
*   - op2: Second input operand array.
*   - size1: Number of segments in op1.
*   - size2: Number of segments in op2.
*
* Returns:
*   - Borrow out resulting from op1[segment] - op2[segment].
*
* Note:
*   - Does not perform memory allocation; result must have max(size1, size2) allocated segments.
*   - size1 must be greater than or equal to size2.
*/
APAC_API u8 apn_sub(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/**
* Subtracts a single u64 value from an operand array and stores the result, returning any borrow (0 or 1).
*
* Parameters:
*   - result: Output array storing the difference.
*   - op1: Input operand array.
*   - size: Number of segments in op1.
*   - val: Single u64 value to subtract.
*
* Returns:
*   - Borrow out resulting from op1[0] - val.
*
* Note:
*   - Does not perform memory allocation; result must have 'size' allocated segments.
*/
APAC_API u8 apn_sub_one(u64* result, const u64* op1, u64 size, u64 val);

/**
* Copies (size) number of segments from op1 to result.
*
* Input -> result, op1, size (of both op1 and result)
* Output -> None
*
* NOTE -> Assumes both result and op1 have (size) amount of segments
*/
APAC_API void apn_cpy(u64* result, const u64* op1, u64 size);

/**
* Computes the two's complement (negation) of a big integer stored as 64-bit segments.
*
* Parameters:
*   - op1: Input/output operand array representing the big integer.
*   - size: Number of segments in op1.
*
* Note:
*   - Modifies op1 in place.
*/
APAC_API void apn_negate(u64* result, const u64* op1, u64 size);

/**
* 
*/
APAC_API void apn_mul_n(u64* result, const u64* op1, const u64* op2, u64 size);

#endif