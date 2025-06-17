#include "../../../include/apac.h"
#include "apn_thresholds.h"
#include "_hidden_mul.h"

#define KARATSUBA_MUL_N_WS_SIZE(size)		((u64)size * 2 + 64) // scratch workspace size of karatsuba balanced
#define KARATSUBA_MUL_WS_SIZE(size1, size2) ((u64)size1 * 2 + 64) // scratch workspace size of unbalanced karatsuba

void apn_mul_n(u64* result, const u64* op1, const u64* op2, u64 size)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(result != op1);
	APAC_ASSERT(result != op2);
	APAC_ASSERT(size != 0);

	// zero out result before mul
	apn_set(result, 2 * size, 0);

	if (size < KARATSUBA_MUL_N_THRESHOLD)
	{
		_apn_basecase_mul(result, op1, op2, size, size);
	}
	else
	{
		u64 ws_size = KARATSUBA_MUL_N_WS_SIZE(size);
		u64* workspace = apac_malloc(sizeof(u64) * ws_size);

		if (!workspace)
		{
			APAC_REPORT_ERR("Memory allocation failed in apn_mul_n!");
			abort();
		}

		apn_set(workspace, ws_size, 0);

		_apn_karatsuba_mul_n(result, op1, op2, size, workspace);
		apac_free(workspace);
	}

	return;
}

void apn_mul(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
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

	if (size2 <= ((size1 + 1) >> 1) || size1 < KARATSUBA_MUL_THRESHOLD)
	{
		_apn_basecase_mul(result, op1, op2, size1, size2);
	}
	else
	{
		u64 ws_size = KARATSUBA_MUL_WS_SIZE(size1, size2);
		u64* workspace = apac_malloc(sizeof(u64) * ws_size);

		if (!workspace)
		{
			APAC_REPORT_ERR("Memory allocation failed in apn_mul!");
			abort();
		}

		apn_set(workspace, ws_size, 0);
		
		_apn_karatsuba_mul(result, op1, op2, size1, size2, workspace);
		apac_free(workspace);
	}

	return;
}