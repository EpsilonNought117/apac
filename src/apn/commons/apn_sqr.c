#include "../../../include/apac.h"
#include "apn_thresholds.h"
#include "hidden_sqr.h"

#define KARATSUBA_SQR_WS_SIZE(size)		\
		(size * 2 + 64)

void apn_sqr(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
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
		APAC_ASSERT(apac_malloc != NULL && apac_free != NULL);

		apn_size_t ws_size = KARATSUBA_SQR_WS_SIZE(size);
		apn_seg_t* workspace = apac_malloc(sizeof(apn_seg_t) * ws_size);

		APAC_ALWAYS_ASSERT(workspace != NULL);

		apn_set(workspace, ws_size, 0);

		apn_karatsuba_sqr(result, op1, size, workspace);
		apac_free(workspace);
	}

	return;
}
