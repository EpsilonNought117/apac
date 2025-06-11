#include "../../../include/apac.h"

void _set_avx512f_4unroll(u64* result, u64 size, u64 val)
{
	__m512i my_val = _mm512_set1_epi64((i64)val);

	u64 blocks = size & ((u64)-32);
	u64 counter = 0;

	while (counter < blocks)
	{
		_mm512_storeu_epi64(&result[counter], my_val);
		_mm512_storeu_epi64(&result[counter + 8], my_val);
		_mm512_storeu_epi64(&result[counter + 16], my_val);
		_mm512_storeu_epi64(&result[counter + 24], my_val);
		counter += 32;
	}

	u64 remaining = size - counter;
	u64 limb_mask = _bzhi_u64(~0ULL, remaining);

	_mm512_mask_storeu_epi64(&result[counter], (u8)(limb_mask), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 8], (u8)(limb_mask >> 8), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 16], (u8)(limb_mask >> 16), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 24], (u8)(limb_mask >> 24), my_val);

	return;
}

void _set_avx512f_2unroll(u64* result, u64 size, u64 val)
{
	__m512i my_val = _mm512_set1_epi64((i64)val);

	u64 blocks = size & ((u64)-16);
	u64 counter = 0;

	while (counter < blocks)
	{
		_mm512_storeu_epi64(&result[counter], my_val);
		_mm512_storeu_epi64(&result[counter + 8], my_val);

		counter += 16;
	}

	u64 remaining = size - counter;
	u64 limb_mask = _bzhi_u64(~0ULL, remaining);

	_mm512_mask_storeu_epi64(&result[counter], (u8)(limb_mask), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 8], (u8)(limb_mask >> 8), my_val);

	return;
}

void _set_avx_4unroll(u64* result, u64 size, u64 val)
{
	__m256i my_val = _mm256_set1_epi64x((i64)val);

	u64 blocks = size & ((u64)-16);
	u64 counter = 0;

	while (counter < blocks)
	{
		_mm256_storeu_si256((__m256i*)(&result[counter]), my_val);
		_mm256_storeu_si256((__m256i*)(&result[counter + 4]), my_val);
		_mm256_storeu_si256((__m256i*)(&result[counter + 8]), my_val);
		_mm256_storeu_si256((__m256i*)(&result[counter + 12]), my_val);

		counter += 16;
	}

	while (counter < size)
	{
		result[counter] = val;
		counter++;
	}

	return;
}

void _set_sse2_4unroll(u64* result, u64 size, u64 val)
{
	__m128i my_val = _mm_set1_epi64x((i64)val);

	u64 blocks = size & ((u64)-8);
	u64 counter = 0;

	while (counter < blocks)
	{
		_mm_storeu_si128((__m128i*)(&result[counter]), my_val);
		_mm_storeu_si128((__m128i*)(&result[counter + 2]), my_val);
		_mm_storeu_si128((__m128i*)(&result[counter + 4]), my_val);
		_mm_storeu_si128((__m128i*)(&result[counter + 6]), my_val);

		counter += 8;
	}

	while (counter < size)
	{
		result[counter] = val;
		counter++;
	}

	return;
}
