#include "../../header/apac_internal.h"

void apn_basecase_sqr(
	apn_dig_t* result,
	const apn_dig_t* op1,
	apn_size_t size
)
{
	APAC_ASSERT(apn_sqr_bc_ptr != NULL);

	apn_sqr_bc_ptr(result, op1, size);
	return;
}