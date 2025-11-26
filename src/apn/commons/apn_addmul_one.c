#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

apn_seg_t apn_addmul_one(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(size != 0);
	APAC_ASSERT((result <= op1) || (result >= (op1 + size * 8)));
	APAC_ASSERT(curr_cpu.apn_addmul_one_ptr != NULL);

	apn_seg_t out = curr_cpu.apn_addmul_one_ptr(result, op1, size, val);
	return out;
}