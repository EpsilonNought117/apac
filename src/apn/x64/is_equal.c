#include "../../../include/apac.h"

#if defined(__GNUC__)
__attribute__((target("avx512f,bmi2")))
#endif
int is_equal_avx512f_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
)
{
	apn_size_t blocks = size & ((apn_size_t)(-32));
	apn_size_t counter = 0;
	__mmask8 a0 = 0, a1 = 0, a2 = 0, a3 = 0;

	while (counter < blocks)
	{
		a0 = _mm512_cmpneq_epu64_mask(
			_mm512_loadu_si512(&op1[counter]),
			_mm512_loadu_si512(&op2[counter])
		);

		a1 = _mm512_cmpneq_epu64_mask(
			_mm512_loadu_si512(&op1[counter + 8]),
			_mm512_loadu_si512(&op2[counter + 8])
		);
		
		a2 = _mm512_cmpneq_epu64_mask(
			_mm512_loadu_si512(&op1[counter + 16]),
			_mm512_loadu_si512(&op2[counter + 16])
		);
		
		a3 = _mm512_cmpneq_epu64_mask(
			_mm512_loadu_si512(&op1[counter + 24]),
			_mm512_loadu_si512(&op2[counter + 24])
		);

		a0 |= a1;
		a2 |= a3;
		a0 |= a2;

		if (a0)
			return 1;

		counter += 32;
	}

	apn_size_t remaining = size - counter;
	apn_size_t limb_mask = _bzhi_u64(~0ULL, remaining);

	a0 = _mm512_cmpneq_epu64_mask(
		_mm512_maskz_loadu_epi64((limb_mask), &op1[counter]),
		_mm512_maskz_loadu_epi64((limb_mask), &op2[counter])
	);

	a1 = _mm512_cmpneq_epu64_mask(
		_mm512_maskz_loadu_epi64((limb_mask >> 8), &op1[counter + 8]),
		_mm512_maskz_loadu_epi64((limb_mask >> 8), &op2[counter + 8])
	);

	a2 = _mm512_cmpneq_epu64_mask(
		_mm512_maskz_loadu_epi64((limb_mask >> 16), &op1[counter + 16]),
		_mm512_maskz_loadu_epi64((limb_mask >> 16), &op2[counter + 16])
	);

	a3 = _mm512_cmpneq_epu64_mask(
		_mm512_maskz_loadu_epi64((limb_mask >> 24), &op1[counter + 24]),
		_mm512_maskz_loadu_epi64((limb_mask >> 24), &op2[counter + 24])
	);

	a0 |= a1;
	a2 |= a3;
	a0 |= a2;

	return (a0 ? 1 : 0);
}

#if defined(__GNUC__)
__attribute__((target("avx2")))
#endif
int is_equal_avx2_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
)
{
	apn_size_t blocks = size & ((apn_size_t)(-16));
	apn_size_t counter = 0;
	
	while (counter < blocks)
	{
		__m256i a0 = _mm256_xor_epi64(
			_mm256_loadu_si256((const __m256i*) & op1[counter]),
			_mm256_loadu_si256((const __m256i*) & op2[counter])
		);

		__m256i a1 = _mm256_xor_epi64(
			_mm256_loadu_si256((const __m256i*) & op1[counter + 4]),
			_mm256_loadu_si256((const __m256i*) & op2[counter + 4])
		);

		__m256i a2 = _mm256_xor_epi64(
			_mm256_loadu_si256((const __m256i*) & op1[counter + 8]),
			_mm256_loadu_si256((const __m256i*) & op2[counter + 8])
		);

		__m256i a3 = _mm256_xor_epi64(
			_mm256_loadu_si256((const __m256i*) & op1[counter + 12]),
			_mm256_loadu_si256((const __m256i*) & op2[counter + 12])
		);

		a0 = _mm256_or_si256(a0, a1);
		a2 = _mm256_or_si256(a2, a3);
		a0 = _mm256_or_si256(a0, a2);

		int zf = _mm256_testz_si256(a0, a0);

		if (!zf)
			return 1;

		counter += 16;
	}

	while (counter < size)
	{
		if (op1[counter] != op2[counter])
			return 1;

		counter++;
	}

	return 0;
}

#if defined(__GNUC__)
__attribute__((target("sse2")))
#endif
int is_equal_sse2_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
)
{
	apn_size_t blocks = size & ((apn_size_t)(-8));
	apn_size_t counter = 0;

	while (counter < blocks)
	{
		__m128i a0 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[counter]),
			_mm_loadu_si128((const __m128i*) & op2[counter])
		);

		__m128i a1 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[counter + 2]),
			_mm_loadu_si128((const __m128i*) & op2[counter + 2])
		);

		__m128i a2 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[counter + 4]),
			_mm_loadu_si128((const __m128i*) & op2[counter + 4])
		);

		__m128i a3 = _mm_xor_si128(
			_mm_loadu_si128((const __m128i*) & op1[counter + 6]),
			_mm_loadu_si128((const __m128i*) & op2[counter + 6])
		);

		a0 = _mm_or_si128(a0, a1);
		a2 = _mm_or_si128(a2, a3);
		a0 = _mm_or_si128(a0, a2);

		int zf = _mm_testz_si128(a0, a0);

		if (!zf)
			return 1;

		counter += 8;
	}

	while (counter < size)
	{
		if (op1[counter] != op2[counter])
			return 1;

		counter++;
	}

	return 0;
}
