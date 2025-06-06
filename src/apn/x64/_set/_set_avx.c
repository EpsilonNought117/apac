#include "../../../../include/apac.h"

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
