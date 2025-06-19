#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

void apn_cpy(u64* result, const u64* op1, u64 size)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != op1);

	if (curr_cpu.apn_cpy_ptr == NULL)
		apacGetCPUSpec();

	curr_cpu.apn_cpy_ptr(result, op1, size);

	return;
}