#include "../../../../include/apac.h"
#include "../apac_thresholds.h"
#include "_hidden_mul.h"

void apn_mul_n(u64* result, const u64* op1, const u64* op2, u64 size)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(result != op2);
	APAC_ASSERT(size != 0);

	// zero out result before mul
	apn_set(result, 2 * size, 0);

	if (size < KARATSUBA_MUL_THRESHOLD)
	{
		_apn_basecase_mul(result, op1, op2, size, size);
	}
	else
	{
		_apn_additive_karatsuba_mul(result, op1, op2, size, 0, NULL);
	}

	return;
}