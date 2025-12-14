#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

apn_seg_t apn_addmul_one(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(curr_cpu.apn_addmul_one_ptr != NULL,
		"apacGetCPUSpec() or apacInit() not called");

	// Prevent partial overlap between result and op1.
	// Allowed only if result is entirely before or after op1's range.
	APAC_ASSERT((result <= op1 - 1) || (result >= op1 + size),
		"(result) lies in the middle of (op1). Allowed only if (result) <= (op1 - 1) "
		"or (result) >= (op1 + size)");

	apn_seg_t out_val = curr_cpu.apn_addmul_one_ptr(result, op1, size, val);
	return out_val;
}
