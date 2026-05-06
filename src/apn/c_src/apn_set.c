#include "../headers/apac_internal.h"

void apn_set(
	ap_dig_t* result, 
	ap_size_t size, 
	ap_dig_t val
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(size != 0);
	APAC_ASSERT(curr_cpu.apn_set_ptr != NULL);

	curr_cpu.apn_set_ptr(result, size, val);
}