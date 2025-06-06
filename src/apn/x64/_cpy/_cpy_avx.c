#include "../../../../include/apac.h"

void _cpy_avx_4unroll(u64* result, const u64* op1, u64 size)
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