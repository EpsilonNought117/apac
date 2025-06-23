#ifndef APN_HIDDEN_MUL
#define APN_HIDDEN_MUL

#include "../../../include/apac.h"

/*
	1) Calls multiplication routines written in assembly
	2) Unbalanced/Balanced Both
	3) O(n^2) schoolbook multiplication
	4) size1 must be greater than or equal to size2
*/
void apn_basecase_mul(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/*
	1) Subtractive Karatsuba variant
	2) Balanced multiplication
	3) Uses (2 * n + 64) limbs of space for size n operands
	4) O(n^1.585) time complexity
*/
void apn_karatsuba_mul_n(u64* result, const u64* op1, const u64* op2, u64 size, u64* temp);

/*
	1) Subtractive Karatsuba variant
	2) Unbalanced Multiplication
	3) Uses (2 * size1 + 64) space
	4) Time complexity in worst case can be O(n^2) and O(n^1.585) in best case depending upon size2 
	5) size1 must be greater than or equal to size2
*/
void apn_karatsuba_mul(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2, u64* temp);

#endif