#include "../headers/hidden_sqr.h"

void apn_karatsuba_sqr(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t* temp
)
{
	/*
		Read apn_karatsuba_mul code to understand this
		Essentially the same except op1 = op2
		Avoids few O(n) ops compared to Karatsuba Balanced Multiplication
	*/

	APAC_ASSERT(temp != NULL);
	APAC_ASSERT(KARATSUBA_SQR_THRESHOLD >= 2);

	if (size < KARATSUBA_SQR_THRESHOLD)
	{
		apn_basecase_sqr(result, op1, size);
		return;
	}
	
	ap_size_t lower = (size + 1) >> 1;
	ap_size_t upper = size >> 1;

	ap_dig_t carry = apn_sub(temp, op1, &op1[lower], lower, upper);
	if (carry) { apn_neg(temp, temp, lower); }

	// first recursive call
	apn_karatsuba_sqr(result, temp, lower, &temp[lower]);

	apn_cpy(temp, result, 2 * lower);
	apn_set(result, 2 * lower, 0);

	// c0 = a0 ^ 2
	apn_karatsuba_sqr(result, op1, lower, &temp[2 * lower]);
	// c1 = a1 ^ 2
	apn_karatsuba_sqr(&result[2 * lower], &op1[lower], upper, &temp[2 * lower]);

	// (c0 + c1)
	temp[4 * lower] += apn_add(&temp[2 * lower], result, &result[2 * lower], 2 * lower, 2 * upper);
	
	// c2 = (c0 + c1 - (|c0 - c1|)^2)
	apn_sub(&temp[2 * lower], &temp[2 * lower], temp, 2 * lower + 1, 2 * lower);

	apn_add(&result[lower], &result[lower], &temp[2 * lower], 2 * lower + upper, 2 * lower + 1);
	apn_set(temp, 4 * lower + 1, 0);
			
	return;
}