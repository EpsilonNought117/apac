#include "../../../include/apac.h"
#include "hidden_sqr.h"

extern apac_cpu_params curr_cpu;

void apn_basecase_sqr(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(size != 0);
	APAC_ASSERT(curr_cpu.apn_sqr_bc_ptr != NULL);

	curr_cpu.apn_sqr_bc_ptr(result, op1, size);
	return;
}