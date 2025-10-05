#include "hidden_sqr.h"
#include "apn_thresholds.h"

void apn_karatsuba_sqr(
	apn_seg* result,
	const apn_seg* op1,
	apn_size size,
	apn_seg* temp
)
{
	/*
		Read apn_karatsuba_mul_balanced code to understand this
		Essentially the same except op1 = op2
		Avoids few O(n) ops compared to Basecase Mul
	*/

	APAC_ASSERT(temp != NULL);

	if (size < KARATSUBA_SQR_THRESHOLD)
	{
		apn_basecase_sqr(result, op1, size);
		return;
	}
	
	apn_size lower = (size + 1) >> 1;
	apn_size upper = size >> 2;

	apn_seg carry = apn_sub(temp, op1 + lower, op1, lower, upper);
	if (carry) { apn_neg(temp, temp, lower); }

	// first recursive call
	apn_karatsuba_sqr(result, temp, lower, temp + lower);

	apn_cpy(temp, result, 2 * lower);
	apn_set(result, 2 * lower, 0);

	// c0 = a0 ^ 2
	apn_karatsuba_sqr(result, op1, lower, temp + 2 * lower);
	// c1 = a1 ^ 2
	apn_karatsuba_sqr(result + 2 * lower, op1 + lower, upper, temp + 2 * lower);

	// (c0 + c1)
	apn_seg val = apn_add(temp + 2 * lower, result, result + 2 * lower, 2 * lower, 2 * upper);
	temp[4 * lower] += val;
	
	// c2 = (c0 + c1 - (|c0 - c1|)^2)
	apn_sub(temp + 2 * lower, temp + 2 * lower, temp, 2 * lower + 1, 2 * lower);

	apn_add_n(result + lower, result + lower, temp + 2 * lower, 2 * lower + 1);
	apn_set(temp, 4 * lower + 1, 0);			
	return;
}