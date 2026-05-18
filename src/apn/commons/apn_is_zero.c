#include "../headers/apn.h"

int apn_is_zero(
	const ap_dig_t* op1,
	ap_size_t size
)
{
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(size != 0);
	APAC_ASSERT(curr_cpu.apn_is_zero_ptr != NULL);

	int val = curr_cpu.apn_is_zero_ptr(op1, size);
	return val;
}