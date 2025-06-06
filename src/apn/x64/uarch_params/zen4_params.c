#include "../../../../include/apac.h"
#include "../x64_hidden_funcs.h"
#include "../x64_uarch_params.h"

extern __apac_cpu_params curr_cpu;

void zen4_set_params(void)
{
	curr_cpu.karatsuba_mul_n_threshold = 50;
	curr_cpu.toom33_mul_n_threshold = 160;

	curr_cpu.__apn_add_n_ptr = _add_n_4unroll;
	curr_cpu.__apn_sub_n_ptr = _sub_n_4unroll;
	curr_cpu.__apn_mul_bc_ptr = _mul_bc_mulx_adx_4unroll;
	curr_cpu.__apn_neg_ptr = _neg_4unroll;
	curr_cpu.__apn_set_ptr = _set_avx512f_4unroll;
	curr_cpu.__apn_cpy_ptr = _cpy_avx512f_4unroll;

	return;
}