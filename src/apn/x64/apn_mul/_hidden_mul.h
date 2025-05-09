#ifndef _APN_HIDDEN_MUL_H_
#define _APN_HIDDEN_MUL_H_

#include "../../../../include/apac.h"

/*
	Calls multiplication routines written in assembly
	O(n^2) schoolbook multiplication
*/
void _apn_basecase_mul(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/*
	Subtractive Karatsuba variant, Uses (2*n + 32) limbs of space for size n operands
	O(n^1.585) time complexity
*/
void _apn_karatsuba_mul_n(u64* result, const u64* op1, const u64* op2, u64 size, u64* temp);

#endif