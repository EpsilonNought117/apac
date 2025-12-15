#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

apn_seg_t apn_submul_one(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(size != 0);
	APAC_PARTIAL_OVERLAP_BELOW(result, 1, op1, size);
	APAC_DETAILED_ASSERT(curr_cpu.apn_submul_one_ptr != NULL,
		"apacInit() or apacGetCPUSpec() not invoked!"
	);
	apn_seg_t out_val = curr_cpu.apn_submul_one_ptr(result, op1, size, val);
	return out_val;
}