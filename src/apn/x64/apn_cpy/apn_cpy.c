#include "../../../../include/apac.h"

static void _avx512f_apn_cpy_4unroll(u64* result, const u64* op1, u64 size)
{
	u64 blocks = size & ((u64)(-32)); // first process blocks of 32 limbs
	u64 counter = 0;

	while (counter < blocks)
	{
		// no typecasts needed as per function declaration

		_mm512_storeu_epi64(&result[counter], _mm512_loadu_epi64(&op1[counter]));
		_mm512_storeu_epi64(&result[counter + 8], _mm512_loadu_epi64(&op1[counter + 8]));
		_mm512_storeu_epi64(&result[counter + 16],_mm512_loadu_epi64(&op1[counter + 16]));
		_mm512_storeu_epi64(&result[counter + 24], _mm512_loadu_epi64(&op1[counter + 24]));

		counter += 32;
	}

	while (counter < size)
	{
		result[counter] = op1[counter];
		counter++;
	}
}

static void _avx_apn_cpy_4unroll(u64* result, const u64* op1, u64 size)
{
	u64 blocks = size & ((u64)(-16));  // first process blocks of 16 limbs
	u64 counter = 0;

	while (counter < blocks)
	{
		_mm256_storeu_si256(
			(__m256i*)(&result[counter]), 
			_mm256_lddqu_si256((const __m256i*)(&op1[counter]))
		);
		_mm256_storeu_si256(
			(__m256i*)(&result[counter + 4]),
			_mm256_lddqu_si256((const __m256i*)(&op1[counter + 4]))
		);
		_mm256_storeu_si256(
			(__m256i*)(&result[counter + 8]), 
			_mm256_lddqu_si256((const __m256i*)(&op1[counter + 8]))
		);
		_mm256_storeu_si256(
			(__m256i*)(&result[counter + 12]),
			_mm256_lddqu_si256((const __m256i*)(&op1[counter + 12]))
		);
		counter += 16;
	}

	while (counter < size)
	{
		result[counter] = op1[counter];
		counter++;
	}
}

static void _sse_apn_cpy_4unroll(u64* result, const u64* op1, u64 size)
{
	u64 blocks = size & ((u64)(-8));	// first process blocks of 8 limbs
	u64 counter = 0;

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

	while (counter < size)
	{
		result[counter] = op1[counter];
		counter++;
	}
}

static void (*_apn_cpy_ptrs[])(u64*, const u64*, u64) = {
	_sse_apn_cpy_4unroll,
	_avx_apn_cpy_4unroll,
	_avx512f_apn_cpy_4unroll
};

static int _apn_cpy_idx = -1; // invalid index when starting out

void apn_cpy(u64* result, const u64* op1, u64 size)
{
	APAC_ASSERT(size != 0)
	APAC_ASSERT(result != NULL)
	APAC_ASSERT(op1 != NULL)
	APAC_ASSERT(result != op1)

	if (_apn_cpy_idx == -1)
	{
		if (avx512f_chk)
		{
			_apn_cpy_idx = 2;
		}
		else if (avx_chk)
		{
			_apn_cpy_idx = 1;
		}
		else
		{
			_apn_cpy_idx = 0;
		}
	}

	_apn_cpy_ptrs[_apn_cpy_idx](result, op1, size);
	return;
}