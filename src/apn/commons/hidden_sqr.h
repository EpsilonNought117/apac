#ifndef APN_HIDDEN_SQR
#define APN_HIDDEN_SQR

#include "../../../include/apac.h"

/*
 * 1) Calls squaring routines written in assembly
 * 2) O(n^2) squaring
 * 3) size cannot be zero
 */
void apn_basecase_sqr(
	apn_seg* result,
	const apn_seg* op1,
	apn_size size
);

/**
* 
*/
void apn_karatsuba_sqr(
	apn_seg* result,
	const apn_seg* op1,
	apn_size size,
	apn_seg* temp
);

#endif
