#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

void apn_cpy(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_PARTIAL_OVERLAP_BELOW(result, size + 1, op1, size);
	APAC_DETAILED_ASSERT(curr_cpu.apn_cpy_ptr != NULL,
		"apacInit() or apacGetCPUSpec() not invoked!"
	);

	curr_cpu.apn_cpy_ptr(result, op1, size);
}
