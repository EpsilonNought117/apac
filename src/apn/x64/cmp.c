#include "../../../include/apac.h"

/*
	The key idea is to test equality of limbs using SIMD intrinsics.
	If the equality test returns false, we can goto to the section to test for which of the limbs the 
	discrepancy lies in and then return the appropriate result from the function.
*/

/*
    _MM_TERNLOG_A = 0xF0 
    _MM_TERNLOG_B = 0xCC 
    _MM_TERNLOG_C = 0xAA
*/

int cmp_avx512f_4unroll(const apn_seg* op1, const apn_seg* op2, apn_size size)
{
    __mmask32 k = 0;
	apn_size blocks = size & ((apn_size)-32);
	apn_size counter = 0;

    __m512i acc = _mm512_setzero_si512();  // Accumulate mismatches

    while (counter < blocks)
    {
        __m512i a0 = _mm512_loadu_si512(&op1[counter]);
        __m512i b0 = _mm512_loadu_si512(&op2[counter]);

        __m512i a1 = _mm512_loadu_si512(&op1[counter + 8]);
        __m512i b1 = _mm512_loadu_si512(&op2[counter + 8]);

        __m512i a2 = _mm512_loadu_si512(&op1[counter + 16]);
        __m512i b2 = _mm512_loadu_si512(&op2[counter + 16]);

        __m512i a3 = _mm512_loadu_si512(&op1[counter + 24]);
        __m512i b3 = _mm512_loadu_si512(&op2[counter + 24]);

        // ri |= (ai ^ bi) fused using ternary logic
        __m512i r0 = _mm512_xor_si512(a0, b0);
        __m512i r1 = _mm512_ternarylogic_epi64(r0, a1, b1, 0xF6); // r0 | (a1 ^ b1)
        __m512i r2 = _mm512_ternarylogic_epi64(r1, a2, b2, 0xF6); // r1 | (a2 ^ b2)
        __m512i r3 = _mm512_ternarylogic_epi64(r2, a3, b3, 0xF6); // r2 | (a3 ^ b3)

        // acc |= r3
        acc = _mm512_or_epi64(acc, r3);

        counter += 32;
    }



    return 0;
}

int cmp_sse2_4unroll(const apn_seg* op1, const apn_seg* op2, apn_size size)
{

}