#include "../../../include/apac.h"
#include "x64_hidden_funcs.h"

extern apac_cpu_params curr_cpu;

void zen4_set_params(void)
{
	curr_cpu.karatsuba_mul_n_threshold = 43;
	curr_cpu.karatsuba_mul_threshold = 43;

	curr_cpu.apn_add_n_ptr = add_n_4unroll;
	curr_cpu.apn_sub_n_ptr = sub_n_4unroll;
	curr_cpu.apn_mul_bc_ptr = mul_bc_mulx_adx_4unroll;
	curr_cpu.apn_neg_ptr = neg_4unroll;
	curr_cpu.apn_set_ptr = set_avx512f_2unroll;
	curr_cpu.apn_cpy_ptr = cpy_avx512f_2unroll;

	return;
}

void alderlake_set_params(void)
{
	curr_cpu.karatsuba_mul_n_threshold = 33;
	curr_cpu.karatsuba_mul_threshold = 33;

	curr_cpu.apn_add_n_ptr = add_n_8unroll;
	curr_cpu.apn_sub_n_ptr = sub_n_8unroll;
	curr_cpu.apn_mul_bc_ptr = mul_bc_mulx_adx_8unroll;
	curr_cpu.apn_neg_ptr = neg_8unroll;
	curr_cpu.apn_set_ptr = set_avx_4unroll;
	curr_cpu.apn_cpy_ptr = cpy_avx_4unroll;

	return;
}