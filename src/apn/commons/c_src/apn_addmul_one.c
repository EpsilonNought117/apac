#include "../../../../include/apac.h"

extern apac_cpu_params curr_cpu;

ap_seg_t apn_addmul_one(
	ap_seg_t* result,
	const ap_seg_t* op1,
	ap_size_t size,
	ap_seg_t val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_PARTIAL_OVERLAP_BELOW(result, size + 1, op1, size);
	APAC_ASSERT(curr_cpu.apn_addmul_one_ptr != NULL);

	ap_seg_t out_val = curr_cpu.apn_addmul_one_ptr(result, op1, size, val);
	return out_val;
}
