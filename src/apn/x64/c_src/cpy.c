#include "../x64_hidden_funcs.h"

#if (defined(__GNUC__) || defined(__clang__))
__attribute__((target("avx512f,bmi2")))
#endif
void cpy_avx512f_4unroll(
	apn_dig_t* result,
	const apn_dig_t* op1,
	apn_size_t size
)
{
	// first process blocks of 32 limbs
	apn_size_t blocks = size - (size % 32);
	apn_size_t counter = 0;

	while (counter < blocks)
	{
		// no typecasts needed as per function declaration

		_mm512_storeu_si512(&result[counter], _mm512_loadu_si512(&op1[counter]));
		_mm512_storeu_si512(&result[counter + 8], _mm512_loadu_si512(&op1[counter + 8]));
		_mm512_storeu_si512(&result[counter + 16], _mm512_loadu_si512(&op1[counter + 16]));
		_mm512_storeu_si512(&result[counter + 24], _mm512_loadu_si512(&op1[counter + 24]));

		counter += 32;
	}

	apn_size_t remaining = size - counter;
	apn_size_t limb_mask = _bzhi_u64(~0ULL, remaining);

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

#if (defined(__GNUC__) || defined(__clang__))
__attribute__((target("avx")))
#endif
void cpy_avx_4unroll(
	apn_dig_t* result,
	const apn_dig_t* op1,
	apn_size_t size
)
{
	// first process blocks of 16 limbs
	apn_size_t blocks = size - (size % 16);
	apn_size_t counter = 0;

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

#if (defined(__GNUC__) || defined(__clang__))
__attribute__((target("sse2")))
#endif
void cpy_sse2_4unroll(
	apn_dig_t* result,
	const apn_dig_t* op1,
	apn_size_t size
)
{
	// first process blocks of 8 limbs
	apn_size_t blocks = size - (size % 8);
	apn_size_t counter = 0;

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