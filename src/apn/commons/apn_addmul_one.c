#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

apn_seg apn_addmul_one(
	apn_seg* result,
	const apn_seg* op1,
	apn_size size,
	apn_seg val
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(size != 0);
	APAC_ASSERT(curr_cpu.apn_addmul_one_ptr != NULL);

	apn_seg out = curr_cpu.apn_addmul_one_ptr(result, op1, size, val);
	return out;
}