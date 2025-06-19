#include "_hidden_mul.h"

extern apac_cpu_params curr_cpu;

void apn_basecase_mul(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(result != op2);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);

	if (curr_cpu.apn_mul_bc_ptr == NULL)
		apacGetCPUSpec();

	curr_cpu.apn_mul_bc_ptr(result, op1, op2, size1, size2);
	return;
}
