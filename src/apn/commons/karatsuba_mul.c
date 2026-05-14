#include "../headers/hidden_mul.h"

/*
*	TO DO:
*		1) Write a verion with apn_cmp instead of apn_negate
*		2) Compare performance gains/loss
* 
*/

void apn_karatsuba_mul(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size1,
	ap_size_t size2,
	ap_dig_t* temp
)
{
	APAC_ASSERT(temp != NULL);
	APAC_ASSERT(KARATSUBA_MUL_THRESHOLD >= 2);

	bool is_karatsuba_valid = (size2 > (size1 + 1) / 2) && (size1 >= KARATSUBA_MUL_THRESHOLD);

	if (!is_karatsuba_valid)
	{
		// for sizes below threshold
		// use the basecase multiplication

		apn_mul_dispatcher(result, op1, op2, size1, size2, temp);
		return;
	}

	ap_size_t lower = (size1 + 1) >> 1;	// lower half of the operands
	ap_size_t upper_a = size1 - lower;  // upper half of operand 1
	ap_size_t upper_b = size2 - lower;	// upper half of operand 2

	// the lower half is at most 1 digit larger than upper half
	// (size + 1) / 2 = ceil(size / 2)
	// size / 2 = floor(size / 2)

	// a0 = op1[0 : (lower - 1)]
	// a1 = op1[lower : (upper - 1)]

	// temp[0 : (lower - 1)] = (a0 - a1) 
	// if a carry is generated that means a1 > a0 in which case 
	// perform negation of result to get the absolute value

	// carry1 = carryA

	ap_dig_t carry1 = apn_sub(temp, op1, &op1[lower], lower, upper_a);
	if (carry1) { apn_neg(temp, temp, lower); }

	// b0 = op2[0 : (lower - 1)]
	// b1 = op2[lower : (upper - 1)]
	// temp[lower : (2 * lower - 1)] = (b0 - b1)

	// carry2 = carryB
	// rest is same
	ap_dig_t carry2 = apn_sub(&temp[lower], op2, &op2[lower], lower, upper_b);
	if (carry2) { apn_neg(&temp[lower], &temp[lower], lower); }

	// result[0 : (2 * lower - 1)] = temp[0 : (lower - 1)] * temp[lower : (2 * lower - 1)]
	apn_karatsuba_mul(result, temp, &temp[lower], lower, lower, &temp[2 * lower]);

	// we now have c2 in result 

	// copy c2 from result to temp
	// clear out result for accumulating c0 and c1
	apn_cpy(temp, result, 2 * lower);
	apn_set(result, 2 * lower, 0);

	// c0 = a0 * b0
	apn_karatsuba_mul(result, op1, op2, lower, lower, &temp[2 * lower]);
	// c1 = a1 * b1
	apn_karatsuba_mul(&result[2 * lower], &op1[lower], &op2[lower], upper_a, upper_b, &temp[2 * lower]);

	// prepare (c0 + c1) in temp[(2 * lower) : (4 * lower - 1)] and then propagate any carry
	temp[4 * lower] += apn_add(&temp[2 * lower], result, &result[2 * lower], 2 * lower, upper_a + upper_b);
	 
	if (carry1 == carry2) // if both signs are same
	{
		// do c2 = c0 + c1 - c2
		apn_sub(&temp[2 * lower], &temp[2 * lower], temp, 2 * lower + 1, 2 * lower);
	}
	else // otherwise if opposite signs
	{
		// do c2 = c0 + c1 + c2
		apn_add(&temp[2 * lower], &temp[2 * lower], temp, 2 * lower + 1, 2 * lower);
	}

	/*
		|------------- c --------------|
		
		|----- c0 -----|
		|   2 * lower  |
				|----- c2 -----|
				|   2 * lower  |
						|----- c1 -----|
						|   2 * upper  |
	*/

	ap_size_t temp_size1 = lower + upper_a + upper_b;
	ap_size_t temp_size2 = temp_size1 > 2 * lower + 1 ? temp_size1 : 2 * lower + 1;
	
	// add c2 to the middle of result
	apn_add(&result[lower], &result[lower], &temp[2 * lower], temp_size1, temp_size2);
	apn_set(temp, 4 * lower + 1, 0);	// clear workspace for any further calls

	return;
}