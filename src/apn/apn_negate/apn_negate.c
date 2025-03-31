#include "../../../include/apac.h"

static void _avx2_apn_negate_4unroll(u64* op1, u64 size)
{
    __m256i mask = _mm256_cmpeq_epi64(_mm256_setzero_si256(), _mm256_setzero_si256());

    u64 remainder = size & 15;
    u64 blocks = size >> 4;
    u64 counter = 0;

    for (u64 i = 0; i < blocks; i++)
    {
        _mm256_storeu_si256((__m256i*) & op1[counter], _mm256_xor_si256(mask, _mm256_lddqu_si256((__m256i*) & op1[counter])));
        _mm256_storeu_si256((__m256i*) & op1[counter + 4], _mm256_xor_si256(mask, _mm256_lddqu_si256((__m256i*) & op1[counter + 4])));
        _mm256_storeu_si256((__m256i*) & op1[counter + 8], _mm256_xor_si256(mask, _mm256_lddqu_si256((__m256i*) & op1[counter + 8])));
        _mm256_storeu_si256((__m256i*) & op1[counter + 12], _mm256_xor_si256(mask, _mm256_lddqu_si256((__m256i*) & op1[counter + 12])));

        counter += 16;
    }

    for (u64 i = 0; i < remainder; i++)
    {
        op1[counter + i] = ~op1[counter + i];
    }

    // now op1 is one's complement of it's former self
    // add 1 to it to turn it into a 2's complement

    u8 carry = 1;
    u64 i = 0;

    while (carry && i < size)
    {
        carry = _addcarry_u64(carry, 0, op1[i], &op1[i]);
        i++;
    }
}

static void _sse_apn_negate_4unroll(u64* op1, u64 size)
{
	__m128i mask = _mm_cmpeq_epi64(_mm_setzero_si128(), _mm_setzero_si128());

	u64 remainder = size & 7;
	u64 blocks = size >> 3;
	u64 counter = 0;


    for (u64 i = 0; i < blocks; i++)
    {
        _mm_store_si128((__m128i*) & op1[counter], _mm_xor_si128(_mm_load_si128((__m128i*) & op1[counter]), mask));
        _mm_store_si128((__m128i*) & op1[counter + 2], _mm_xor_si128(_mm_load_si128((__m128i*) & op1[counter + 2]), mask));
        _mm_store_si128((__m128i*) & op1[counter + 4], _mm_xor_si128(_mm_load_si128((__m128i*) & op1[counter + 4]), mask));
        _mm_store_si128((__m128i*) & op1[counter + 6], _mm_xor_si128(_mm_load_si128((__m128i*) & op1[counter + 6]), mask));

        counter += 8;
    }

    for (u64 i = 0; i < remainder; i++)
    {
        op1[counter + i] = ~op1[counter + i];
    }

    // now op1 is one's complement of it's former self
    // add 1 to it to turn it into a 2's complement

    u8 carry = 1;
    u64 i = 0;

    while (carry && i < size)
    {
        carry = _addcarry_u64(carry, 0, op1[i], &op1[i]);
        i++;
    }
}

void apn_negate(u64* op1, u64 size)
{
    if (size > 16 && avx2_chk)
    {
        _avx2_apn_negate_4unroll(op1, size);
    }
    else
    {
        _sse_apn_negate_4unroll(op1, size);
    }
}