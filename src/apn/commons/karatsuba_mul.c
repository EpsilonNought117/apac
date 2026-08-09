#include "../headers/hidden_mul.h"

/*
*	TO DO:
*		1) Write a verion with apn_cmp instead of apn_negate
*		2) Compare performance gains/loss
* 
*/

void apn_karatsuba_mul(
	apn_dig_t* result,
	const apn_dig_t* op1,
	const apn_dig_t* op2,
	apn_size_t size1,
	apn_size_t size2,
	apn_dig_t* temp
)
{
	APAC_ASSERT(temp != NULL);
	APAC_ASSERT(KARATSUBA_MUL_THRESHOLD >= 2);

	if (size1 < KARATSUBA_MUL_THRESHOLD || (size2 <= (size1 + 1) / 2))
	{
		apn_basecase_mul(result, op1, op2, size1, size2);
		return;
	}

	apn_size_t lower = (size1 + 1) >> 1;	// lower half of the operands
	apn_size_t upper_a = size1 - lower;  // upper half of operand 1
	apn_size_t upper_b = size2 - lower;	// upper half of operand 2

	APAC_ASSERT(upper_a > 0 && upper_a <= lower);
	APAC_ASSERT(upper_b > 0 && upper_b <= lower);
	APAC_ASSERT(upper_a + upper_b >= lower);

	int cmp_res = 0;
	int isneg1 = 0, isneg2 = 0; // false in both

	if (lower == upper_a)
	{
		cmp_res = apn_cmp(op1, &op1[lower], lower);

		if (cmp_res < 0)
		{
			apn_sub_n(result, &op1[lower], op1, lower);
			isneg1 = 1;	// true
		}
		else
		{
			apn_sub_n(result, op1, &op1[lower], lower);
		}
	}
	else
	{
		cmp_res = (op1[lower - 1] == 0) ? apn_cmp(op1, &op1[lower], upper_a) : 1;

		if (cmp_res < 0)
		{
			apn_sub_n(result, &op1[lower], op1, upper_a);
			result[lower - 1] = 0;
			isneg1 = 1; // true
		}
		else
		{
			apn_sub(result, op1, &op1[lower], lower, upper_a);
		}
	}

	if (lower == upper_b)
	{
		cmp_res = apn_cmp(op2, &op2[lower], lower);

		if (cmp_res < 0)
		{
			apn_sub_n(&result[lower], &op2[lower], op2, lower);
			isneg2 = 1; // true
		}
		else
		{
			apn_sub_n(&result[lower], op2, &op2[lower], lower);
		}
	}
	else
	{
		cmp_res = apn_is_zero(&op2[upper_b], lower - upper_b) ? apn_cmp(op2, &op2[lower], upper_b) : 1;

		if (cmp_res < 0)
		{
			apn_sub_n(&result[lower], &op2[lower], op2, upper_b);
			apn_set(&result[lower + upper_b], lower - upper_b, 0);
			isneg2 = 1;
		}
		else
		{
			apn_sub(&result[lower], op2, &op2[lower], lower, upper_b);
		}
	}

	apn_karatsuba_mul(temp, result, &result[lower], lower, lower, &temp[2 * lower]);

	// c0 = a0 * b0
	apn_karatsuba_mul(result, op1, op2, lower, lower, &temp[2 * lower]);
	// c1 = a1 * b1
	apn_karatsuba_mul(&result[2 * lower], &op1[lower], &op2[lower], upper_a, upper_b, &temp[2 * lower]);

	// prepare (c0 + c1) in temp[(2 * lower) : (4 * lower - 1)] and then propagate any carry
	apn_dig_t temp_val = apn_add(&temp[2 * lower], result, &result[2 * lower], 2 * lower, upper_a + upper_b);

	if (isneg1 == isneg2) // if both signs are same
	{
		// do c2 = c0 + c1 - c2
		temp_val -= apn_sub_n(&temp[2 * lower], &temp[2 * lower], temp, 2 * lower);
	}
	else // otherwise if opposite signs
	{
		// do c2 = c0 + c1 + c2
		temp_val += apn_add_n(&temp[2 * lower], &temp[2 * lower], temp, 2 * lower);
	}

	/*
		|--------------  c  --------------|

		|------- c0 -------|
		|     2 * lower    |
				|------- c2 -------|
				|     2 * lower    |
						|------ c1 -------|
						|upper_a + upper_b|
	*/

	// add c2 to the middle of result
	apn_add(&result[lower], &result[lower], &temp[2 * lower], lower + upper_a + upper_b, 2 * lower);

	if (upper_a + upper_b > lower)
	{
		apn_add_one(&result[3 * lower], &result[3 * lower], upper_a + upper_b - lower, temp_val);
	}

	return;
}