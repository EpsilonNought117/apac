#include "../../../include/apac.h"
#include "x64_hidden_funcs.h"

extern apac_cpu_params curr_cpu;

void zen4_set_params(void)
{
	curr_cpu.karatsuba_mul_balanced_threshold = 33;
	curr_cpu.karatsuba_mul_unbalanced_threshold = 33;

	curr_cpu.apn_add_n_ptr = add_n_zen4;
	curr_cpu.apn_sub_n_ptr = sub_n_zen4;
	curr_cpu.apn_mul_bc_ptr = mul_bc_zen4;
	curr_cpu.apn_neg_ptr = neg_zen4;
	curr_cpu.apn_set_ptr = set_avx512f_4unroll;
	curr_cpu.apn_cpy_ptr = cpy_avx512f_4unroll;

	return;
}

void generic_x64_set_params(void)
{
	curr_cpu.karatsuba_mul_balanced_threshold = 20;
	curr_cpu.karatsuba_mul_unbalanced_threshold = 30;

	curr_cpu.apn_add_n_ptr = add_n_x64;
	curr_cpu.apn_sub_n_ptr = sub_n_x64;
	curr_cpu.apn_mul_bc_ptr = mul_bc_x64;
	curr_cpu.apn_neg_ptr = neg_x64;
	curr_cpu.apn_set_ptr = set_sse2_4unroll;
	curr_cpu.apn_cpy_ptr = cpy_sse2_4unroll;

	return;
}