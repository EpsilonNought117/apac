#include "../headers/hidden_sqr.h"

void apn_karatsuba_sqr(
	apn_dig_t* result,
	const apn_dig_t* op1,
	apn_size_t size,
	apn_dig_t* temp
)
{
	/*
		Read apn_karatsuba_mul code to understand this
		Essentially the same except op1 = op2
		Avoids few O(n) ops compared to Karatsuba Balanced Multiplication
	*/

	APAC_ASSERT(temp != NULL);
	APAC_ASSERT(KARATSUBA_SQR_THRESHOLD >= 5);

	if (size < KARATSUBA_SQR_THRESHOLD)
	{
		apn_basecase_sqr(result, op1, size);
		return;
	}
	
	apn_size_t lower = (size + 1) >> 1;
	apn_size_t upper = size >> 1;

	if (lower == upper)
	{
		int cmp_res = apn_cmp(op1, &op1[lower], lower);

		if (cmp_res < 0)
		{
			apn_sub_n(result, &op1[lower], op1, lower);
		}
		else
		{
			apn_sub_n(result, op1, &op1[lower], lower);
		}
	}
	else
	{
		int cmp_res = (op1[lower - 1] == 0) ? apn_cmp(op1, &op1[lower], upper) : 1;

		if (cmp_res < 0)
		{
			apn_sub_n(result, &op1[lower], op1, upper);
			result[lower - 1] = 0;
		}
		else
		{
			apn_sub(result, op1, &op1[lower], lower, upper);
		}
	}

	// first recursive call
	apn_karatsuba_sqr(temp, result, lower, &temp[lower * 2]);

	// c0 = a0 ^ 2
	apn_karatsuba_sqr(result, op1, lower, &temp[lower * 2]);
	// c1 = a1 ^ 2
	apn_karatsuba_sqr(&result[2 * lower], &op1[lower], upper, &temp[lower * 2]);

	// (c0 + c1)
	apn_dig_t temp_val = apn_add(&temp[2 * lower], result, &result[2 * lower], 2 * lower, 2 * upper);
	
	// c2 = (c0 + c1 - (|c0 - c1|)^2)
	temp_val -= apn_sub_n(&temp[2 * lower], &temp[2 * lower], temp, 2 * lower);

	apn_add(&result[lower], &result[lower], &temp[2 * lower], lower + 2 * upper, 2 * lower);

	if (2 * upper > lower)
	{
		apn_add_one(&result[3 * lower], &result[3 * lower], 2 * upper - lower, temp_val);
	}
			
	return;
}