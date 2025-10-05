#include "../../../include/apac.h"
#include "apn_thresholds.h"
#include "hidden_mul.h"

#define KARATSUBA_MUL_BALANCED_WS_SIZE(size)	\
		(size * 2 + 64) // scratch workspace size of karatsuba balanced
#define KARATSUBA_MUL_UNBALANCED_WS_SIZE(size1, size2)	\
		(size1 * 2 + 64) // scratch workspace size of unbalanced karatsuba

void apn_mul_n(
	apn_seg* result, 
	const apn_seg* op1, 
	const apn_seg* op2, 
	apn_size size
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(result != op2);
	APAC_ASSERT(size != 0);

	// zero out result before mul
	apn_set(result, 2 * size, 0);

	if (size < KARATSUBA_MUL_BALANCED_THRESHOLD)
	{
		apn_basecase_mul(result, op1, op2, size, size);
	}
	else
	{
		APAC_ASSERT(apac_malloc != NULL && apac_free != NULL);

		apn_size ws_size = KARATSUBA_MUL_BALANCED_WS_SIZE(size);
		apn_seg* workspace = apac_malloc(sizeof(apn_seg) * ws_size);

		APAC_ALWAYS_ASSERT(workspace != NULL);
		
		apn_set(workspace, ws_size, 0);

		apn_karatsuba_mul_balanced(result, op1, op2, size, workspace);
		apac_free(workspace);
	}

	return;
}

void apn_mul(
	apn_seg* result, 
	const apn_seg* op1, 
	const apn_seg* op2, 
	apn_size size1, 
	apn_size size2
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(result != op2);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);

	// zero out result before mul
	apn_set(result, size1 + size2, 0);

	if (size2 <= ((size1 + 1) >> 1) || size1 < KARATSUBA_MUL_UNBALANCED_THRESHOLD)
	{
		apn_basecase_mul(result, op1, op2, size1, size2);
	}
	else
	{
		APAC_ASSERT(apac_malloc != NULL && apac_free != NULL);

		apn_size ws_size = KARATSUBA_MUL_UNBALANCED_WS_SIZE(size1, size2);
		apn_seg* workspace = apac_malloc(sizeof(apn_seg) * ws_size);

		APAC_ALWAYS_ASSERT(workspace != NULL);

		apn_set(workspace, ws_size, 0);
		
		apn_karatsuba_mul_unbalanced(result, op1, op2, size1, size2, workspace);
		apac_free(workspace);
	}

	return;
}