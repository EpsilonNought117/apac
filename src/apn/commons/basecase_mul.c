#include "hidden_mul.h"

extern apac_cpu_params curr_cpu;

void apn_basecase_mul(
	apn_seg_t* result, 
	const apn_seg_t* op1, 
	const apn_seg_t* op2,
	apn_size_t size1, 
	apn_size_t size2
)
{
	APAC_DETAILED_ASSERT(curr_cpu.apn_mul_bc_ptr != NULL,
		"apacInit() or apacGetCPUSpec() not invoked!"
	);

	curr_cpu.apn_mul_bc_ptr(result, op1, op2, size1, size2);
	return;
}
