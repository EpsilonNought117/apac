#include "../headers/hidden_mul.h"

void apn_toomcook32_mul(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size1,
	ap_size_t size2,
	ap_dig_t* temp
)
{
	APAC_ASSERT(temp != NULL);
	APAC_ASSERT(size1 >= size2);

	// Heuristic obtained from proof + trial-and-error
	bool is_toomcook32_valid = (size2 + 2 <= size1) && (size1 + 6 <= 3 * size2);

	if (size1 < KARATSUBA_MUL_THRESHOLD)
	{
		apn_basecase_mul(result, op1, op2, size1, size2);
	}
	else if (!is_toomcook32_valid)
	{
		apn_mul_dispatcher(result, op1, op2, size1, size2, temp);
	}
	else
	{
		APAC_ASSERT((size2 + 2 <= size1) && (size1 + 8 <= 3 * size2));

		ap_size_t split = size1 * 2 >= size2 * 3 ? (size1 + 2) / 3 : (size2 + 1) / 2;

		ap_size_t b1_size = size2 - split;
		ap_size_t a2_size = size1 - split * 2;

		APAC_ASSERT((a2_size > 0) && (a2_size <= split));
		APAC_ASSERT((b1_size > 0) && (b1_size <= split));
		APAC_ASSERT((a2_size + b1_size) >= split);

		const ap_dig_t* b0 = op2, * b1 = &op2[split];
		const ap_dig_t* a0 = op1, * a1 = &op1[split], * a2 = &op1[2 * split];

		ap_dig_t* a_mid = &temp[0];

		temp[split] += apn_add(a_mid, a0, a2, split, a2_size);
		ap_dig_t* a_pos = &temp[2 * (split + 1)];
		apn_cpy(a_pos, a_mid, split + 1);

		/*
			lower = split

					|- a0 + a2 -|   empty   |- a0 + a2 -|   empty   |
			temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |
		*/

		ap_dig_t* a_neg = a_mid;
		ap_dig_t borrow1 = apn_sub(a_neg, a_neg, a1, split + 1, split);
		if (borrow1) { apn_neg(a_neg, a_neg, split + 1); }

		temp[3 * split + 2] += apn_add_n(a_pos, a_pos, a1, split);

		/*
			lower = split

					|-- a_neg --|   empty   |-- a_pos --|   empty   |
			temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |
		*/

		ap_dig_t* b_pos = &temp[3 * (split + 1)];
		temp[4 * split + 3] += apn_add(b_pos, b0, b1, split, b1_size);

		ap_dig_t* b_neg = &temp[split + 1];
		ap_dig_t borrow2 = apn_sub(b_neg, b0, b1, split, b1_size);
		if (borrow2)
		{
			apn_neg(b_neg, b_neg, split);
		}

		/*
			lower = split

					|-- a_neg --|-- b_neg --|-- a_pos --|-- b_pos --|
			temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |
		*/

		ap_dig_t* w_neg = result;
		apn_toomcook3_mul(w_neg, a_neg, b_neg, split + 1, split + 1, &temp[4 * (split + 1)]);

		apn_cpy(temp, w_neg, 2 * (split + 1));
		w_neg = temp;
		apn_set(result, 2 * (split + 1), 0);

		ap_dig_t* w_pos = result;
		apn_toomcook3_mul(w_pos, a_pos, b_pos, split + 1, split + 1, &temp[4 * (split + 1)]);

		apn_cpy(&temp[2 * (split + 1)], w_pos, 2 * (split + 1));
		w_pos = &temp[2 * (split + 1)];
		apn_set(result, 2 * (split + 1), 0);

		/*
			lower = split

					|-------- w_neg --------|-------- w_pos --------|
			temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |
		*/

		apn_toomcook3_mul(result, a0, b0, split, split, &temp[4 * (split + 1)]);

		if (a2_size >= b1_size)
		{
			apn_toomcook32_mul(&result[3 * split], a2, b1, a2_size, b1_size, &temp[4 * (split + 1)]);
		}
		else
		{
			apn_toomcook32_mul(&result[3 * split], b1, a2, b1_size, a2_size, &temp[4 * (split + 1)]);
		}

		// All 4 recursive products done
		// Now to interpolate the w_pos and w_neg, before recomibining
		// to recover r1 and r2

		APAC_ASSERT(apn_clamp(w_pos, 2 * (split + 1)) <= 2 * split + 1);
		APAC_ASSERT(apn_clamp(w_neg, 2 * (split + 1)) <= 2 * split + 1);

		if (borrow1 == borrow2)
		{
			apn_add_n(w_neg, w_pos, w_neg, 2 * split + 1);
		}
		else
		{
			apn_sub_n(w_neg, w_pos, w_neg, 2 * split + 1);
		}

		ap_dig_t out = apn_rshift(w_neg, w_neg, 2 * split + 2, 1);
		APAC_ASSERT(out == 0);

		apn_sub_n(w_pos, w_pos, w_neg, 2 * split + 1);

		ap_dig_t* w1 = w_pos, * w2 = w_neg, * w0 = result, * w3 = &result[split * 3];

		out = apn_sub(w1, w1, w3, 2 * split + 1, b1_size + a2_size);
		APAC_ASSERT(out == 0);

		out = apn_sub(w2, w2, w0, 2 * split + 1, split * 2);
		APAC_ASSERT(out == 0);

		/*
			recomposition phase
		*/

		ap_size_t temp_size1 = split + a2_size + b1_size;
		ap_size_t temp_size2 = temp_size1 > 2 * split + 1 ? temp_size1 : 2 * split + 1;

		apn_add(&result[split], &result[split], w1, 2 * split + a2_size + b1_size, 2 * split + 1);
		apn_add(&result[2 * split], &result[2 * split], w2, split + a2_size + b1_size, temp_size2);

		apn_set(temp, 4 * (split + 1), 0);
	}

	return;
}