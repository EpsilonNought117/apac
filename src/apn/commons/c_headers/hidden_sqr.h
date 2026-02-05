#ifndef APN_HIDDEN_SQR
#define APN_HIDDEN_SQR

#include "../../../../include/apac.h"

/*
 * 1) Calls squaring routines written in assembly
 * 2) O(n^2) squaring
 * 3) In-place, no auxiliary space needed
 * 4) size cannot be zero
 * 5) Faster than doing basecase_mul with identical input operands as it does approx n(n+1)/2 steps
 */
void apn_basecase_sqr(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

/*
* 1) Subtractive Karatsuba Variant
* 2) Needs (2 * n + 64) limbs of auxiliary space
* 3) O(n^1.585) time complexity
* 4) Slightly faster than doing karatsuba mul with identical inputs, avoids some O(n) ops that mul has to do
* 5) Recursively calls basecase multiplication
*/
void apn_karatsuba_sqr(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t* temp
);

#endif
