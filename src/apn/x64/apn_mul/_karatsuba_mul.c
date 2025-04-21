#include "_hidden_mul.h"
#include "../apac_thresholds.h"

void _apn_additive_karatsuba_mul(
	u64* result,
	const u64* op1,
	const u64* op2,
	u64 size,
	u8 depth,
	u64* workspace
)
{
	u8	allocd_here = 0; // variable to indicate whether to free workspace
	// if it was allocated within the function here or if some 
	// higher function gave it needed amount

	if (!depth && !workspace)
	{
		u64 temp_val = size / KARATSUBA_MUL_THRESHOLD;
		uint32_t log_part = 0;

		_BitScanReverse64(&log_part, temp_val);
		log_part++;
		u64 workspace_size = ((u64)log_part * 12 + (size >> 1) * 8);

		workspace = apac_malloc(sizeof(u64) * workspace_size);

		if (!workspace)
		{
			APAC_REPORT_ERR("Memory allocation failed in karatsuba mul!");
			abort();
		}

		allocd_here = 1; // set flag to indicate memory allocated here
		apn_set(workspace, workspace_size, 0);
	}

	if (size < KARATSUBA_MUL_THRESHOLD)
	{
		_apn_basecase_mul(result, op1, op2, size, size);
		return;
	}
	else
	{
		u64 lower = (size + 1) >> 1;
		u64 upper = size >> 1;

		_apn_additive_karatsuba_mul(
			&result[0],
			&op1[0],
			&op2[0],
			lower,
			depth + 1,
			&workspace[0]
		);

		_apn_additive_karatsuba_mul(
			&result[2 * lower],
			&op1[lower],
			&op2[lower],
			upper,
			depth + 1,
			&workspace[0]
		);

		u8 cy1 = apn_add(&workspace[0], &op1[0], &op1[lower], lower, upper);
		workspace[lower] += cy1;

		u8 cy2 = apn_add(&workspace[lower + 1], &op2[0], &op2[lower], lower, upper);
		workspace[2 * lower + 1] += cy2;

		_apn_additive_karatsuba_mul(
			&workspace[2 * (lower + 1)],
			&workspace[0],
			&workspace[lower + 1],
			lower + 1,
			depth + 1,
			&workspace[4 * (lower + 1)]
		);

		apn_sub(
			&workspace[2 * (lower + 1)],
			&workspace[2 * (lower + 1)],
			&result[0],
			2 * (lower + 1),
			2 * lower
		);

		apn_sub(
			&workspace[2 * (lower + 1)],
			&workspace[2 * (lower + 1)],
			&result[2 * lower],
			2 * (lower + 1),
			2 * upper
		);

		apn_add_n(
			&result[lower],
			&result[lower],
			&workspace[2 * (lower + 1)],
			2 * lower + 1
		);

		apn_set(&workspace[0], 4 * (lower + 1), 0);
	}

	if (!depth && allocd_here)
	{
		apac_free(workspace);
		workspace = NULL;
	}

	return;
}