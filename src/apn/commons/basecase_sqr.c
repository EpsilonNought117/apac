#include "../../../include/apac.h"
#include "hidden_sqr.h"

void apn_basecase_sqr(
	apn_seg* result,
	const apn_seg* op1,
	apn_size size
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(size != 0);
	
	APAC_ALWAYS_ASSERT(curr_cpu.apn_sqr_bc_ptr != NULL);

	curr_cpu.apn_sqr_bc_ptr(result, op1, size);
	return;
}