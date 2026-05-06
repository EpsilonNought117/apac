#include "../../headers/apac_internal.h"

void apn_cpy(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_PARTIAL_OVERLAP_ABOVE(result, size, op1, size);
	APAC_ASSERT(curr_cpu.apn_cpy_ptr != NULL);

	curr_cpu.apn_cpy_ptr(result, op1, size);
}
