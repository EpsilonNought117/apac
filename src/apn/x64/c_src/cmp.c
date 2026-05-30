#include "../x64_hidden_funcs.h"

/*
	Key Idea is to check for equality. 
	If equality fails at any point, test only those digits.
*/

/*
	Check if need for AVX512 variant arises and if so, add it.
*/

#if (defined(__GNUC__) || defined(__clang__))
__attribute__((target("avx2")))
#endif
int cmp_avx2_4unroll(
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size
)
{
	ap_size_t remainder = size % 16;
	ap_size_t blocks = size - remainder;

	while (remainder)
	{
		if (op1[size - 1] != op2[size - 1])
		{
			return (op1[size - 1] > op2[size - 1] ? 1 : -1);
		}

		size--;
		remainder--;
	}

	const __m256i zero = _mm256_setzero_si256();

	while (blocks)
	{
		__m256i a0 = _mm256_xor_si256(
			_mm256_loadu_si256((const __m256i*) & op1[size - 4]),
			_mm256_loadu_si256((const __m256i*) & op2[size - 4])
		);

		__m256i a1 = _mm256_xor_si256(
			_mm256_loadu_si256((const __m256i*) & op1[size - 8]),
			_mm256_loadu_si256((const __m256i*) & op2[size - 8])
		);

		__m256i a2 = _mm256_xor_si256(
			_mm256_loadu_si256((const __m256i*) & op1[size - 12]),
			_mm256_loadu_si256((const __m256i*) & op2[size - 12])
		);

		__m256i a3 = _mm256_xor_si256(
			_mm256_loadu_si256((const __m256i*) & op1[size - 16]),
			_mm256_loadu_si256((const __m256i*) & op2[size - 16])
		);

		__m256i y0 = _mm256_or_si256(a0, a1);
		__m256i y1 = _mm256_or_si256(a2, a3);
		y0 = _mm256_or_si256(y0, y1);

		y0 = _mm256_cmpeq_epi64(y0, zero);
		
		int temp_mask = _mm256_movemask_pd(_mm256_castsi256_pd(y0));

		if (temp_mask != 0xF)
		{
			a0 = _mm256_cmpeq_epi64(a0, zero);
			a1 = _mm256_cmpeq_epi64(a1, zero);
			a2 = _mm256_cmpeq_epi64(a2, zero);
			a3 = _mm256_cmpeq_epi64(a3, zero);

			uint64_t x0 = _mm256_movemask_pd(_mm256_castsi256_pd(a0));
			uint64_t x1 = _mm256_movemask_pd(_mm256_castsi256_pd(a1));
			uint64_t x2 = _mm256_movemask_pd(_mm256_castsi256_pd(a2));
			uint64_t x3 = _mm256_movemask_pd(_mm256_castsi256_pd(a3));

			x0 <<= 4;
			x2 <<= 4;
			x0 |= x1;
			x2 |= x3;
			x0 = ((x0 << 8) | x2);

			x0 <<= 48;
			x0 = ~x0;

			uint32_t num_lz = 0;
			CLZ(x0, num_lz);
			APAC_ASSERT((num_lz < 16) && (num_lz >= 0));

			return (op1[size - num_lz - 1] > op2[size - num_lz - 1] ? 1 : -1);
		}

		size -= 16;
		blocks -= 16;
	}

	return 0;
}

#if (defined(__GNUC__) || defined(__clang__))
__attribute__((target("sse2")))
#endif
int cmp_sse2_4unroll(
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size
)
{
	ap_size_t remainder = size % 8;
	ap_size_t blocks = size - remainder;

	while (remainder)
	{
		if (op1[size - 1] != op2[size - 1])
		{
			return (op1[size - 1] > op2[size - 1] ? 1 : -1);
		}

		size--;
		remainder--;
	}

	const __m128d zero = _mm_castsi128_pd(_mm_setzero_si128());

	while (blocks)
	{
		__m128i a0 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[size - 2]),
			_mm_loadu_si128((const __m128i*) & op2[size - 2])
		);

		__m128i a1 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[size - 4]),
			_mm_loadu_si128((const __m128i*) & op2[size - 4])
		);

		__m128i a2 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[size - 6]),
			_mm_loadu_si128((const __m128i*) & op2[size - 6])
		);
		
		__m128i a3 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[size - 8]),
			_mm_loadu_si128((const __m128i*) & op2[size - 8])
		);

		__m128d y0 = _mm_castsi128_pd(_mm_or_si128(a0, a1));
		__m128d y1 = _mm_castsi128_pd(_mm_or_si128(a2, a3));

		y0 = _mm_or_pd(y0, y1);
		y0 = _mm_cmpeq_pd(y0, zero);

		int temp_mask = _mm_movemask_pd(y0);

		if (temp_mask != 0x3)
		{
			__m128d x0 = _mm_cmpeq_pd(_mm_castsi128_pd(a0), zero);
			__m128d x1 = _mm_cmpeq_pd(_mm_castsi128_pd(a1), zero);
			__m128d x2 = _mm_cmpeq_pd(_mm_castsi128_pd(a2), zero);
			__m128d x3 = _mm_cmpeq_pd(_mm_castsi128_pd(a3), zero);

			uint64_t b0 = _mm_movemask_pd(x0);
			uint64_t b1 = _mm_movemask_pd(x1);
			uint64_t b2 = _mm_movemask_pd(x2);
			uint64_t b3 = _mm_movemask_pd(x3);

			b0 <<= 2;
			b2 <<= 2;
			b0 |= b1;
			b2 |= b3;
			b0 = (b0 << 4) | b2;

			b0 <<= 56;
			b0 = ~b0;

			uint32_t num_lz = 0;
			CLZ(b0, num_lz);
			APAC_ASSERT((num_lz < 8) && (num_lz >= 0));

			return (op1[size - num_lz - 1] > op2[size - num_lz - 1] ? 1 : -1);
		}

		blocks -= 8;
		size -= 8;
	}

	return 0;
}