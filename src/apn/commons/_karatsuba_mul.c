#include "_hidden_mul.h"
#include "apn_thresholds.h"

/*
*	TO DO:
*		1) Write a verion with apn_cmp instead of apn_negate
*		2) Compare performance gains/loss
*/

void _apn_karatsuba_mul_n(
	u64* result,
	const u64* op1,
	const u64* op2,
	u64 size,
	u64* temp
)
{
	APAC_ASSERT(temp != NULL);

	if (size < KARATSUBA_MUL_N_THRESHOLD)
	{
		// for sizes below threshold
		// use the basecase multiplication

		_apn_basecase_mul(result, op1, op2, size, size);
		return;
	}

	u64 lower = (size + 1) >> 1;	// upper half of the operands
	u64 upper = size >> 1;			// lower half of the operands

	// the lower half is at most 1 limb larger than upper half
	// (size + 1) / 2 = ceil(size / 2)
	// size / 2 = floor(size / 2)

	// a0 = op1[0 : (lower - 1)]
	// a1 = op1[lower : (upper - 1)]

	// temp[0 : (lower - 1)] = (a0 - a1) 
	// if a carry is generated that means a1 > a0 in which case 
	// perform negation of result to get the absolute value

	// cy1 = carryA

	u8 cy1 = apn_sub(temp, op1, op1 + lower, lower, upper);
	if (cy1) apn_neg(temp, temp, lower);

	// b0 = op2[0 : (lower - 1)]
	// b1 = op2[lower : (upper - 1)]
	// temp[lower : (2 * lower - 1)] = (b0 - b1)

	// cy2 = carryB
	// rest is same
	u8 cy2 = apn_sub(temp + lower, op2, op2 + lower, lower, upper);
	if (cy2) apn_neg(temp + lower, temp + lower, lower);

	// result[lower : (3 * lower - 1)] = temp[0 : (lower - 1)] * temp[lower : (2 * lower - 1)]
	_apn_karatsuba_mul_n(result, temp, temp + lower, lower, temp + 2 * lower);

	// we now have c2 in result 

	// copy (2 * lower) count of limbs from result to temp
	// clear out result for accumulating c0 and c1
	apn_cpy(temp, result, 2 * lower);
	apn_set(result, 2 * lower, 0);

	// c0 = a0 * b0
	_apn_karatsuba_mul_n(result, op1, op2, lower, temp + 2 * lower);
	// c1 = a1 * b1
	_apn_karatsuba_mul_n(result + 2 * lower, op1 + lower, op2 + lower, upper, temp + 2 * lower);

	// prepare (c0 + c1) in temp[0 : (2 * lower - 1)]
	u8 val = apn_add(temp + 2 * lower, result, result + 2 * lower, 2 * lower, 2 * upper);
	temp[4 * lower] += val; // propagate carry

	if (cy1 == cy2) // if both signs are same
	{
		// do c2 = c0 + c1 - temp[0 : (2 * lower - 1)]
		apn_sub(temp + 2 * lower, temp + 2 * lower, temp, 2 * lower + 1, 2 * lower);
	}
	else
	{
		// do c2 = c0 + c1 + temp[0 : (2 * lower - 1)]
		apn_add(temp + 2 * lower, temp + 2 * lower, temp, 2 * lower + 1, 2 * lower);
	}

	// add c2 to the middle of result
	apn_add_n(result + lower, result + lower, temp + 2 * lower, 2 * lower + 1);
	apn_set(temp, 4 * lower + 1, 0);	// clear workspace for any further calls
	return;
}

void _apn_karatsuba_mul(
	u64* result,
	const u64* op1,
	const u64* op2,
	u64 size1,
	u64 size2,
	u64* temp
)
{
	APAC_ASSERT(temp != NULL);
	APAC_ASSERT(size1 >= size2);

	if (size2 <= ((size1 + 1) >> 1) || (size1 < KARATSUBA_MUL_THRESHOLD))
	{
		// Highly unbalanced values handled by basecase multiplication

		_apn_basecase_mul(result, op1, op2, size1, size2);
		return;
	}

	// follows nearly the same logic as balanced karatsuba

	APAC_ASSERT(size2 > ((size1 + 1) >> 1));

	u64 lowerA = (size1 + 1) >> 1;
	// lowerB is the same as lowerA
	u64 upperA = size1 - lowerA;
	u64 upperB = size2 - lowerA;

	u8 cy1 = apn_sub(temp, op1, op1 + lowerA, lowerA, upperA);
	if (cy1) apn_neg(temp, temp, lowerA);

	u8 cy2 = apn_sub(temp + lowerA, op2, op2 + lowerA, lowerA, upperB);
	if (cy2) apn_neg(temp + lowerA, temp + lowerA, lowerA);

	// Always Balanced Multiplication
	_apn_karatsuba_mul_n(result, temp, temp + lowerA, lowerA, temp + 2 * lowerA);

	apn_cpy(temp, result, lowerA * 2);
	apn_set(result, lowerA * 2, 0);

	// Always Balanced Multiplication
	_apn_karatsuba_mul_n(result, op1, op2, lowerA, temp + lowerA * 2);

	// Frequently Unbalanced Multiplication
	_apn_karatsuba_mul(result + lowerA * 2, op1 + lowerA, op2 + lowerA, upperA, upperB, temp + lowerA * 2);

	u8 val = apn_add(temp + 2 * lowerA, result, result + 2 * lowerA, 2 * lowerA, upperA + upperB);
	temp[4 * lowerA] += val;

	if (cy1 == cy2)
	{
		apn_sub(temp + 2 * lowerA, temp + 2 * lowerA, temp, 2 * lowerA + 1, 2 * lowerA);
	}
	else
	{
		apn_add(temp + 2 * lowerA, temp + 2 * lowerA, temp, 2 * lowerA + 1, 2 * lowerA);
	}

	apn_add_n(result + lowerA, result + lowerA, temp + 2 * lowerA + 1, 2 * lowerA + 1);
	apn_set(temp, 4 * lowerA + 1, 0);
	return;
}