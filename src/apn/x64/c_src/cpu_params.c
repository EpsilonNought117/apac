#include "../../../../include/apac.h"
#include "../x64_hidden_funcs.h"

extern apac_cpu_params curr_cpu;

// Tested on Ryzen 7 6800H running at
// Technically this processor is Zen 3+, but it is just a node shrink compared to Zen 3 Cezanne
void zen3_set_params(void)
{
	curr_cpu.karatsuba_mul_balanced_threshold   = (ap_size_t)(27);
	curr_cpu.karatsuba_sqr_threshold            = (ap_size_t)(55);
	curr_cpu.karatsuba_mul_unbalanced_threshold = (ap_size_t)(58);
	curr_cpu.dnc_div_threshold                  = (ap_size_t)(38);

	curr_cpu.apn_add_n_ptr			= add_n_zen4;
	curr_cpu.apn_sub_n_ptr			= sub_n_zen4;
	curr_cpu.apn_add_one_ptr		= add_one_zen4;
	curr_cpu.apn_sub_one_ptr		= sub_one_zen4;
	curr_cpu.apn_addmul_one_ptr		= addmul_one_zen4;
	curr_cpu.apn_submul_one_ptr		= submul_one_zen4;
	curr_cpu.apn_lshift_lt64_ptr	= lshift_lt64_zen4;
	curr_cpu.apn_rshift_lt64_ptr	= rshift_lt64_zen4;
	curr_cpu.apn_mul_bc_ptr			= mul_bc_zen4;
	curr_cpu.apn_sqr_bc_ptr			= sqr_bc_zen4;
	curr_cpu.apn_neg_ptr			= neg_zen4;

	curr_cpu.apn_set_ptr			= set_avx_4unroll;
	curr_cpu.apn_cpy_ptr			= cpy_avx_4unroll;
	curr_cpu.apn_cmp_ptr			= cmp_avx2_4unroll;
	curr_cpu.apn_is_zero_ptr		= is_zero_avx2_4unroll;

	return;
}

// Tested on Ryzen 7 8845HS running at 3.8 GHz Base Clock
void zen4_set_params(void)
{
	curr_cpu.karatsuba_mul_balanced_threshold   = (ap_size_t)(28);
	curr_cpu.karatsuba_sqr_threshold            = (ap_size_t)(52);
	curr_cpu.karatsuba_mul_unbalanced_threshold = (ap_size_t)(50);
	curr_cpu.dnc_div_threshold                  = (ap_size_t)(36);

	curr_cpu.apn_add_n_ptr			= add_n_zen4;
	curr_cpu.apn_sub_n_ptr			= sub_n_zen4;
	curr_cpu.apn_add_one_ptr		= add_one_zen4;
	curr_cpu.apn_sub_one_ptr		= sub_one_zen4;
	curr_cpu.apn_addmul_one_ptr		= addmul_one_zen4;
	curr_cpu.apn_submul_one_ptr		= submul_one_zen4;
	curr_cpu.apn_lshift_lt64_ptr	= lshift_lt64_zen4;
	curr_cpu.apn_rshift_lt64_ptr	= rshift_lt64_zen4;
	curr_cpu.apn_mul_bc_ptr			= mul_bc_zen4;
	curr_cpu.apn_sqr_bc_ptr			= sqr_bc_zen4;
	curr_cpu.apn_neg_ptr			= neg_zen4;

	curr_cpu.apn_set_ptr			= set_avx512f_4unroll;
	curr_cpu.apn_cpy_ptr			= cpy_avx512f_4unroll;
	curr_cpu.apn_cmp_ptr			= cmp_avx2_4unroll;
	curr_cpu.apn_is_zero_ptr		= is_zero_avx512f_4unroll;

	return;
}

// Tested on Ryzen AI 9 HX 370 running at 1.9 GHz Base Clock
void zen5_set_params(void)
{
	curr_cpu.karatsuba_mul_balanced_threshold	= (ap_size_t)(26);
	curr_cpu.karatsuba_sqr_threshold			= (ap_size_t)(45);
	curr_cpu.karatsuba_mul_unbalanced_threshold = (ap_size_t)(44);
	curr_cpu.dnc_div_threshold					= (ap_size_t)(34);

	curr_cpu.apn_add_n_ptr			= add_n_zen4;
	curr_cpu.apn_sub_n_ptr			= sub_n_zen4;
	curr_cpu.apn_add_one_ptr		= add_one_zen4;
	curr_cpu.apn_sub_one_ptr		= sub_one_zen4;
	curr_cpu.apn_addmul_one_ptr		= addmul_one_zen4;
	curr_cpu.apn_submul_one_ptr		= submul_one_zen4;
	curr_cpu.apn_lshift_lt64_ptr	= lshift_lt64_zen4;
	curr_cpu.apn_rshift_lt64_ptr	= rshift_lt64_zen4;
	curr_cpu.apn_mul_bc_ptr			= mul_bc_zen4;
	curr_cpu.apn_sqr_bc_ptr			= sqr_bc_zen4;
	curr_cpu.apn_neg_ptr			= neg_zen4;

	curr_cpu.apn_set_ptr			= set_avx512f_4unroll;
	curr_cpu.apn_cpy_ptr			= cpy_avx512f_4unroll;
	curr_cpu.apn_cmp_ptr			= cmp_avx2_4unroll;
	curr_cpu.apn_is_zero_ptr		= is_zero_avx512f_4unroll;

	return;
}

void generic_x64_set_params(void)
{
	/* random cut-off thresholds */
	curr_cpu.karatsuba_mul_balanced_threshold   = (ap_size_t)(20);
	curr_cpu.karatsuba_mul_unbalanced_threshold = (ap_size_t)(30);
	curr_cpu.karatsuba_sqr_threshold            = (ap_size_t)(40);
	curr_cpu.dnc_div_threshold                  = (ap_size_t)(50);

	curr_cpu.apn_add_n_ptr          = add_n_x64;
	curr_cpu.apn_sub_n_ptr          = sub_n_x64;
	curr_cpu.apn_add_one_ptr        = add_one_x64;
	curr_cpu.apn_sub_one_ptr        = sub_one_x64;
	curr_cpu.apn_addmul_one_ptr     = addmul_one_x64;
	curr_cpu.apn_submul_one_ptr     = submul_one_x64;
	curr_cpu.apn_lshift_lt64_ptr    = lshift_lt64_x64;
	curr_cpu.apn_rshift_lt64_ptr    = rshift_lt64_x64;
	curr_cpu.apn_mul_bc_ptr         = mul_bc_x64;
	curr_cpu.apn_sqr_bc_ptr         = sqr_bc_x64;
	curr_cpu.apn_neg_ptr            = neg_x64;
	curr_cpu.apn_set_ptr            = set_sse2_4unroll;
	curr_cpu.apn_cpy_ptr            = cpy_sse2_4unroll;
	curr_cpu.apn_cmp_ptr            = cmp_sse2_4unroll;
	curr_cpu.apn_is_zero_ptr        = is_zero_sse2_4unroll;

	return;
}