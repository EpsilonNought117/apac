#include "../../../../include/apac.h"

void _cpy_avx512f_4unroll(u64* result, const u64* op1, u64 size)
{
	u64 blocks = size & ((u64)(-32)); // first process blocks of 32 limbs
	u64 counter = 0;

	while (counter < blocks)
	{
		// no typecasts needed as per function declaration

		_mm512_storeu_epi64(&result[counter], _mm512_loadu_epi64(&op1[counter]));
		_mm512_storeu_epi64(&result[counter + 8], _mm512_loadu_epi64(&op1[counter + 8]));
		_mm512_storeu_epi64(&result[counter + 16], _mm512_loadu_epi64(&op1[counter + 16]));
		_mm512_storeu_epi64(&result[counter + 24], _mm512_loadu_epi64(&op1[counter + 24]));

		counter += 32;
	}

	u64 remaining = size - counter;
	u64 limb_mask = _bzhi_u64(~0ULL, remaining);

	_mm512_mask_storeu_epi64(
		&result[counter],
		(limb_mask),
		_mm512_maskz_loadu_epi64((limb_mask), &op1[counter])
	);
	_mm512_mask_storeu_epi64(
		&result[counter + 8],
		(limb_mask >> 8),
		_mm512_maskz_loadu_epi64((limb_mask >> 8), &op1[counter + 8])
	);
	_mm512_mask_storeu_epi64(
		&result[counter + 16],
		(limb_mask >> 16),
		_mm512_maskz_loadu_epi64((limb_mask >> 16), &op1[counter + 16])
	);
	_mm512_mask_storeu_epi64(
		&result[counter + 24],
		(limb_mask >> 24),
		_mm512_maskz_loadu_epi64((limb_mask >> 24), &op1[counter + 24])
	);

	return;
}

void _cpy_avx512f_2unroll(u64* result, const u64* op1, u64 size)
{
	u64 blocks = size & ((u64)(-16)); // first process blocks of 16 limbs
	u64 counter = 0;

	while (counter < blocks)
	{
		// no typecasts needed as per function declaration

		_mm512_storeu_epi64(&result[counter], _mm512_loadu_epi64(&op1[counter]));
		_mm512_storeu_epi64(&result[counter + 8], _mm512_loadu_epi64(&op1[counter + 8]));

		counter += 16;
	}

	u64 remaining = size - counter;
	u64 limb_mask = _bzhi_u64(~0ULL, remaining);

	_mm512_mask_storeu_epi64(
		&result[counter],
		(limb_mask),
		_mm512_maskz_loadu_epi64((limb_mask), &op1[counter])
	);
	_mm512_mask_storeu_epi64(
		&result[counter + 8],
		(limb_mask >> 8),
		_mm512_maskz_loadu_epi64((limb_mask >> 8), &op1[counter + 8])
	);

	return;
}