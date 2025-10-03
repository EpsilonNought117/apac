#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

void apn_set(
	apn_seg* result, 
	apn_size size, 
	apn_seg val
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(size != 0);

	APAC_ALWAYS_ASSERT(curr_cpu.apn_set_ptr != NULL);

	curr_cpu.apn_set_ptr(result, size, val);

	return;
}