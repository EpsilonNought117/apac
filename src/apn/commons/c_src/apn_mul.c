#include "../../../../include/apac.h"
#include "../c_headers/apn_thresholds.h"
#include "../c_headers/hidden_mul.h"

// scratch workspace size of balanced karatsuba
#define KARATSUBA_MUL_WS_SIZE(size) (((size) + 16) * 2)

apac_err apn_mul_n(
	ap_dig_t* result, 
	const ap_dig_t* op1, 
	const ap_dig_t* op2, 
	ap_size_t size
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(size != 0);
	APAC_NO_OVERLAP(result, size * 2, op1, size);
	APAC_NO_OVERLAP(result, size * 2, op2, size);

	// zero out result before mul
	apn_set(result, 2 * size, 0);

	if (size < KARATSUBA_MUL_THRESHOLD)
	{
		apn_basecase_mul(result, op1, op2, size, size);
	}
	else
	{
		APAC_ASSERT(apac_malloc != NULL && apac_free != NULL);

		ap_size_t ws_size = KARATSUBA_MUL_WS_SIZE(size);
		ap_dig_t* workspace = apac_malloc(sizeof(ap_dig_t) * ws_size);

		if (!workspace)
		{
			APAC_LOG_ERR("Memory allocation failed in apn_mul_n!");
			return APAC_OOM;
		}
		
		apn_set(workspace, ws_size, 0);

		apn_karatsuba_mul(result, op1, op2, size, workspace);
		apac_free(workspace);	// free temporary workspace
	}

	return APAC_OK;
}

apac_err apn_mul(
	ap_dig_t* result, 
	const ap_dig_t* op1, 
	const ap_dig_t* op2, 
	ap_size_t size1, 
	ap_size_t size2
)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_NO_OVERLAP(result, size1 + size2, op1, size1);
	APAC_NO_OVERLAP(result, size1 + size2, op2, size2);

	// zero out result before mul
	apn_set(result, size1 + size2, 0);

	if (size2 == 1)
	{
		ap_dig_t carry = apn_addmul_one(result, op1, size1, op2[0]);
		APAC_ASSERT(carry == 0);
	}
	else if (size1 == size2)
	{
		apac_err ret_val = apn_mul_n(result, op1, op2, size1);
		return ret_val;
	}
	else if (size1 < KARATSUBA_MUL_THRESHOLD)
	{
		apn_basecase_mul(result, op1, op2, size1, size2);
	}
	else
	{
		
	}

	return APAC_OK;
}