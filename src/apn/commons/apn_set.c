#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

void apn_set(u64* result, u64 size, u64 val)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(size != 0);

	if (curr_cpu.apn_set_ptr == NULL)
		apacGetCPUSpec();

	curr_cpu.apn_set_ptr(result, size, val);

	return;
}