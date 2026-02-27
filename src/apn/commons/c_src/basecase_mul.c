#include "../c_headers/hidden_mul.h"

extern apac_cpu_params curr_cpu;

void apn_basecase_mul(
	ap_seg_t* result, 
	const ap_seg_t* op1, 
	const ap_seg_t* op2,
	ap_size_t size1, 
	ap_size_t size2
)
{
	APAC_ASSERT(curr_cpu.apn_mul_bc_ptr != NULL);

	curr_cpu.apn_mul_bc_ptr(result, op1, op2, size1, size2);
	return;
}
