#include "../../../../include/apac.h"

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