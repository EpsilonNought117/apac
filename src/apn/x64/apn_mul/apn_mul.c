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

	_apn_basecase_mul(result, op1, op2, size, size); // size >= size
}