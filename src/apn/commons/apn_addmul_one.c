#include "../../header/apac_internal.h"

apn_dig_t apn_addmul_one(
	apn_dig_t* result,
	const apn_dig_t* op1,
	apn_size_t size,
	apn_dig_t val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_PARTIAL_OVERLAP_ABOVE(result, size + 1, op1, size);
	APAC_ASSERT(curr_cpu.apn_addmul_one_ptr != NULL);

	apn_dig_t out_val = curr_cpu.apn_addmul_one_ptr(result, op1, size, val);
	return out_val;
}
