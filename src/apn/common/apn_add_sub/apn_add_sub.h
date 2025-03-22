#ifndef APN_ADD_SUB_H
#define APN_ADD_SUB_H
#include "../../apn_def.h"
#include "../../../apacExport.h"

/**
* Adds two operand arrays of type u64 and stores them in result and returns carry of type u8
*
* Input -> result, operand1, operand2, size (same for both operands)
* Output -> Carry out resulting from multiprecision add (0 or 1)
* NOTE -> Does not perform any memory allocation in result and assumes that result has (size) segments of space
*/
APAC_API u8 apn_add_n(u64* result, u64* op1, u64* op2, u64 size);

/**
* Adds two operand arrays of type u64 and stores them in result and returns carry
*
* Input -> result, operand1, operand2, size1, size2
* Output -> Carry out resulting from multiprecision add (0 or 1)
* NOTE -> Does not perform any memory allocation in result and assumes that result has max(size1, size2) segments of space
*/
APAC_API u8 apn_add(u64* result, u64* op1, u64* op2, u64 size1, u64 size2);

/**
* Adds one integer value of type u64 to an operand array of type u64 to and stores in result, returning any carry
*
* Input -> result, operand1, val, size (of operand1)
* Output -> Carry out
* NOTE -> Does not perform any memory allocation in result and assumes that result has (size) segments of space
*/
APAC_API u8 apn_add_one(u64* result, u64* op1, u64 size, u64 val);


#endif
