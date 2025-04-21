#ifndef _APN_HIDDEN_MUL_H_
#define _APN_HIDDEN_MUL_H_

#include "../../../../include/apac.h"

/*
	Calls multiplication routines written in assembly
*/
void _apn_basecase_mul(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/*
	O(n^1.59) time complexity
*/
void _apn_additive_karatsuba_mul(u64* result, const u64* op1, const u64* op2, u64 size, u8 depth, u64* workspace);

#endif