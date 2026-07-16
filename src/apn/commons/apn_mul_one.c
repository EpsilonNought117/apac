#include "../../header/apac_internal.h"

void apn_mul_one(
	apn_dig_t* result,
	const apn_dig_t* op1,
	apn_size_t size,
	apn_dig_t val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_PARTIAL_OVERLAP_BELOW(result, size + 1, op1, size);
	APAC_ASSERT(curr_cpu.apn_mul_one_ptr != NULL);

	curr_cpu.apn_mul_one_ptr(result, op1, size, val);
	return;
}