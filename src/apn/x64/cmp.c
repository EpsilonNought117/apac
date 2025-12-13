#include "../../../include/apac.h"
#include "../commons/hidden_helpers.h" // for CLZ64 macro

/*
	Key Idea is to check for equality. 
	If equality fails at any point, test only those limbs.
*/

#if defined(__GNUC__)
__attribute__((target("avx512f,bmi2")))
#endif
int cmp_avx512f_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
)
{
	apn_size_t blocks = size & ((apn_size_t)-32);
	apn_size_t remainder = size & 31;
	uint32_t idx = 0;

	__mmask8 mask = 0;
	__m512i a0, a1, a2, a3, b0, b1, b2, b3;

	// start checking values from the MSD of the BigInts

	uint64_t limb_mask = _bzhi_u64(~0ULL, remainder);

	a0 = _mm512_maskz_loadu_epi64((__mmask8)(limb_mask >> 24), &op1[size - remainder + 24]);
	b0 = _mm512_maskz_loadu_epi64((__mmask8)(limb_mask >> 24), &op2[size - remainder + 24]);

	a1 = _mm512_maskz_loadu_epi64((__mmask8)(limb_mask >> 16), &op1[size - remainder + 16]);
	b1 = _mm512_maskz_loadu_epi64((__mmask8)(limb_mask >> 16), &op2[size - remainder + 16]);

	a2 = _mm512_maskz_loadu_epi64((__mmask8)(limb_mask >> 8), &op1[size - remainder + 8]);
	b2 = _mm512_maskz_loadu_epi64((__mmask8)(limb_mask >> 8), &op2[size - remainder + 8]);

	a3 = _mm512_maskz_loadu_epi64((__mmask8)(limb_mask), &op1[size - remainder]);
	b3 = _mm512_maskz_loadu_epi64((__mmask8)(limb_mask), &op1[size - remainder]);

	__m512i r0 = _mm512_xor_si512(a0, b0);
	__m512i r1 = _mm512_ternarylogic_epi64(r0, a1, b1, 0xF0 | (0xCC ^ 0xAA)); // r0 | (a1 ^ b1)
	__m512i r2 = _mm512_ternarylogic_epi64(r1, a2, b2, 0xF0 | (0xCC ^ 0xAA));
	__m512i r3 = _mm512_ternarylogic_epi64(r2, a3, b3, 0xF0 | (0xCC ^ 0xAA));

	mask = _mm512_test_epi64_mask(r3, r3);

	if (mask)
		goto not_equal;
	
	// No more need for masked loads/stores, they can be processed in 32-limb chunks
	while (blocks)
	{
		a0 = _mm512_loadu_epi64(&op1[size - remainder - 8]);
		b0 = _mm512_loadu_epi64(&op2[size - remainder - 8]);

		a1 = _mm512_loadu_epi64(&op1[size - remainder - 16]);
		b1 = _mm512_loadu_epi64(&op2[size - remainder - 16]);

		a2 = _mm512_loadu_epi64(&op1[size - remainder - 24]);
		b2 = _mm512_loadu_epi64(&op2[size - remainder - 24]);

		a3 = _mm512_loadu_epi64(&op1[size - remainder - 32]);
		b3 = _mm512_loadu_epi64(&op2[size - remainder - 32]);

		__m512i r0 = _mm512_xor_si512(a0, b0);
		__m512i r1 = _mm512_ternarylogic_epi64(r0, a1, b1, 0xF0 | (0xCC ^ 0xAA)); // r0 | (a1 ^ b1)
		__m512i r2 = _mm512_ternarylogic_epi64(r1, a2, b2, 0xF0 | (0xCC ^ 0xAA));
		__m512i r3 = _mm512_ternarylogic_epi64(r2, a3, b3, 0xF0 | (0xCC ^ 0xAA));

		mask = _mm512_test_epi64_mask(r3, r3);

		if (mask)
			goto not_equal;

		blocks -= 32;
	}

	// if reached here, means equality
	return 0;

not_equal:

	// if reached here, it means in the current values of
	// a0, b0, a1, b1, a2, b2, a3, b3 there is a difference
	// compare all 4 with cmpneq (AVX512F)
	// combine the masks in appropriate order
	// use 

	__mmask8 mask0 = _mm512_cmpneq_epu64_mask(a0, b0);
	__mmask8 mask1 = _mm512_cmpneq_epu64_mask(a1, b1);
	__mmask8 mask2 = _mm512_cmpneq_epu64_mask(a2, b2);
	__mmask8 mask3 = _mm512_cmpneq_epu64_mask(a3, b3);

	uint64_t x0 = (uint64_t)mask0;
	uint64_t x2 = (uint64_t)mask2;

	x0 <<= 8;
	x2 <<= 8;
	x0 |= (uint64_t)mask1;
	x2 |= (uint64_t)mask3;

	x0 = (x0 << 16) | x2;

	CLZ64(idx, x0);
	APAC_ASSERT(idx != -1); // for safety just in case, vanishes in release builds

	idx -= 32;	// as top 32-bits are counted in leading zeros which are not wanted

	return (op1[size - 32 + idx] > op2[size - 32 + idx] ? 1 : -1);
}

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
	const __m256i zero = _mm256_setzero_si256();

	while (remainder)
	{
		if (op1[size - 1] != op2[size - 1])
			goto not_equal;

		size--;
		remainder--;
	}

	uint64_t idx = 0;

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

		a0 = _mm256_cmpeq_epi64(a0, zero);
		a1 = _mm256_cmpeq_epi64(a1, zero);
		a2 = _mm256_cmpeq_epi64(a2, zero);
		a3 = _mm256_cmpeq_epi64(a3, zero);
		
		uint32_t y0 = (uint32_t)_mm256_movemask_pd(_mm256_castsi256_pd(a0));
		uint32_t y1 = (uint32_t)_mm256_movemask_pd(_mm256_castsi256_pd(a1));
		uint32_t y2 = (uint32_t)_mm256_movemask_pd(_mm256_castsi256_pd(a2));
		uint32_t y3 = (uint32_t)_mm256_movemask_pd(_mm256_castsi256_pd(a3));

		y0 <<= 4;
		y2 <<= 4;
		y0 |= y1;
		y2 |= y3;

		idx = ((uint64_t)y0 << 8) | (uint64_t)y2;

		if (idx != 0xFFFFULL)
			goto not_equal;

		idx = 0;
		size -= 16;
		blocks -= 16;
	}

	return 0;

not_equal:

	if (remainder)
		return (op1[size - 1] > op2[size - 1] ? 1 : -1);

	idx <<= 48;
	idx = ~idx;

	int num_lz = 0;
	CLZ64(num_lz, idx);
	APAC_ASSERT((num_lz != -1) && (num_lz <= 15));

	idx = (uint64_t)63 - num_lz;
	idx &= 15;

	return (op1[size - 16 + idx] > op2[size - 16 + idx] ? 1 : -1);
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
	const __m128d zero = _mm_castsi128_pd(_mm_setzero_si128());

	while (remainder)
	{
		if (op1[size - 1] != op2[size - 1])
			goto not_equal;

		size--;
		remainder--;
	}

	uint64_t idx = 0;

	while (blocks)
	{
		__m128i a0 = _mm_xor_si128(
			_mm_load_si128((const __m128i*) & op1[size - 2]),
			_mm_load_si128((const __m128i*) & op2[size - 2])
		);

		__m128i a1 = _mm_xor_si128(
			_mm_load_si128((const __m128i*) & op1[size - 4]),
			_mm_load_si128((const __m128i*) & op2[size - 4])
		);

		__m128i a2 = _mm_xor_si128(
			_mm_load_si128((const __m128i*) & op1[size - 6]),
			_mm_load_si128((const __m128i*) & op2[size - 6])
		);
		
		__m128i a3 = _mm_xor_si128(
			_mm_load_si128((const __m128i*) & op1[size - 8]),
			_mm_load_si128((const __m128i*) & op2[size - 8])
		);

		// if a0 == zero, the all bits in that 64-bit value will be set
		__m128d x0 = _mm_cmpeq_pd(_mm_castsi128_pd(a0), zero);
		__m128d x1 = _mm_cmpeq_pd(_mm_castsi128_pd(a1), zero);
		__m128d x2 = _mm_cmpeq_pd(_mm_castsi128_pd(a2), zero);
		__m128d x3 = _mm_cmpeq_pd(_mm_castsi128_pd(a3), zero);

		// no problem here with signed-ness as in the absolute
		// worst case only the lower-most 2-bits are set
		uint32_t y0 = (uint32_t)_mm_movemask_pd(x0);
		uint32_t y1 = (uint32_t)_mm_movemask_pd(x1);
		uint32_t y2 = (uint32_t)_mm_movemask_pd(x2);
		uint32_t y3 = (uint32_t)_mm_movemask_pd(x3);

		y0 <<= 2;
		y2 <<= 2;
		y0 |= y1;
		y2 |= y3;

		idx = ((uint64_t)y0 << 4) | ((uint64_t)y2);

		// if all the 8-bits are not set, which means that
		// the highest 0-bit indicates the first element of difference
		if (idx != 0xFFULL)
			goto not_equal;

		idx = 0;	// reset
		blocks -= 8;
		size -= 8;
	}

	return 0;

not_equal:

	if (remainder)
		return (op1[size - 1] > op2[size - 1] ? 1 : -1);

	idx <<= 56;	// idx bottom 56-bits now filled with 0's
	idx = ~idx;	// flip the bits so now you can run CLZ64 on it to
	// find the first set bit in top 8-bits which corresponds to 
	// the exact element where the difference occurs

	int num_lz = 0;
	CLZ64(num_lz, idx);
	APAC_ASSERT((num_lz != -1) && (num_lz <= 7));
	
	idx = (uint64_t)63 - num_lz;
	idx &= 7;				// getting the offset

	return (op1[size - 8 + idx] > op2[size - 8 + idx] ? 1 : -1);
}