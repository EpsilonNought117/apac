#include "../../../../include/apac.h"

ap_size_t apn_clamp(
	const ap_dig_t* op1,
	ap_size_t size
)
{
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(size != 0);

	while (size != 0 && (op1[size - 1] == 0))
	{
		size--;
	}

	return size;
}