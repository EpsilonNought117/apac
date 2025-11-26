#include "../../../include/apac.h"
#include "apn_thresholds.h"
#include "hidden_mul.h"

#define KARATSUBA_MUL_BALANCED_WS_SIZE(size)			\
		(size * 2 + 64)		// scratch workspace size of balanced karatsuba
#define KARATSUBA_MUL_UNBALANCED_WS_SIZE(size1, size2)	\
		(size1 * 2 + 64)	// scratch workspace size of unbalanced karatsuba

apac_err apn_mul_n(
	apn_seg_t* result, 
	const apn_seg_t* op1, 
	const apn_seg_t* op2, 
	apn_size_t size
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(size != 0);
	APAC_ASSERT((result >= (op1 + size * 8)) || (op1 >= (result + size * 16)));
	APAC_ASSERT((result >= (op2 + size * 8)) || (op2 >= (result + size * 16)));

	// zero out result before mul
	apn_set(result, 2 * size, 0);

	if (size < KARATSUBA_MUL_BALANCED_THRESHOLD)
	{
		apn_basecase_mul(result, op1, op2, size, size);
	}
	else
	{
		APAC_ASSERT(apac_malloc != NULL && apac_free != NULL);

		apn_size_t ws_size = KARATSUBA_MUL_BALANCED_WS_SIZE(size);
		apn_seg_t* workspace = apac_malloc(sizeof(apn_seg_t) * ws_size);

		if (!workspace)
		{
			APAC_LOG_ERR("Memory allocation failed in apn_mul_n!");
			return APAC_OOM;
		}
		
		apn_set(workspace, ws_size, 0);

		apn_karatsuba_mul_balanced(result, op1, op2, size, workspace);
		apac_free(workspace);
	}

	return APAC_OK;
}

apac_err apn_mul(
	apn_seg_t* result, 
	const apn_seg_t* op1, 
	const apn_seg_t* op2, 
	apn_size_t size1, 
	apn_size_t size2
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT((result >= (op1 + size1 * 8)) || (op1 >= (result + (size1 + size2) * 8)));
	APAC_ASSERT((result >= (op2 + size2 * 8)) || (op2 >= (result + (size1 + size2) * 8)));

	// zero out result before mul
	apn_set(result, size1 + size2, 0);

	if (size2 <= ((size1 + 1) >> 1) || size1 < KARATSUBA_MUL_UNBALANCED_THRESHOLD)
	{
		apn_basecase_mul(result, op1, op2, size1, size2);
	}
	else
	{
		APAC_ASSERT(apac_malloc != NULL && apac_free != NULL);

		apn_size_t ws_size = KARATSUBA_MUL_UNBALANCED_WS_SIZE(size1, size2);
		apn_seg_t* workspace = apac_malloc(sizeof(apn_seg_t) * ws_size);

		if (!workspace)
		{
			APAC_LOG_ERR("Memory allocation failed in apn_mul!");
			return APAC_OOM;
		}

		apn_set(workspace, ws_size, 0);
		
		apn_karatsuba_mul_unbalanced(result, op1, op2, size1, size2, workspace);
		apac_free(workspace);
	}

	return APAC_OK;
}