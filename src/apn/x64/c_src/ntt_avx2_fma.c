#include "../x64_hidden_funcs.h"
#include "../../headers/ntt_primes.h"

#if defined(__GNUC__) || defined(__clang__)
__attribute__((target("avx,fma")))
#endif
static inline __m256d
mul_mod_p51_avx_fma(
    const __m256d a,
    const __m256d b,
    const __m256d p,    /* prime */
    const __m256d p_inv /* prime inverse*/
)
{
    __m256d x = _mm256_mul_pd(a, b);
    __m256d q = _mm256_mul_pd(x, p_inv);
    q = _mm256_round_pd(
        q, 
        (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)
    );
    __m256d y = _mm256_fmsub_pd(a, b, x);
    __m256d z = _mm256_fnmadd_pd(q, p, x);
    return _mm256_add_pd(z, y);
}

#if defined(__GNUC__) || defined(__clang__)
__attribute((target("avx")))
#endif
static inline __m256d
add_mod_p51_avx(
    const __m256d x,
    const __m256d y,
    const __m256d p
)
{
    return _mm256_add_pd(x, y);
}

#if defined(__GNUC__) || defined(__clang__)
__attribute((target("avx")))
#endif
static inline __m256d
sub_mod_p51_avx(
    const __m256d x,
    const __m256d y,
    const __m256d p
)
{
    return _mm256_sub_pd(x, y);
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((target("avx")))
#endif
static inline __m256d
canonicalize_p51_avx(
    __m256d a,
    __m256d p
)
{
    __m256d mask = _mm256_cmp_pd(
        a,
        _mm256_setzero_pd(),
        _CMP_LT_OQ
    );

    return _mm256_add_pd(
        a,
        _mm256_and_pd(mask, p)
    );
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((target("avx,fma")))
#endif
static inline __m256d
normalize_p51_avx_fma(
    __m256d a,
    __m256d p,
    __m256d p_inv
)
{
    __m256d x = _mm256_mul_pd(a, p_inv);

    __m256d q = _mm256_round_pd(
        x,
        _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC
    );

    return _mm256_fnmadd_pd(q, p, a);
}

#if defined(__GNUC__) || defined(__clang__)
__attribute((target("avx2,fma")))
#endif
static void
radix4_dif_fwd_ntt_unroll64(
    double* op1,    /* assumes 64-point transform */
    __m256d vp,
    __m256d vp_inv,
    const ap_dig_t* table
)
{
    double psi1 = (double)table[0];
    double psi2 = (double)table[1];
    double psi4 = (double)table[2];
    double psi8 = (double)table[3];   
    double zeta = (double)table[4]; // psi ^ 16


    __m256d temp1 = _mm256_set1_pd(psi1);
    __m256d temp2 = _mm256_set1_pd(psi2);

    // v(psi3) < 1
    __m256d psi3 = mul_mod_p51_avx_fma(temp2, temp1, vp, vp_inv);

    // v(twiddle) < 1
    __m256d twiddle = _mm256_set_pd(_mm256_cvtsd_f64(psi3), psi2, psi1, 1.0);

    // v(zetav) < 1
    __m256d zetav = _mm256_set1_pd(zeta);

    // v(curr_twiddle) < 1
    __m256d curr_twiddle = twiddle;
    __m256d psi4v = _mm256_set1_pd(psi4);

    /*
        Only need the following valus for now

        1) zetav
        2) psi4v
        3) twiddle
        4) curr_twiddle
    */

    APAC_UNROLL(4)
    for (ap_size_t j = 0; j < 16; j += 4)
    {
        /*
            Growth analysis (v(x) = |x| / p)

            Inputs:                         v(c0), v(c1), v(c2), v(c3) < 1
                                            v(psi3)         < 1
                                            v(zetav)        < 1
                                            v(twiddle)      < 1
                                            v(curr_twiddle) < 1

            d0 = c0 + c2                    => v(d0) < 2
            d1 = c0 - c2                    => v(d1) < 2
            d2 = c1 + c3                    => v(d2) < 2
            d3 = c1 - c3                    => v(d3) < 2

            d3 = mul_mod(d3, zetav)         => v(d3) <= 1/2 + 1/2 * 2 * 1 = 1.5

            d0 = normalize(d0)              => v(d0) < 0.5+

            a0 = d0 + d2                    => v(a0) < 0.5 + 2 = 2.5+
            a1 = d1 + d3                    => v(a1) < 2 + 1.5 = 3.5
            a2 = d0 - d2                    => v(a2) < 0.5 + 2 = 2.5+
            a3 = d1 - d3                    => v(a3) < 2 + 1.5 = 3.5

            a1 = mul_mod(a1, twd1)          => v(a1) <= 1/2 + 1/2 * 3.5 * 1 = 2.25
            a2 = mul_mod(a2, twd2)          => v(a2) <= 1/2 + 1/2 * 2.5 * 1 = 1.75
            a3 = mul_mod(a3, twd3)          => v(a3) <= 1/2 + 1/2 * 3.5 * 1 = 2.25

            Before normalize:               v(a0) < 2.5+ 
                                            v(a1) <= 2.25 
                                            v(a2) <= 1.75 
                                            v(a3) <= 2.25

            normalize(a0,a1,a2,a3)          => centered residues

            Stored outputs:                 v(output) < 0.5+

            Loop invariant restored.
        */

        __m256d c0 = _mm256_loadu_pd(&op1[j]);
        __m256d c1 = _mm256_loadu_pd(&op1[j + 16]);
        __m256d c2 = _mm256_loadu_pd(&op1[j + 32]);
        __m256d c3 = _mm256_loadu_pd(&op1[j + 48]);

        __m256d d0 = add_mod_p51_avx(c0, c2, vp);
        __m256d d1 = sub_mod_p51_avx(c0, c2, vp);
        __m256d d2 = add_mod_p51_avx(c1, c3, vp);
        __m256d d3 = sub_mod_p51_avx(c1, c3, vp);

        d3 = mul_mod_p51_avx_fma(d3, zetav, vp, vp_inv);
        d0 = normalize_p51_avx_fma(d0, vp, vp_inv);

        __m256d a0 = add_mod_p51_avx(d0, d2, vp);

        __m256d a1 = add_mod_p51_avx(d1, d3, vp);
        a1 = mul_mod_p51_avx_fma(a1, curr_twiddle, vp, vp_inv);

        curr_twiddle = mul_mod_p51_avx_fma(curr_twiddle, twiddle, vp, vp_inv);

        __m256d a2 = sub_mod_p51_avx(d0, d2, vp);
        a2 = mul_mod_p51_avx_fma(a2, curr_twiddle, vp, vp_inv);

        curr_twiddle = mul_mod_p51_avx_fma(curr_twiddle, twiddle, vp, vp_inv);

        __m256d a3 = sub_mod_p51_avx(d1, d3, vp);
        a3 = mul_mod_p51_avx_fma(a3, curr_twiddle, vp, vp_inv);

        twiddle = mul_mod_p51_avx_fma(twiddle, psi4v, vp, vp_inv);
        curr_twiddle = twiddle;

        a0 = normalize_p51_avx_fma(a0, vp, vp_inv);
        a1 = normalize_p51_avx_fma(a1, vp, vp_inv);
        a2 = normalize_p51_avx_fma(a2, vp, vp_inv);
        a3 = normalize_p51_avx_fma(a3, vp, vp_inv);
        
        _mm256_storeu_pd(&op1[j], a0);
        _mm256_storeu_pd(&op1[j + 16], a1);
        _mm256_storeu_pd(&op1[j + 32], a2);
        _mm256_storeu_pd(&op1[j + 48], a3);
    }

    __m256d psi8v = _mm256_set1_pd(psi8);
    __m256d psi12v = mul_mod_p51_avx_fma(psi8v, psi4v, vp, vp_inv);

    __m256d twd1 = _mm256_set_pd(_mm256_cvtsd_f64(psi12v), psi8, psi4, 1.0);
    __m256d twd2 = mul_mod_p51_avx_fma(twd1, twd1, vp, vp_inv);
    __m256d twd3 = mul_mod_p51_avx_fma(twd2, twd1, vp, vp_inv);

    APAC_UNROLL(4)
    for (ap_size_t j = 0; j < 64; j += 16)
    {
        /*
            Growth analysis (v(x) = |x| / p)

            Inputs:                         v(c0), v(c1), v(c2), v(c3) < 0.5+

            d0 = c0 + c2                    => v(d0) < 1+
            d1 = c0 - c2                    => v(d1) < 1+
            d2 = c1 + c3                    => v(d2) < 1+
            d3 = c1 - c3                    => v(d3) < 1+

            d3 = mul_mod(d3, zeta)          => v(d3) <= 1/2 + 1/2 * 1 * 1 = 1

            d0 = normalize(d0)              => v(d0) < 0.5+

            a0 = d0 + d2                    => v(a0) < 0.5 + 1   = 1.5+

            a1 = d1 + d3                    => v(a1) < 1 + 1     = 2
            a1 = mul_mod(a1, twd1)          => v(a1) <= 1/2 + 1/2 * 2   * 1 = 1.5

            a2 = d0 - d2                    => v(a2) < 0.5 + 1   = 1.5+
            a2 = mul_mod(a2, twd2)          => v(a2) <= 1/2 + 1/2 * 1.5 * 1 = 1.25

            a3 = d1 - d3                    => v(a3) < 1 + 1     = 2
            a3 = mul_mod(a3, twd3)          => v(a3) <= 1/2 + 1/2 * 2   * 1 = 1.5

            Before normalize:               v(a0) < 1.5+
                                            v(a1) <= 1.5
                                            v(a2) <= 1.25
                                            v(a3) <= 1.5

            normalize(a0,a1,a2,a3)          => centered residues

            Stored outputs:                 v(output) < 0.5+

            Loop invariant restored.
        */

        __m256d c0 = _mm256_loadu_pd(&op1[j]);
        __m256d c1 = _mm256_loadu_pd(&op1[j + 4]);
        __m256d c2 = _mm256_loadu_pd(&op1[j + 8]);
        __m256d c3 = _mm256_loadu_pd(&op1[j + 12]);
        
        __m256d d0 = add_mod_p51_avx(c0, c2, vp);
        __m256d d1 = sub_mod_p51_avx(c0, c2, vp);
        __m256d d2 = add_mod_p51_avx(c1, c3, vp);
        __m256d d3 = sub_mod_p51_avx(c1, c3, vp);
    
        d3 = mul_mod_p51_avx_fma(d3, zetav, vp, vp_inv);
        d0 = normalize_p51_avx_fma(d0, vp, vp_inv);

        __m256d a0 = add_mod_p51_avx(d0, d2, vp);
        
        __m256d a1 = add_mod_p51_avx(d1, d3, vp);
        a1 = mul_mod_p51_avx_fma(a1, twd1, vp, vp_inv);

        __m256d a2 = sub_mod_p51_avx(d0, d2, vp);
        a2 = mul_mod_p51_avx_fma(a2, twd2, vp, vp_inv);

        __m256d a3 = sub_mod_p51_avx(d1, d3, vp);
        a3 = mul_mod_p51_avx_fma(a3, twd3, vp, vp_inv);

        a0 = normalize_p51_avx_fma(a0, vp, vp_inv);
        a1 = normalize_p51_avx_fma(a1, vp, vp_inv);
        a2 = normalize_p51_avx_fma(a2, vp, vp_inv);
        a3 = normalize_p51_avx_fma(a3, vp, vp_inv);
        
        _mm256_storeu_pd(&op1[j], a0);
        _mm256_storeu_pd(&op1[j + 4], a1);
        _mm256_storeu_pd(&op1[j + 8], a2);
        _mm256_storeu_pd(&op1[j + 12], a3);
    }

    // zeta_top < 1
    __m256d zeta_top = _mm256_set_pd(zeta, 1.0, 1.0, 1.0);

    APAC_UNROLL(4)
    for (ap_size_t j = 0; j < 64; j += 16)
    {
                /*
            Growth analysis (v(x) = |x| / p)

            Inputs:
                                            v(j0), v(j1), v(j2), v(j3) < 0.5+

            Permute stage:
                                            v(r1_0), v(r2_0) < 0.5+
                                            v(r1_1), v(r2_1) < 0.5+
                                            v(r1_2), v(r2_2) < 0.5+
                                            v(r1_3), v(r2_3) < 0.5+
                                            (pure permutations, bounds unchanged)

            For k = 0..3:

                a0_k = addsub(r1_k, r2_k)       => v(a0_k) < 1+

                permute(a0_k)                   => v(a0_k) < 1+

                a0_k = mul_mod(a0_k, zeta_top)  => v(a0_k) <= 1

            Stored outputs:         v(output) <= 1

            Loop invariant:         v(output) <= 1
        */

        __m256d r0_0 = _mm256_loadu_pd(&op1[j +  0]);
        __m256d r1_0 = _mm256_permute4x64_pd(r0_0, 0x50);
        __m256d r2_0 = _mm256_permute4x64_pd(r0_0, 0xFA);

        __m256d r0_1 = _mm256_loadu_pd(&op1[j +  4]);
        __m256d r1_1 = _mm256_permute4x64_pd(r0_1, 0x50);
        __m256d r2_1 = _mm256_permute4x64_pd(r0_1, 0xFA);

        __m256d r0_2 = _mm256_loadu_pd(&op1[j +  8]);
        __m256d r1_2 = _mm256_permute4x64_pd(r0_2, 0x50);
        __m256d r2_2 = _mm256_permute4x64_pd(r0_2, 0xFA);

        __m256d r0_3 = _mm256_loadu_pd(&op1[j + 12]);
        __m256d r1_3 = _mm256_permute4x64_pd(r0_3, 0x50);
        __m256d r2_3 = _mm256_permute4x64_pd(r0_3, 0xFA);

        __m256d a0_0 = _mm256_addsub_pd(r1_0, r2_0);
        a0_0 = _mm256_permute_pd(a0_0, 0xF);
        a0_0 = mul_mod_p51_avx_fma(a0_0, zeta_top, vp, vp_inv);
        
        __m256d a0_1 = _mm256_addsub_pd(r1_1, r2_1);
        a0_1 = _mm256_permute_pd(a0_1, 0xF);
        a0_1 = mul_mod_p51_avx_fma(a0_1, zeta_top, vp, vp_inv);

        __m256d a0_2 = _mm256_addsub_pd(r1_2, r2_2);
        a0_2 = _mm256_permute_pd(a0_2, 0xF);
        a0_2 = mul_mod_p51_avx_fma(a0_2, zeta_top, vp, vp_inv);

        __m256d a0_3 = _mm256_addsub_pd(r1_3, r2_3);
        a0_3 = _mm256_permute_pd(a0_3, 0xF);
        a0_3 = mul_mod_p51_avx_fma(a0_3, zeta_top, vp, vp_inv);

        _mm256_storeu_pd(&op1[j +  0], a0_0);
        _mm256_storeu_pd(&op1[j +  4], a0_1);
        _mm256_storeu_pd(&op1[j +  8], a0_2);
        _mm256_storeu_pd(&op1[j + 12], a0_3);
    }
}

#if defined(__GNUC__) || defined(__clang__)
__attribute((target("avx,fma")))
#endif
static void
radix4_dit_inv_ntt_unroll64(
    double* op1,
    __m256d vp,
    __m256d vp_inv,
    const ap_dig_t* 
)
{

}

/*
    TO WRITE:
    - r4_dit_ntt_unroll64
    - dif_fwd_ntt_avx_fma
    - dit_inv_ntt_avx_fma
    - pointwise_mul_avx_fma
    - matrix_trans_avx_fma
    - garner_crt_avx_fma
*/

#if defined(__GNUC__) || defined(__clang__)
__attribute__((target("avx,fma")))
#endif
void
dif_fwd_ntt_avx_fma(
    ap_dig_t* op1,      /* in-place DIF-Forward NTT */
    ap_size_t size,     /* size must be power of 2  */
    const ntt_prime_t* p
)
{
    APAC_ASSERT((size & (size - 1)) == 0);
    APAC_ASSERT(size <= (1ULL << 42));
    APAC_ASSERT(size >= (1ULL << 7));

    ap_size_t twiddle_idx = 0;

    double* op = (double*)op1;

    __m256d vp = _mm256_set1_pd((double)p->prime);
    __m256d vp_inv = _mm256_set1_pd(p->prime_inv);

    for (ap_size_t stride = size / 2; stride >= 64; stride /= 2, twiddle_idx++)
    {
        double omega = (double)p->twiddle[twiddle_idx];
        double twiddle = 1.0;

        for (ap_size_t j = 0; j < stride; j++)
        {
            
        }
    }
}