#include "apn_cpy.h"
#include "../../../misc/getCPUSpec/getCPUSpec.h"
#include <immintrin.h>

static void _avx_apn_cpy_4unroll(u64* result, const u64* op1, u64 size)
{
    u64 blocks = size >> 4;
    u64 remainder = size & 15;
    u64 counter = 0;

    for (u64 i = 0; i < blocks; i++)
    {
        _mm256_storeu_si256((__m256i*)(result + counter), _mm256_loadu_si256((const __m256i*)(op1 + counter)));
        _mm256_storeu_si256((__m256i*)(result + counter + 4), _mm256_loadu_si256((const __m256i*)(op1 + counter + 4)));
        _mm256_storeu_si256((__m256i*)(result + counter + 8), _mm256_loadu_si256((const __m256i*)(op1 + counter + 8)));
        _mm256_storeu_si256((__m256i*)(result + counter + 12), _mm256_loadu_si256((const __m256i*)(op1 + counter + 12)));
        counter += 16;
    }

    for (u64 i = 0; i < remainder; i++)
    {
        result[counter + i] = op1[counter + i];
    }
    return;
}

static void _sse_apn_cpy_4unroll(uint64_t* result, const uint64_t* op1, uint64_t size)
{
    uint64_t blocks = size >> 3;  
    uint64_t remainder = size & 7;
    uint64_t counter = 0;

    for (uint64_t i = 0; i < blocks; i++)
    {

        _mm_storeu_si128((__m128i*)(result + counter), _mm_loadu_si128((const __m128i*)(op1 + counter)));
        _mm_storeu_si128((__m128i*)(result + counter + 2), _mm_loadu_si128((const __m128i*)(op1 + counter + 2)));
        _mm_storeu_si128((__m128i*)(result + counter + 4), _mm_loadu_si128((const __m128i*)(op1 + counter + 4)));
        _mm_storeu_si128((__m128i*)(result + counter + 6), _mm_loadu_si128((const __m128i*)(op1 + counter + 6)));
        counter += 8;
    }

    for (uint64_t i = 0; i < remainder; i++)
    {
        result[counter + i] = op1[counter + i];
    }
    return;
}

void apn_cpy(u64* result, const u64* op1, u64 size)
{
	if (avx_chk)
	{
        _avx_apn_cpy_4unroll(result, op1, size);
	}
	else
	{
        _sse_apn_cpy_4unroll(result, op1, size);
	}
}