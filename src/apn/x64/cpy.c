#include "../../../include/apac.h"

// These procedures don't need micro-architecture specific code (usually)
// Generic x64 implementations after checking for the 

void cpy_zen4(apn_seg* result, const apn_seg* op1, apn_size size)
{
	apn_size blocks = size & ((apn_size)(-32)); // first process blocks of 32 limbs
	apn_size counter = 0;

	while (counter < blocks)
	{
		// no typecasts needed as per function declaration

		_mm512_storeu_epi64(&result[counter], _mm512_loadu_epi64(&op1[counter]));
		_mm512_storeu_epi64(&result[counter + 8], _mm512_loadu_epi64(&op1[counter + 8]));
		_mm512_storeu_epi64(&result[counter + 16], _mm512_loadu_epi64(&op1[counter + 16]));
		_mm512_storeu_epi64(&result[counter + 24], _mm512_loadu_epi64(&op1[counter + 24]));

		counter += 32;
	}

	apn_size remaining = size - counter;
	apn_size limb_mask = _bzhi_u64(~0ULL, remaining);

	// handle remaining limbs with masked load/stores
	// instead of a remainder while-loop

	_mm512_mask_storeu_epi64(
		&result[counter],
		(unsigned char)(limb_mask),
		_mm512_maskz_loadu_epi64((limb_mask), &op1[counter])
	);
	_mm512_mask_storeu_epi64(
		&result[counter + 8],
		(unsigned char)(limb_mask >> 8),
		_mm512_maskz_loadu_epi64((limb_mask >> 8), &op1[counter + 8])
	);
	_mm512_mask_storeu_epi64(
		&result[counter + 16],
		(unsigned char)(limb_mask >> 16),
		_mm512_maskz_loadu_epi64((limb_mask >> 16), &op1[counter + 16])
	);
	_mm512_mask_storeu_epi64(
		&result[counter + 24],
		(unsigned char)(limb_mask >> 24),
		_mm512_maskz_loadu_epi64((limb_mask >> 24), &op1[counter + 24])
	);

	return;
}

// lowest common generic implementation that works on any x64 processor

void cpy_x64(apn_seg* result, const apn_seg* op1, apn_size size)
{
	apn_size blocks = size & ((apn_size)(-8));	// first process blocks of 8 limbs
	apn_size counter = 0;

	while (counter < blocks)
	{
		_mm_storeu_si128(
			(__m128i*)(&result[counter]),
			_mm_loadu_si128((const __m128i*)(&op1[counter]))
		);
		_mm_storeu_si128(
			(__m128i*)(&result[counter + 2]),
			_mm_loadu_si128((const __m128i*)(&op1[counter + 2]))
		);
		_mm_storeu_si128(
			(__m128i*)(&result[counter + 4]),
			_mm_loadu_si128((const __m128i*)(&op1[counter + 4]))
		);
		_mm_storeu_si128(
			(__m128i*)(&result[counter + 6]),
			_mm_loadu_si128((const __m128i*)(&op1[counter + 6]))
		);
		counter += 8;
	}

	// handle remaining limbs with a remainder while-loop

	while (counter < size)
	{
		result[counter] = op1[counter];
		counter++;
	}
}