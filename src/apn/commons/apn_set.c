#include "../../header/apac_internal.h"

void apn_set(
	apn_dig_t* result, 
	apn_size_t size, 
	apn_dig_t val
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(size != 0);
	APAC_ASSERT(curr_cpu.apn_set_ptr != NULL);

	curr_cpu.apn_set_ptr(result, size, val);
}