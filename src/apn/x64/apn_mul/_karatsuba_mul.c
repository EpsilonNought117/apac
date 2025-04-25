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
	u8 allocd_here = 0;
	
	// variable to indicate whether to free workspace
	// if it was allocated within the function here or if some 
	// higher function gave it needed amount

	if (!depth && !workspace)
	{
		u64 temp_val = size / KARATSUBA_MUL_THRESHOLD; 

		// do log2(temp_val) + 1 to determine how many times
		// the functions recurses on the worst skew of the
		// recursion tree

		uint32_t log_part = 0;

		_BitScanReverse64(&log_part, temp_val);
		log_part++;
		u64 workspace_size = ((u64)log_part * 12 + ((size + 1) >> 1) * 8);

		// explained more in the documentations

		workspace = apac_malloc(sizeof(u64) * workspace_size);

		if (!workspace)
		{
			APAC_REPORT_ERR("Memory allocation failed in karatsuba mul!");
			abort();
		}

		allocd_here = 1; // set flag to indicate memory allocated
		apn_set(workspace, workspace_size, 0); // clear out workspace
	}

	if (size < KARATSUBA_MUL_THRESHOLD)
	{
		// for sizes below threshold
		// use the basecase multiplication

		_apn_basecase_mul(result, op1, op2, size, size);
		return;
	}
	else
	{
		u64 lower = (size + 1) >> 1;	// upper half of the operands
		u64 upper = size >> 1;			// lower half of the operands

		// the lower half is at most 1 limb larger than upper half
		// (size + 1) / 2 = floor(size / 2)
		// size / 2 = ceil(size / 2)
		// (a / b) is integer division so it truncates the fraction part

		_apn_additive_karatsuba_mul(
			&result[0],
			&op1[0],
			&op2[0],
			lower,
			depth + 1,
			&workspace[0]
		);	// result[0 : (2 * lower)] = a0 * b0
		// a0 is op1[0 : lower]
		// b0 is op2[0 : lower]
		// this is c0

		_apn_additive_karatsuba_mul(
			&result[2 * lower],
			&op1[lower],
			&op2[lower],
			upper,
			depth + 1,
			&workspace[0]
		); // result[(2 * lower) : (2 * size)] = a1 * b1
		// a1 is op1[(lower + 1) : upper]
		// b1 is op2[(lower + 1) : upper]
		// this is c1

		u8 carry1 = apn_add(&workspace[0], &op1[0], &op1[lower], lower, upper);
		workspace[lower] += carry1;

		// propagate carry if occured

		u8 carry2 = apn_add(&workspace[lower + 1], &op2[0], &op2[lower], lower, upper);
		workspace[2 * lower + 1] += carry2;

		// same as above here

		_apn_additive_karatsuba_mul(
			&workspace[2 * (lower + 1)],
			&workspace[0],
			&workspace[lower + 1],
			lower + 1,
			depth + 1,
			&workspace[4 * (lower + 1)]
		); // workspace[2 * (lower + 1) : 4 * (lower + 1)] = (a1 + a0) * (b1 + b0)
		// (a1 + a0) is workspace[0 : (lower + 1)]
		// (b1 + b0) is workspace[(lower + 1) : 2 * (lower + 1)]

		// c2 = (a1 + a0) * (b1 + b0)

		apn_sub(
			&workspace[2 * (lower + 1)],
			&workspace[2 * (lower + 1)],
			&result[0],
			2 * (lower + 1),
			2 * lower
		); // c2 -= c0

		apn_sub(
			&workspace[2 * (lower + 1)],
			&workspace[2 * (lower + 1)],
			&result[2 * lower],
			2 * (lower + 1),
			2 * upper
		); // c2 -= c1

		// at this point c2 = c2 - c0 - c1

		apn_add_n(
			&result[lower],
			&result[lower],
			&workspace[2 * (lower + 1)],
			2 * lower + 1
		);
		// add c2 to result[lower : 2 * lower + 1]
		
		//   [----- c0 -----]
		// +          [----- c2 -----]
		// + 		    	    [----- c1 -----]
		// -------------------------------------
		// [-------------- result --------------]

		apn_set(&workspace[0], 4 * (lower + 1), 0);
		
		// clear workspace for upcoming recursive calls
	}

	if (!depth && allocd_here)
	{
		// deallocate aux storage if it was
		// allocated within the function itself and
		// not provided by some caller

		apac_free(workspace);
		workspace = NULL;
	}

	return;
}