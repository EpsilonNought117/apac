#include "../../../include/apac.h"
#include "apn_thresholds.h"
#include "hidden_sqr.h"

#define KARATSUBA_SQR_WS_SIZE(size)		\
		(size * 2 + 64)

void apn_sqr(
	apn_seg* result,
	const apn_seg* op1,
	apn_size size
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(size != 0);

	apn_set(result, 2 * size, 0);

	if (size < KARATSUBA_SQR_THRESHOLD)
	{
		apn_basecase_sqr(result, op1, size);
	}
	else
	{
		if (!apac_malloc || !apac_free)
			apacSetMemFuncs(NULL, NULL, NULL);

		apn_size ws_size = KARATSUBA_SQR_WS_SIZE(size);
		apn_seg* workspace = apac_malloc(sizeof(apn_seg) * ws_size);

		APAC_ALWAYS_ASSERT(workspace != NULL);

		apn_set(workspace, ws_size, 0);

		apn_karatsuba_sqr(result, op1, size, workspace);
		apac_free(workspace);
	}

	return;
}
