#include "../headers/hidden_mul.h"

void apn_basecase_mul(
	ap_dig_t* result, 
	const ap_dig_t* op1, 
	const ap_dig_t* op2,
	ap_size_t size1, 
	ap_size_t size2
)
{
	APAC_ASSERT(curr_cpu.apn_mul_bc_ptr != NULL);

	curr_cpu.apn_mul_bc_ptr(result, op1, op2, size1, size2);
	return;
}
