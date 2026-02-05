#include "../c_headers/hidden_mul.h"
#include "../c_headers/apn_thresholds.h"

/*
*	TO DO:
*		1) Write a verion with apn_cmp instead of apn_negate
*		2) Compare performance gains/loss
* 
*/

void apn_karatsuba_mul_balanced(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size,
	apn_seg_t* temp
)
{
	APAC_ASSERT(temp != NULL);

	if (size < KARATSUBA_MUL_BALANCED_THRESHOLD)
	{
		// for sizes below threshold
		// use the basecase multiplication

		apn_basecase_mul(result, op1, op2, size, size);
		return;
	}

	apn_size_t lower = (size + 1) >> 1;	// upper half of the operands
	apn_size_t upper = size >> 1;		// lower half of the operands

	// the lower half is at most 1 limb larger than upper half
	// (size + 1) / 2 = ceil(size / 2)
	// size / 2 = floor(size / 2)

	// a0 = op1[0 : (lower - 1)]
	// a1 = op1[lower : (upper - 1)]

	// temp[0 : (lower - 1)] = (a0 - a1) 
	// if a carry is generated that means a1 > a0 in which case 
	// perform negation of result to get the absolute value

	// carry1 = carryA

	apn_seg_t carry1 = apn_sub(temp, op1, op1 + lower, lower, upper);
	if (carry1) { apn_neg(temp, temp, lower); }

	// b0 = op2[0 : (lower - 1)]
	// b1 = op2[lower : (upper - 1)]
	// temp[lower : (2 * lower - 1)] = (b0 - b1)

	// carry2 = carryB
	// rest is same
	apn_seg_t carry2 = apn_sub(temp + lower, op2, op2 + lower, lower, upper);
	if (carry2) { apn_neg(temp + lower, temp + lower, lower); }

	// result[lower : (3 * lower - 1)] = temp[0 : (lower - 1)] * temp[lower : (2 * lower - 1)]
	apn_karatsuba_mul_balanced(result, temp, temp + lower, lower, temp + 2 * lower);

	// we now have c2 in result 

	// copy (2 * lower) count of limbs from result to temp
	// clear out result for accumulating c0 and c1
	apn_cpy(temp, result, 2 * lower);
	apn_set(result, 2 * lower, 0);

	// c0 = a0 * b0
	apn_karatsuba_mul_balanced(result, op1, op2, lower, temp + 2 * lower);
	// c1 = a1 * b1
	apn_karatsuba_mul_balanced(result + 2 * lower, op1 + lower, op2 + lower, upper, temp + 2 * lower);

	// prepare (c0 + c1) in temp[(2 * lower) : (4 * lower - 1)]
	apn_seg_t val = apn_add(temp + 2 * lower, result, result + 2 * lower, 2 * lower, 2 * upper);
	temp[4 * lower] += val; // propagate carry

	if (carry1 == carry2) // if both signs are same
	{
		// do c2 = c0 + c1 - (|c0 - c1|)^2
		apn_sub(temp + 2 * lower, temp + 2 * lower, temp, 2 * lower + 1, 2 * lower);
	}
	else
	{
		// do c2 = c0 + c1 + (|c0 - c1|)^2
		apn_add(temp + 2 * lower, temp + 2 * lower, temp, 2 * lower + 1, 2 * lower);
	}

	// add c2 to the middle of result
	apn_add_n(result + lower, result + lower, temp + 2 * lower, 2 * lower + 1);
	apn_set(temp, 4 * lower + 1, 0);	// clear workspace for any further calls
	return;
}

void apn_karatsuba_mul_unbalanced(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size1,
	apn_size_t size2,
	apn_seg_t* temp
)
{
	APAC_ASSERT(temp != NULL);
	APAC_ASSERT(size1 >= size2);

	if ((size1 < KARATSUBA_MUL_UNBALANCED_THRESHOLD) || (size2 <= ((size1 + 1) >> 1)))
	{
		// Highly unbalanced values or values below threshold
		// handled by basecase multiplication

		apn_basecase_mul(result, op1, op2, size1, size2);
		return;
	}

	// follows nearly the same logic as balanced karatsuba

	apn_size_t lowerA = (size1 + 1) >> 1;
	// lowerB is the same as lowerA
	apn_size_t upperA = size1 - lowerA;
	apn_size_t upperB = size2 - lowerA;

	apn_seg_t carry1 = apn_sub(temp, op1, op1 + lowerA, lowerA, upperA);
	if (carry1) { apn_neg(temp, temp, lowerA); }

	apn_seg_t carry2 = apn_sub(temp + lowerA, op2, op2 + lowerA, lowerA, upperB);
	if (carry2) { apn_neg(temp + lowerA, temp + lowerA, lowerA); }

	// Always Balanced Multiplication
	apn_karatsuba_mul_balanced(result, temp, temp + lowerA, lowerA, temp + 2 * lowerA);

	apn_cpy(temp, result, lowerA * 2);
	apn_set(result, lowerA * 2, 0);

	// Always Balanced Multiplication
	apn_karatsuba_mul_balanced(result, op1, op2, lowerA, temp + lowerA * 2);

	// Always Unbalanced Multiplication
	apn_karatsuba_mul_unbalanced(result + lowerA * 2, op1 + lowerA, op2 + lowerA, upperA, upperB, temp + lowerA * 2);

	apn_seg_t val = apn_add(temp + 2 * lowerA, result, result + 2 * lowerA, 2 * lowerA, upperA + upperB);
	temp[4 * lowerA] += val;

	if (carry1 == carry2)
	{
		apn_sub(temp + 2 * lowerA, temp + 2 * lowerA, temp, 2 * lowerA + 1, 2 * lowerA);
	}
	else
	{
		apn_add(temp + 2 * lowerA, temp + 2 * lowerA, temp, 2 * lowerA + 1, 2 * lowerA);
	}

	apn_add_n(result + lowerA, result + lowerA, temp + 2 * lowerA, 2 * lowerA + 1);
	apn_set(temp, 4 * lowerA + 1, 0);
	return;
}