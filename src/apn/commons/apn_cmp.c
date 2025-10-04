#include "../../../include/apac.h"

// Nothing fancy needed here
// Avoid using this in performance critical stuff

int apn_cmp(
	const apn_seg* op1,
	const apn_seg* op2,
	apn_size size
)
{
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(size != 0);

	while (size != 0)
	{
		if (op1[size - 1] != op2[size - 1])
			goto not_equal;

		size--;
	}

	return 0;

not_equal:
	return (op1[size - 1] > op2[size - 1] ? 1 : -1);
}