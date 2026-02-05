#include "../../../../include/apac.h"

#if defined(__GNUC__)
__attribute__((target("avx512f,bmi2")))
#endif
void set_avx512f_4unroll(
	apn_seg_t* result,
	apn_size_t size,
	apn_seg_t val
)
{
	__m512i my_val = _mm512_set1_epi64(val);

	apn_size_t blocks = size - (size % 32);
	apn_size_t counter = 0;

	while (counter < blocks)
	{
		_mm512_storeu_epi64(&result[counter], my_val);
		_mm512_storeu_epi64(&result[counter + 8], my_val);
		_mm512_storeu_epi64(&result[counter + 16], my_val);
		_mm512_storeu_epi64(&result[counter + 24], my_val);
		counter += 32;
	}

	apn_size_t remaining = size - counter;
	apn_size_t limb_mask = _bzhi_u64(~0ULL, remaining);

	_mm512_mask_storeu_epi64(&result[counter], (unsigned char)(limb_mask), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 8], (unsigned char)(limb_mask >> 8), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 16], (unsigned char)(limb_mask >> 16), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 24], (unsigned char)(limb_mask >> 24), my_val);

	return;
}

#if defined(__GNUC__)
__attribute__((target("avx")))
#endif
void set_avx_4unroll(
	apn_seg_t* result,
	apn_size_t size,
	apn_seg_t val
)
{
	__m256i my_val = _mm256_set1_epi64x(val);

	apn_size_t blocks = size - (size % 16);
	apn_size_t counter = 0;

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

#if defined(__GNUC__)
__attribute__((target("sse2")))
#endif
void set_sse2_4unroll(
	apn_seg_t* result,
	apn_size_t size,
	apn_seg_t val
)
{
	__m128i my_val = _mm_set1_epi64x(val);

	apn_size_t blocks = size - (size % 8);
	apn_size_t counter = 0;

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