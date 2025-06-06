#include "../../../../include/apac.h"

void _cpy_sse2_4unroll(u64* result, const u64* op1, u64 size)
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