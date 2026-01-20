#include "../../../include/apac.h"
#include "arm64_hidden_funcs.h"

extern apac_cpu_params curr_cpu;

void generic_arm64_set_params(void)
{
	/* random cut-off thresholds */
	curr_cpu.karatsuba_mul_balanced_threshold   = (apn_size_t)(20);
	curr_cpu.karatsuba_mul_unbalanced_threshold = (apn_size_t)(30);
	curr_cpu.karatsuba_sqr_threshold            = (apn_size_t)(40);
	curr_cpu.dnc_div_threshold                  = (apn_size_t)(50);

	curr_cpu.apn_add_n_ptr          = add_n_arm64;
	curr_cpu.apn_sub_n_ptr          = sub_n_arm64;
	curr_cpu.apn_add_one_ptr        = add_one_arm64;
	curr_cpu.apn_sub_one_ptr        = sub_one_arm64;
	curr_cpu.apn_addmul_one_ptr     = addmul_one_arm64;
	curr_cpu.apn_submul_one_ptr     = submul_one_arm64;
	curr_cpu.apn_lshift_lt64_ptr    = lshift_lt64_arm64;
	curr_cpu.apn_rshift_lt64_ptr    = rshift_lt64_arm64;
	curr_cpu.apn_mul_bc_ptr         = mul_bc_arm64;
	curr_cpu.apn_sqr_bc_ptr         = sqr_bc_arm64;
	curr_cpu.apn_neg_ptr            = neg_arm64;
	curr_cpu.apn_set_ptr            = set_asimd_4unroll;
	curr_cpu.apn_cpy_ptr            = cpy_asimd_4unroll;
	curr_cpu.apn_cmp_ptr            = cmp_asimd_4unroll;
	curr_cpu.apn_is_zero_ptr        = is_zero_asimd_4unroll;

	return;
}