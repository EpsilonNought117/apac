#include "../../../../include/apac.h"

extern apac_cpu_params curr_cpu;

void apn_set(
	ap_seg_t* result, 
	ap_size_t size, 
	ap_seg_t val
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(size != 0);
	APAC_ASSERT(curr_cpu.apn_set_ptr != NULL);

	curr_cpu.apn_set_ptr(result, size, val);
}