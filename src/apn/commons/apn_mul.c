#include "../headers/hidden_mul.h"

/**
 * Scratch workspace size upper bound for karatsuba multiplication
 * 
 * The constant APN_DIG_BITS here is the number of times the function recurses before hitting 
 * basecase. In reality you never recurse even 8 times before you hit basecase multiplication,
 * and you are likely way outside the size ranges for which karatsuba mul is fastest.
 * 
 * At each level, 4 * ((curr_size + 1) / 2) space is needed, with no 
 * extra space for carries as a subtractive karatsuba variant is used.
 * 
 * The "infinite" geometric sum caps out at 2*n + 2*(log_2(n)), where we assume log_2(n)
 * to be at most APN_DIG_BITS.
 */
#define KARATSUBA_MUL_WS_SIZE(size1, size2)	(((size1) + APN_DIG_BITS) * 2)

/**
 * Scratch workspace size upper bound for toomcook3 multiplication
 * 
 * The constant APN_DIG_BITS here is the number of times the function recurses before reaching karatsuba mul.
 * The constant (2 * 3) is the extra space needed to account for uneven splits which can be
 * 2 greater than top most split piece along with accounting for carry-outs.
 * 
 * At each level, 6 * (((curr_size + 2) / 3) + 1) scratch space is needed.
 * 
 * The "infinite" geometric sum caps out at 3*n + 18*(log_3(n)), so log_3(n) being assumed 
 * to be APN_DIG_BITS at max before the range of toom3 exhausts, results in a safe upper bound.
 */
#define TOOMCOOK3_MUL_WS_SIZE(size1, size2)	(((size1) + 2 * 3 * APN_DIG_BITS) * 3)

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
	else if (size < TOOMCOOK3_MUL_THRESHOLD)
	{
		APAC_ASSERT(apac_malloc != NULL && apac_free != NULL);

		ap_size_t ws_size = KARATSUBA_MUL_WS_SIZE(size, size);
		ap_dig_t* workspace = apac_malloc(sizeof(ap_dig_t) * ws_size);

		if (!workspace)
		{
			APAC_LOG_ERR("Memory allocation failed in apn_mul_n!");
			return APAC_OOM;
		}
		
		apn_set(workspace, ws_size, 0);

		apn_karatsuba_mul(result, op1, op2, size, size, workspace);
		apac_free(workspace);	// free temporary workspace
	}
	else
	{
		APAC_ASSERT(apac_malloc != NULL && apac_free != NULL);

		ap_size_t ws_size = TOOMCOOK3_MUL_WS_SIZE(size, size);
		ap_dig_t* workspace = apac_malloc(sizeof(ap_dig_t) * ws_size);
	
		if (!workspace)
		{
			APAC_LOG_ERR("Memory allocation failed in apn_mul_n!");
			return APAC_OOM;
		}

		apn_set(workspace, ws_size, 0);

		apn_toomcook3_mul(result, op1, op2, size, size, workspace);
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
	else if (size1 < KARATSUBA_MUL_THRESHOLD)
	{
		apn_basecase_mul(result, op1, op2, size1, size2);
	}
	else
	{
		APAC_ASSERT(apac_malloc != NULL && apac_free != NULL);

		ap_size_t ws_size = TOOMCOOK3_MUL_WS_SIZE(size1, size2);
		ap_dig_t* workspace = apac_malloc(sizeof(ap_dig_t) * ws_size);

		if (!workspace)
		{
			APAC_LOG_ERR("Memory allocation failed in apn_mul_n!");
			return APAC_OOM;
		}

		apn_set(workspace, ws_size, 0);

		apn_mul_dispatcher(result, op1, op2, size1, size2, workspace);
		apac_free(workspace);	// free temporary workspace
	}

	return APAC_OK;
}