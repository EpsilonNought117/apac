#ifndef APN_HIDDEN_MUL
#define APN_HIDDEN_MUL

#include "../../../include/apac.h"

/*
 
1) Calls multiplication routines written in assembly
2) Unbalanced & Balanced multiplications can be done
3) O(n^2) schoolbook multiplication
4) size1 must be greater than or equal to size2

*/
void apn_basecase_mul(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size1, apn_size size2);

/*

1) Subtractive Karatsuba variant
2) Balanced multiplication
3) Uses (2 * n + 64) limbs of space for size n operands
4) O(n^1.585) time complexity

*/
void apn_karatsuba_mul_balanced(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size, apn_seg* temp);

/*
	
1) Subtractive Karatsuba variant
2) Unbalanced Multiplication
3) Uses (2 * size1 + 64) space
4) Time complexity in worst case can be O(n^2) and O(n^1.585) in best case (or somewhere in between) depending upon size2 
5) size1 must be greater than or equal to size2

*/
void apn_karatsuba_mul_unbalanced(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size1, apn_size size2, apn_seg* temp);

#endif