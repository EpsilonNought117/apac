#include "../../../../include/apac.h"

ap_dig_t apn_submul_one(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t val
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(size != 0);
	APAC_PARTIAL_OVERLAP_ABOVE(result, size + 1, op1, size);
	APAC_ASSERT(curr_cpu.apn_submul_one_ptr != NULL);

	ap_dig_t out_val = curr_cpu.apn_submul_one_ptr(result, op1, size, val);
	return out_val;
}