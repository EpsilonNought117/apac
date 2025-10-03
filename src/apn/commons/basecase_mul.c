#include "hidden_mul.h"

extern apac_cpu_params curr_cpu;

void apn_basecase_mul(
	apn_seg* result, 
	const apn_seg* op1, 
	const apn_seg* op2,
	apn_size size1, 
	apn_size size2
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(result != op2);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);

	APAC_ALWAYS_ASSERT(curr_cpu.apn_mul_bc_ptr != NULL);

	curr_cpu.apn_mul_bc_ptr(result, op1, op2, size1, size2);
	return;
}
