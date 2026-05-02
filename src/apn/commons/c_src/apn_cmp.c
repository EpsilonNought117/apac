#include "../../../../include/apac.h"

int apn_cmp(
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size
)
{
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(size != 0);
	APAC_ASSERT(curr_cpu.apn_cmp_ptr != NULL);

	int ret_val = curr_cpu.apn_cmp_ptr(op1, op2, size);
	return ret_val;
}