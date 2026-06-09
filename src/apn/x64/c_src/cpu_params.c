#include "../x64_hidden_funcs.h"

// Tested on Ryzen 7 8845HS running at 3.8 GHz Base Clock
void zen4_set_params(void)
{
	KARATSUBA_MUL_THRESHOLD  = (ap_size_t)(30);
	KARATSUBA_SQR_THRESHOLD  = (ap_size_t)(56);
	DNC_DIV_THRESHOLD        = (ap_size_t)(32);

	curr_cpu.apn_add_n_ptr			= add_n_zen4;
	curr_cpu.apn_sub_n_ptr			= sub_n_zen4;
	curr_cpu.apn_add_one_ptr		= add_one_zen4;
	curr_cpu.apn_sub_one_ptr		= sub_one_zen4;
	curr_cpu.apn_addmul_one_ptr		= addmul_one_zen4;
	curr_cpu.apn_submul_one_ptr		= submul_one_zen4;
	curr_cpu.apn_lshift_ptr			= lshift_lt64_zen4;
	curr_cpu.apn_rshift_ptr			= rshift_lt64_zen4;
	curr_cpu.apn_mul_bc_ptr			= mul_bc_zen4;
	curr_cpu.apn_sqr_bc_ptr			= sqr_bc_zen4;
	curr_cpu.apn_neg_ptr			= neg_zen4;
	curr_cpu.apn_mul_one_ptr		= mul_one_zen4;

	curr_cpu.apn_set_ptr			= set_avx512f_4unroll;
	curr_cpu.apn_cpy_ptr			= cpy_avx512f_4unroll;
	curr_cpu.apn_cmp_ptr			= cmp_avx2_4unroll;
	curr_cpu.apn_is_zero_ptr		= is_zero_avx512f_4unroll;

	return;
}

void generic_x64_set_params(void)
{
	/* random cut-off thresholds */
	KARATSUBA_MUL_THRESHOLD  = (ap_size_t)(20);
	KARATSUBA_SQR_THRESHOLD  = (ap_size_t)(30);
	DNC_DIV_THRESHOLD        = (ap_size_t)(30);

	curr_cpu.apn_add_n_ptr          = add_n_x64;
	curr_cpu.apn_sub_n_ptr          = sub_n_x64;
	curr_cpu.apn_add_one_ptr        = add_one_x64;
	curr_cpu.apn_sub_one_ptr        = sub_one_x64;
	curr_cpu.apn_addmul_one_ptr     = addmul_one_x64;
	curr_cpu.apn_submul_one_ptr     = submul_one_x64;
	curr_cpu.apn_lshift_ptr    		= lshift_lt64_x64;
	curr_cpu.apn_rshift_ptr    		= rshift_lt64_x64;
	curr_cpu.apn_mul_bc_ptr         = mul_bc_x64;
	curr_cpu.apn_sqr_bc_ptr         = sqr_bc_x64;
	curr_cpu.apn_neg_ptr            = neg_x64;
	curr_cpu.apn_mul_one_ptr		= mul_one_x64;
	
	curr_cpu.apn_set_ptr            = set_sse2_4unroll;
	curr_cpu.apn_cpy_ptr            = cpy_sse2_4unroll;
	curr_cpu.apn_cmp_ptr            = cmp_sse2_4unroll;
	curr_cpu.apn_is_zero_ptr        = is_zero_sse2_4unroll;

	return;
}