#include "../../../include/apac.h"
#include "../commons/hidden_helpers.h" // for CLZ64 macro

/*
	Key Idea is to check for equality. 
	If equality fails at any point, test only those limbs.
*/

#if defined(__GNUC__)
__attribute__((target("avx2")))
#endif
int cmp_avx2_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
)
{
	apn_size_t blocks = size & ((apn_seg_t)-16);
	apn_size_t remainder = size & ((apn_seg_t)15);

	while (remainder)
	{
		if (op1[size - 1] != op2[size - 1])
			goto not_equal_scalar;

		size--;
		remainder--;
	}

	__m256i a0, a1, a2, a3, y0, y1;
	const __m256i zero = _mm256_setzero_si256();

	while (blocks)
	{
		a0 = _mm256_xor_si256(
			_mm256_loadu_si256((const __m256i*) & op1[size - 4]),
			_mm256_loadu_si256((const __m256i*) & op2[size - 4])
		);

		a1 = _mm256_xor_si256(
			_mm256_loadu_si256((const __m256i*) & op1[size - 8]),
			_mm256_loadu_si256((const __m256i*) & op2[size - 8])
		);

		a2 = _mm256_xor_si256(
			_mm256_loadu_si256((const __m256i*) & op1[size - 12]),
			_mm256_loadu_si256((const __m256i*) & op2[size - 12])
		);

		a3 = _mm256_xor_si256(
			_mm256_loadu_si256((const __m256i*) & op1[size - 16]),
			_mm256_loadu_si256((const __m256i*) & op2[size - 16])
		);

		a0 = _mm256_cmpeq_epi64(a0, zero);
		a1 = _mm256_cmpeq_epi64(a1, zero);
		a2 = _mm256_cmpeq_epi64(a2, zero);
		a3 = _mm256_cmpeq_epi64(a3, zero);
		
		y0 = _mm256_and_si256(a0, a1);
		y1 = _mm256_and_si256(a2, a3);
		y0 = _mm256_and_si256(y0, y1);

		int temp_mask = _mm256_movemask_pd(_mm256_castsi256_pd(y0));

		if (temp_mask != 0xF)
			goto not_equal_simd;

		size -= 16;
		blocks -= 16;
	}

	return 0;

not_equal_scalar:

	return (op1[size - 1] > op2[size - 1] ? 1 : -1);

not_equal_simd:

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

	int num_lz = 0;
	CLZ64(num_lz, x0);
	APAC_ASSERT((num_lz < 16) && (num_lz >= 0));

	return (op1[size - num_lz - 1] > op2[size - num_lz - 1] ? 1 : -1);
}

#if defined(__GNUC__)
__attribute__((target("sse2")))
#endif
int cmp_sse2_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
)
{
	apn_size_t blocks = size & ((apn_seg_t)-8);
	apn_size_t remainder = size & ((apn_seg_t)7);

	while (remainder)
	{
		if (op1[size - 1] != op2[size - 1])
			goto not_equal_scalar;

		size--;
		remainder--;
	}

	const __m128d zero = _mm_castsi128_pd(_mm_setzero_si128());
	__m128i a0, a1, a2, a3;
	__m128d x0, x1, x2, x3;
	__m128d y0, y1;

	while (blocks)
	{
		a0 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[size - 2]),
			_mm_loadu_si128((const __m128i*) & op2[size - 2])
		);

		a1 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[size - 4]),
			_mm_loadu_si128((const __m128i*) & op2[size - 4])
		);

		a2 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[size - 6]),
			_mm_loadu_si128((const __m128i*) & op2[size - 6])
		);
		
		a3 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[size - 8]),
			_mm_loadu_si128((const __m128i*) & op2[size - 8])
		);

		x0 = _mm_cmpeq_pd(_mm_castsi128_pd(a0), zero);
		x1 = _mm_cmpeq_pd(_mm_castsi128_pd(a1), zero);
		x2 = _mm_cmpeq_pd(_mm_castsi128_pd(a2), zero);
		x3 = _mm_cmpeq_pd(_mm_castsi128_pd(a3), zero);

		y0 = _mm_and_pd(x0, x1);
		y1 = _mm_and_pd(x2, x3);
		y0 = _mm_and_pd(y0, y1);

		int temp_mask = _mm_movemask_pd(y0);

		if (temp_mask != 0x3)
			goto not_equal_simd;

		blocks -= 8;
		size -= 8;
	}

	return 0;

not_equal_scalar:

		return (op1[size - 1] > op2[size - 1] ? 1 : -1);

not_equal_simd:

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

	int num_lz = 0;
	CLZ64(num_lz, b0);
	APAC_ASSERT((num_lz < 8) && (num_lz >= 0));
	
	return (op1[size - num_lz - 1] > op2[size - num_lz - 1] ? 1 : -1);
}