#include "../../../include/apac.h"
#include "x64_hidden_funcs.h"

extern apac_cpu_params curr_cpu;

void zen4_set_params(void)
{
	curr_cpu.karatsuba_mul_balanced_threshold = 28;
	curr_cpu.karatsuba_mul_unbalanced_threshold = 33;
	curr_cpu.karatsuba_sqr_threshold = 58;
	curr_cpu.dnc_div_threshold = 45;

	curr_cpu.apn_add_n_ptr = add_n_zen4;
	curr_cpu.apn_sub_n_ptr = sub_n_zen4;
	curr_cpu.apn_add_one_ptr = add_n_one_zen4;
	curr_cpu.apn_sub_one_ptr = sub_n_one_zen4;
	curr_cpu.apn_addmul_one_ptr = addmul_one_zen4;
	curr_cpu.apn_submul_one_ptr = submul_one_zen4;
	curr_cpu.apn_lshift_lt64_ptr = lshift_lt64_zen4;
	curr_cpu.apn_rshift_lt64_ptr = rshift_lt64_zen4;

	curr_cpu.apn_mul_bc_ptr = mul_bc_zen4;
	curr_cpu.apn_sqr_bc_ptr = sqr_bc_zen4;
	curr_cpu.apn_neg_ptr = neg_zen4;
	curr_cpu.apn_set_ptr = set_avx512f_4unroll;
	curr_cpu.apn_cpy_ptr = cpy_avx512f_4unroll;

	curr_cpu.apn_is_equal_ptr = is_equal_avx512f_4unroll;
	curr_cpu.apn_is_zero_ptr = is_zero_avx512f_4unroll;

	return;
}

void generic_x64_set_params(void)
{
	// random cut-off thresholds xD
	curr_cpu.karatsuba_mul_balanced_threshold = 20;
	curr_cpu.karatsuba_mul_unbalanced_threshold = 30;
	curr_cpu.karatsuba_sqr_threshold = 40;
	curr_cpu.dnc_div_threshold = 50;

	curr_cpu.apn_add_n_ptr = add_n_x64;
	curr_cpu.apn_sub_n_ptr = sub_n_x64;
	curr_cpu.apn_add_one_ptr = add_n_one_x64;
	curr_cpu.apn_sub_one_ptr = sub_n_one_x64;
	curr_cpu.apn_addmul_one_ptr = addmul_one_x64;
	curr_cpu.apn_submul_one_ptr = submul_one_x64;
	curr_cpu.apn_lshift_lt64_ptr = lshift_lt64_x64;
	curr_cpu.apn_rshift_lt64_ptr = rshift_lt64_x64;

	curr_cpu.apn_mul_bc_ptr = mul_bc_x64;
	curr_cpu.apn_sqr_bc_ptr = sqr_bc_x64;
	curr_cpu.apn_neg_ptr = neg_x64;
	curr_cpu.apn_set_ptr = set_sse2_4unroll;
	curr_cpu.apn_cpy_ptr = cpy_sse2_4unroll;

	curr_cpu.apn_is_equal_ptr = is_equal_sse2_4unroll;
	curr_cpu.apn_is_zero_ptr = is_zero_sse2_4unroll;

	return;
}