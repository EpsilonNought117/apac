#include "../../../../include/apac.h"

static void _avx512f_apn_negate_4unroll(u64* result, const u64* op1, u64 size)
{
    __m512i mask = _mm512_set1_epi64(-1);

    u64 blocks = size & ((u64)-32);
    u64 counter = 0;

    while (counter < blocks)
    {
        _mm512_storeu_epi64(&result[counter], _mm512_xor_epi32(mask, _mm512_loadu_epi64(&op1[counter])));
        _mm512_storeu_epi64(&result[counter + 8], _mm512_xor_epi32(mask, _mm512_loadu_epi64(&op1[counter + 8])));
        _mm512_storeu_epi64(&result[counter + 16], _mm512_xor_epi32(mask, _mm512_loadu_epi64(&op1[counter + 16])));
        _mm512_storeu_epi64(&result[counter + 32], _mm512_xor_epi32(mask, _mm512_loadu_epi64(&op1[counter + 32])));

        counter += 32;
    }

    while (counter < size)
    {
        result[counter] = ~op1[counter];
        counter++;
    }

    // now result is one's compliment of op1
    // add 1 to make two's complement

    apn_add_one(result, result, size, 1);   // discard in case carry produced (only one case)
    return;
}

static void _avx2_apn_negate_4unroll(u64* result, const u64* op1, u64 size)
{
    __m256i mask = _mm256_cmpeq_epi64(_mm256_setzero_si256(), _mm256_setzero_si256());

    u64 blocks = size & ((u64)(-16)); 
    u64 counter = 0;

    while (counter < blocks)
    {
        _mm256_storeu_si256(
            (__m256i*)(&result[counter]),
            _mm256_xor_si256(mask, _mm256_lddqu_si256((const __m256i*)(&op1[counter])))
        );
        _mm256_storeu_si256(
            (__m256i*)(&result[counter + 4]),
            _mm256_xor_si256(mask, _mm256_lddqu_si256((const __m256i*)(&op1[counter + 4])))
        );
        _mm256_storeu_si256(
            (__m256i*)(&result[counter + 8]),
            _mm256_xor_si256(mask, _mm256_lddqu_si256((const __m256i*)(&op1[counter + 8])))
        );
        _mm256_storeu_si256(
            (__m256i*)(&result[counter + 12]),
            _mm256_xor_si256(mask, _mm256_lddqu_si256((const __m256i*)(&op1[counter + 12])))
        );

        counter += 16;
    }

    while (counter < size)
    {
        result[counter] = ~op1[counter];
        counter++;
    }

    // now result is one's compliment of op1
    // add 1 to make two's complement

    apn_add_one(result, result, size, 1);   // discard this carry in case produced
    return;
}

static void _sse2_apn_negate_4unroll(u64* result, const u64* op1, u64 size)
{
    __m128i mask = _mm_set1_epi64x((u64)-1);

    u64 blocks = size & ((u64)(-8));
	u64 counter = 0;

    while (counter < blocks)
    {
        _mm_store_si128(
            (__m128i*) (&result[counter]),
            _mm_xor_si128(mask, _mm_load_si128((const __m128i*)(&op1[counter])))
        );
        _mm_store_si128(
            (__m128i*) (&result[counter + 2]),
            _mm_xor_si128(mask, _mm_load_si128((const __m128i*)(&op1[counter + 2])))
        );
        _mm_store_si128(
            (__m128i*) (&result[counter + 4]),
            _mm_xor_si128(mask, _mm_load_si128((const __m128i*)(&op1[counter + 4])))
        );
        _mm_store_si128(
            (__m128i*) (&result[counter + 6]),
            _mm_xor_si128(mask, _mm_load_si128((const __m128i*)(&op1[counter + 6])))
        );
        counter += 8;
    }

    while (counter < blocks)
    {
        result[counter] = ~op1[counter];
        counter++;
    }

    // now result is one's compliment of op1
    // add 1 to make two's complement

    apn_add_one(result, result, size, 1);   // discard this carry
    return;
}

void (*_apn_negate_ptr)(u64*, const u64*, u64) = NULL;

void apn_negate(u64* result, const u64* op1, u64 size)
{
    APAC_ASSERT(size != 0);
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);

    if (_apn_negate_ptr == NULL)
    {
        if (avx512f_chk)
        {
            _apn_negate_ptr = _avx512f_apn_negate_4unroll;
        }
        else if (avx2_chk)
        {
            _apn_negate_ptr = _avx2_apn_negate_4unroll;
        }
        else
        {
            _apn_negate_ptr = _sse2_apn_negate_4unroll;
        }
    }

    _apn_negate_ptr(result, op1, size);
    return;
}