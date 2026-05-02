#include "../../../../include/apac.h"
#include "../c_headers/hidden_sqr.h"

void apn_basecase_sqr(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
)
{
	APAC_ASSERT(curr_cpu.apn_sqr_bc_ptr != NULL);

	curr_cpu.apn_sqr_bc_ptr(result, op1, size);
	return;
}