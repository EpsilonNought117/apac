#include "../headers/hidden_sqr.h"

#define KARATSUBA_SQR_WS_SIZE(size)	(2 * (size + APN_DIG_BITS))
#define TOOMCOOK3_SQR_WS_SIZE(size)	(((size) + 2 * 3 * APN_DIG_BITS) * 3)

apac_err apn_sqr(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(size != 0);
	APAC_NO_OVERLAP(result, size * 2, op1, size);
	
	apn_set(result, 2 * size, 0);

	if (size < KARATSUBA_SQR_THRESHOLD)
	{
		apn_basecase_sqr(result, op1, size);
	}
	else if (size < TOOMCOOK3_SQR_THRESHOLD)
	{

		APAC_ASSERT(apac_allocator.custom_malloc != NULL && apac_allocator.custom_free != NULL);

		ap_size_t ws_size = KARATSUBA_SQR_WS_SIZE(size);
		ap_dig_t* workspace = apac_allocator.custom_malloc(sizeof(ap_dig_t) * ws_size, apac_allocator.ctx);

		if (!workspace)
		{
			APAC_LOG_ERR("Memory allocation failed in apn_sqr!");
			return APAC_OOM;
		}

		apn_set(workspace, ws_size, 0);

		apn_karatsuba_sqr(result, op1, size, workspace);
		apac_allocator.custom_free(workspace, apac_allocator.ctx);
	}
	else
	{
		APAC_ASSERT(apac_allocator.custom_malloc != NULL && apac_allocator.custom_free != NULL);

		ap_size_t ws_size = TOOMCOOK3_SQR_WS_SIZE(size);
		ap_dig_t* workspace = apac_allocator.custom_malloc(sizeof(ap_dig_t) * ws_size, apac_allocator.ctx);
	
		if (!workspace)
		{
			APAC_LOG_ERR("Memory allocation failed in apn_mul_n!");
			return APAC_OOM;
		}

		apn_set(workspace, ws_size, 0);

		apn_toomcook3_sqr(result, op1, size, workspace);
		apac_allocator.custom_free(workspace, apac_allocator.ctx);	// free temporary workspace
	}

	return APAC_OK;
}
