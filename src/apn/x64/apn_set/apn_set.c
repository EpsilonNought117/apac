#include "../../../../include/apac.h"

static void _avx512f_apn_set_4unroll(u64* result, u64 size, u64 val)
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

	while (counter < size)
	{
		result[counter] = val;
		counter++;
	}
}

static void _avx_apn_set_4unroll(u64* result, u64 size, u64 val)
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
}

static void _sse2_apn_set_4unroll(u64* result, u64 size, u64 val)
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
}

void (*_apn_set_ptr)(u64*, u64, u64) = NULL;

void apn_set(u64* result, u64 size, u64 val)
{
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(size != 0);

	if (_apn_set_ptr == NULL)
	{
		if (avx512f_chk)
		{
			_apn_set_ptr = _avx512f_apn_set_4unroll;
		}
		else if (avx_chk)
		{
			_apn_set_ptr = _avx_apn_set_4unroll;
		}
		else
		{
			_apn_set_ptr = _sse2_apn_set_4unroll;
		}
	}

	_apn_set_ptr(result, size, val);
	return;
}