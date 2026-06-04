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
__attribute((target("avx,fma")))
#endif
static void
r4_dif_ntt_unroll64(
    double* op1,
    __m256d vp,
    __m256d vp_inv,
    __m256d zeta,
    const double* codelet_table // always of size 60 
)
{
APAC_UNROLL(4)
    for (ap_size_t j = 0; j < 16; j += 4)
    {
        /*
            Growth analysis (v(x) = |x| / p)

            Inputs:                         v(c0), v(c1), v(c2), v(c3) < 1

            d0 = c0 + c2                    => v(d0) < 2
            d1 = c0 - c2                    => v(d1) < 2
            d2 = c1 + c3                    => v(d2) < 2
            d3 = c1 - c3                    => v(d3) < 2

            d3 = mul_mod(d3, zeta)          => v(d3) <= 1/2 + 1/2 * 2 * 1 = 1.5

            d0 = normalize(d0)              => v(d0) < 0.5+

            a0    = d0 + d2                 => v(a0)    < 0.5 + 2   = 2.5+
            temp1 = d1 + d3                 => v(temp1) < 2 + 1.5   = 3.5
            temp2 = d0 - d2                 => v(temp2) < 0.5 + 2   = 2.5+
            temp3 = d1 - d3                 => v(temp3) < 2 + 1.5   = 3.5

            a1 = mul_mod(temp1, twiddle)    => v(a1) <= 1/2 + 1/2 * 3.5 * 1 = 2.25
            a2 = mul_mod(temp2, twiddle)    => v(a2) <= 1/2 + 1/2 * 2.5 * 1 = 1.75
            a3 = mul_mod(temp3, twiddle)    => v(a3) <= 1/2 + 1/2 * 3.5 * 1 = 2.25

            Before normalize:               v(a0) < 2.5+, v(a1) <= 2.25, v(a2) <= 1.75, v(a3) <= 2.25

            normalize(a0,a1,a2,a3)          => centered residues

            Stored outputs:                 v(output) < 0.5+

            Loop invariant restored.
        */

        __m256d twd1 = _mm256_loadu_pd(&codelet_table[j * 4]);
        __m256d twd2 = _mm256_loadu_pd(&codelet_table[j * 4 + 16]);
        __m256d twd3 = _mm256_loadu_pd(&codelet_table[j * 4 + 32]);
        
        __m256d c0 = _mm256_loadu_pd(&op1[j]);
        __m256d c1 = _mm256_loadu_pd(&op1[j + 16]);
        __m256d c2 = _mm256_loadu_pd(&op1[j + 32]);
        __m256d c3 = _mm256_loadu_pd(&op1[j + 48]);

        __m256d d0 = add_mod_p51_avx(c0, c2, vp);
        __m256d d1 = sub_mod_p51_avx(c0, c2, vp);
        __m256d d2 = add_mod_p51_avx(c1, c3, vp);
        __m256d d3 = sub_mod_p51_avx(c1, c3, vp);

        d3 = mul_mod_p51_avx_fma(d3, zeta, vp, vp_inv);
        d0 = normalize_p51_avx_fma(d0, vp, vp_inv);

        __m256d a0 = add_mod_p51_avx(d0, d2, vp);
                
        __m256d temp1 = add_mod_p51_avx(d1, d3, vp);
        __m256d a1 = mul_mod_p51_avx_fma(temp1, twd1, vp, vp_inv);

        __m256d temp2 = sub_mod_p51_avx(d0, d2, vp);
        __m256d a2 = mul_mod_p51_avx_fma(temp2, twd2, vp, vp_inv);
        
        __m256d temp3 = sub_mod_p51_avx(d1, d3, vp);
        __m256d a3 = mul_mod_p51_avx_fma(temp3, twd3, vp, vp_inv);

        a0 = normalize_p51_avx_fma(a0, vp, vp_inv);
        a1 = normalize_p51_avx_fma(a1, vp, vp_inv);
        a2 = normalize_p51_avx_fma(a2, vp, vp_inv);
        a3 = normalize_p51_avx_fma(a3, vp, vp_inv);
        
        _mm256_storeu_pd(&op1[j], a0);
        _mm256_storeu_pd(&op1[j + 16], a1);
        _mm256_storeu_pd(&op1[j + 32], a2);
        _mm256_storeu_pd(&op1[j + 48], a3);
    }

    __m256d twd1 = _mm256_loadu_pd(&codelet_table[48]);
    __m256d twd2 = _mm256_loadu_pd(&codelet_table[52]);
    __m256d twd3 = _mm256_loadu_pd(&codelet_table[56]);

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

            a0    = d0 + d2                 => v(a0)    < 0.5 + 1   = 1.5+
            temp1 = d1 + d3                 => v(temp1) < 1 + 1     = 2
            temp2 = d0 - d2                 => v(temp2) < 0.5 + 1   = 1.5+
            temp3 = d1 - d3                 => v(temp3) < 1 + 1     = 2

            a1 = mul_mod(temp1, twd1)       => v(a1) <= 1/2 + 1/2 * 2   * 1 = 1.5
            a2 = mul_mod(temp2, twd2)       => v(a2) <= 1/2 + 1/2 * 1.5 * 1 = 1.25
            a3 = mul_mod(temp3, twd3)       => v(a3) <= 1/2 + 1/2 * 2   * 1 = 1.5

            Before normalize:               v(a0) < 1.5+, v(a1) <= 1.5,
                                            v(a2) <= 1.25, v(a3) <= 1.5

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
    
        d3 = mul_mod_p51_avx_fma(d3, zeta, vp, vp_inv);
        d0 = normalize_p51_avx_fma(d0, vp, vp_inv);

        __m256d a0 = add_mod_p51_avx(d0, d2, vp);
        
        __m256d temp1 = add_mod_p51_avx(d1, d3, vp);
        __m256d a1 = mul_mod_p51_avx_fma(temp1, twd1, vp, vp_inv);

        __m256d temp2 = sub_mod_p51_avx(d0, d2, vp);
        __m256d a2 = mul_mod_p51_avx_fma(temp2, twd2, vp, vp_inv);

        __m256d temp3 = sub_mod_p51_avx(d1, d3, vp);
        __m256d a3 = mul_mod_p51_avx_fma(temp3, twd3, vp, vp_inv);

        a0 = normalize_p51_avx_fma(a0, vp, vp_inv);
        a1 = normalize_p51_avx_fma(a1, vp, vp_inv);
        a2 = normalize_p51_avx_fma(a2, vp, vp_inv);
        a3 = normalize_p51_avx_fma(a3, vp, vp_inv);
        
        _mm256_storeu_pd(&op1[j], a0);
        _mm256_storeu_pd(&op1[j + 4], a1);
        _mm256_storeu_pd(&op1[j + 8], a2);
        _mm256_storeu_pd(&op1[j + 12], a3);
    }

APAC_UNROLL(4)
    for (ap_size_t j = 0; j < 64; j += 16)
    {
        /*
            Growth analysis (v(x) = |x| / p)

            Inputs:                         v(r0), v(r1), v(r2), v(r3) < 0.5+

            Transpose/shuffle stage:        v(x0), v(x1), v(x2), v(x3) < 0.5+
                                            (pure permutation, bounds unchanged)

            d0 = x0 + x2                    => v(d0) < 1+
            d1 = x0 - x2                    => v(d1) < 1+
            d2 = x1 + x3                    => v(d2) < 1+
            d3 = x1 - x3                    => v(d3) < 1+

            d3 = mul_mod(d3, zeta)          => v(d3) <= 1/2 + 1/2 * 1 * 1 = 1

            d0 = normalize(d0)              => v(d0) < 0.5+

            a0 = d0 + d2                    => v(a0) < 0.5 + 1 = 1.5+
            a1 = d1 + d3                    => v(a1) < 1 + 1   = 2
            a2 = d0 - d2                    => v(a2) < 0.5 + 1 = 1.5+
            a3 = d1 - d3                    => v(a3) < 1 + 1   = 2

            Before normalize:               v(a0) < 1.5+, v(a1) < 2,
                                            v(a2) < 1.5+, v(a3) < 2

            normalize(a0,a1,a2,a3)          => centered residues

            Stored outputs:                 v(output) < 0.5+

            Loop invariant restored.
        */

        __m256d r0 = _mm256_loadu_pd(&op1[j +  0]);
        __m256d r1 = _mm256_loadu_pd(&op1[j +  4]);
        __m256d r2 = _mm256_loadu_pd(&op1[j +  8]);
        __m256d r3 = _mm256_loadu_pd(&op1[j + 12]);

        __m256d t0 = _mm256_unpacklo_pd(r0, r1);
        __m256d t1 = _mm256_unpackhi_pd(r0, r1);
        __m256d t2 = _mm256_unpacklo_pd(r2, r3);
        __m256d t3 = _mm256_unpackhi_pd(r2, r3);

        __m256d x0 = _mm256_permute2f128_pd(t0, t2, 0x20);
        __m256d x1 = _mm256_permute2f128_pd(t1, t3, 0x20);
        __m256d x2 = _mm256_permute2f128_pd(t0, t2, 0x31);
        __m256d x3 = _mm256_permute2f128_pd(t1, t3, 0x31);

        __m256d d0 = add_mod_p51_avx(x0, x2, vp);
        __m256d d1 = sub_mod_p51_avx(x0, x2, vp);
        __m256d d2 = add_mod_p51_avx(x1, x3, vp);
        __m256d d3 = sub_mod_p51_avx(x1, x3, vp);
        
        d3 = mul_mod_p51_avx_fma(d3, zeta, vp, vp_inv);
        d0 = normalize_p51_avx_fma(d0, vp, vp_inv);

        __m256d a0 = add_mod_p51_avx(d0, d2, vp);
        __m256d a1 = add_mod_p51_avx(d1, d3, vp);
        __m256d a2 = sub_mod_p51_avx(d0, d2, vp);
        __m256d a3 = sub_mod_p51_avx(d1, d3, vp);

        a0 = normalize_p51_avx_fma(a0, vp, vp_inv);
        a1 = normalize_p51_avx_fma(a1, vp, vp_inv);
        a2 = normalize_p51_avx_fma(a2, vp, vp_inv);
        a3 = normalize_p51_avx_fma(a3, vp, vp_inv);

        t0 = _mm256_unpacklo_pd(a0, a1);
        t1 = _mm256_unpackhi_pd(a0, a1);
        t2 = _mm256_unpacklo_pd(a2, a3);
        t3 = _mm256_unpackhi_pd(a2, a3);

        __m256d b0 = _mm256_permute2f128_pd(t0, t2, 0x20);
        __m256d b1 = _mm256_permute2f128_pd(t1, t3, 0x20);
        __m256d b2 = _mm256_permute2f128_pd(t0, t2, 0x31);
        __m256d b3 = _mm256_permute2f128_pd(t1, t3, 0x31);

        _mm256_storeu_pd(&op1[j +  0], b0);
        _mm256_storeu_pd(&op1[j +  4], b1);
        _mm256_storeu_pd(&op1[j +  8], b2);
        _mm256_storeu_pd(&op1[j + 12], b3);
    }
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
    ap_dig_t* op1,      /* in-place DIF-FNTT        */
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
    __m256d vp_inv = _mm256_set1_pd(p->prime_f64_inv);

    for (ap_size_t stage = size / 2; stage >= 64; stage /= 2, twiddle_idx++)
    {
        double omega = (double)p->twiddle[twiddle_idx];
        double twiddle = 1.0;

        for (ap_size_t stride = 0; stride < stage; stride++)
        {
            
        }
    }
}