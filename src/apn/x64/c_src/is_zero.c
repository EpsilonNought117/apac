#include "../../headers/apac_internal.h"

#if (defined(__GNUC__) || defined(__clang__))
__attribute__((target("avx512f,bmi2")))
#endif
int is_zero_avx512f_4unroll(
    const ap_dig_t* op1,
    ap_size_t size
)
{
    ap_size_t blocks = size - (size % 32);
    ap_size_t counter = 0;
    __m512i zero_reg = _mm512_setzero_si512();

    while (counter < blocks)
    {
        __m512i a0 = _mm512_or_si512(
            _mm512_loadu_si512(&op1[counter]),
            zero_reg
        );

        __m512i a1 = _mm512_or_si512(
            _mm512_loadu_si512(&op1[counter + 8]),
            zero_reg
        );

        __m512i a2 = _mm512_or_si512(
            _mm512_loadu_si512(&op1[counter + 16]),
            zero_reg
        );

        __m512i a3 = _mm512_or_si512(
            _mm512_loadu_si512(&op1[counter + 24]),
            zero_reg
        );

        a0 = _mm512_or_si512(a0, a1);
        a2 = _mm512_or_si512(a2, a3);
        a0 = _mm512_or_si512(a0, a2);

        __mmask8 result = _mm512_test_epi64_mask(a0, a0);

        if (result)
            return 1;

        counter += 32;
    }

    ap_size_t remaining = size - counter;
    ap_size_t limb_mask = _bzhi_u64(~0ULL, remaining);

    __m512i a0 = _mm512_or_si512(
        _mm512_maskz_loadu_epi64((limb_mask), &op1[counter]),
        zero_reg
    );

    __m512i a1 = _mm512_or_si512(
        _mm512_maskz_loadu_epi64((limb_mask >> 8), &op1[counter + 8]),
        zero_reg
    );

    __m512i a2 = _mm512_or_si512(
        _mm512_maskz_loadu_epi64((limb_mask >> 16), &op1[counter + 16]),
        zero_reg
    );
    
    __m512i a3 = _mm512_or_si512(
        _mm512_maskz_loadu_epi64((limb_mask >> 24), &op1[counter + 24]),
        zero_reg
    );

    a0 = _mm512_or_si512(a0, a1);
    a2 = _mm512_or_si512(a2, a3);
    a0 = _mm512_or_si512(a0, a2);

    __mmask8 result = _mm512_test_epi64_mask(a0, a0);

    return (result ? 1 : 0);
}

#if (defined(__GNUC__) || defined(__clang__))
__attribute__((target("avx2")))
#endif
int is_zero_avx2_4unroll(
    const ap_dig_t* op1,
    ap_size_t size
)
{
    ap_size_t blocks = size - (size % 16);
    ap_size_t counter = 0;
    __m256i zero_reg = _mm256_setzero_si256();

    while (counter < blocks)
    {
        __m256i a0 = _mm256_or_si256(
            _mm256_loadu_si256((const __m256i*) & op1[counter]),
            zero_reg
        );

        __m256i a1 = _mm256_or_si256(
            _mm256_loadu_si256((const __m256i*) & op1[counter + 4]),
            zero_reg
        );

        __m256i a2 = _mm256_or_si256(
            _mm256_loadu_si256((const __m256i*) & op1[counter + 8]),
            zero_reg
        );

        __m256i a3 = _mm256_or_si256(
            _mm256_loadu_si256((const __m256i*) & op1[counter + 12]),
            zero_reg
        );
        
        a0 = _mm256_or_si256(a0, a1);
        a2 = _mm256_or_si256(a2, a3);
        a0 = _mm256_or_si256(a0, a2);

        __mmask8 result = _mm256_testz_si256(a0, a0);

        if (!result)
            return 1;

        counter += 16;
    }

    while (counter < size)
    {
        if (op1[counter] != 0ULL)
            return 1;

        counter++;
    }

    return 0;
}

#if (defined(__GNUC__) || defined(__clang__))
__attribute__((target("sse2")))
#endif
int is_zero_sse2_4unroll(
    const ap_dig_t* op1,
    ap_size_t size
)
{
    ap_size_t blocks = size - (size % 8);
    ap_size_t counter = 0;
    __m128i zero_reg = _mm_setzero_si128();

    while (counter < blocks)
    {
        __m128i a0 = _mm_or_si128(
            _mm_loadu_si128((const __m128i*) & op1[counter]),
            zero_reg
        );

        __m128i a1 = _mm_or_si128(
            _mm_loadu_si128((const __m128i*) & op1[counter + 2]),
            zero_reg
        );

        __m128i a2 = _mm_or_si128(
            _mm_loadu_si128((const __m128i*) & op1[counter + 4]),
            zero_reg
        );

        __m128i a3 = _mm_or_si128(
            _mm_loadu_si128((const __m128i*) & op1[counter + 6]),
            zero_reg
        );

        // Combine
        a0 = _mm_or_si128(a0, a1);
        a2 = _mm_or_si128(a2, a3);
        a0 = _mm_or_si128(a0, a2);

        // Test if all zeros
        __m128i val = _mm_cmpeq_epi8(a0, zero_reg);
        int result = _mm_movemask_epi8(val);

        if (result != 0xFFFF)
            return 1;

        counter += 8;
    }

    // Handle remaining elements
    while (counter < size)
    {
        if (op1[counter] != 0ULL)
            return 1;

        counter++;
    }

    return 0;
}