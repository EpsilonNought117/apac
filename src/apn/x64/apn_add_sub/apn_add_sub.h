#ifndef APN_ADD_SUB_H
#define APN_ADD_SUB_H
#include "../../apn_def.h"
#include "../../../apacExport.h"

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
APAC_API u8 apn_add_n(u64* result, u64* op1, u64* op2, u64 size);

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
APAC_API u8 apn_add(u64* result, u64* op1, u64* op2, u64 size1, u64 size2);

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
APAC_API u8 apn_add_one(u64* result, u64* op1, u64 size, u64 val);

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
APAC_API u8 apn_sub_n(u64* result, u64* op1, u64* op2, u64 size);

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
APAC_API u8 apn_sub(u64* result, u64* op1, u64* op2, u64 size1, u64 size2);

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
APAC_API u8 apn_sub_one(u64* result, u64* op1, u64 size, u64 val);

#endif
