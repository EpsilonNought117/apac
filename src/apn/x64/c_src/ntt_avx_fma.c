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
fwd_ntt_dif_r4_unroll64(
    double* op1,
    __m256d vp,
    __m256d vp_inv,
    __m256d zeta,
    const double* twd_tbl // always of size 60 
)
{
APAC_UNROLL(4)
    for (apn_size_t j = 0; j < 16; j += 4)
    {
        /*
            Growth analysis (v(x) = |x| / p),  p = 75*2^43 + 1 = 659706976665601 (50-bit prime)

            mul_mod growth coefficient: v(d) <= 1/2 + 0.15 * v(a) * v(b)
            (0.15 safely rounds up the exact coefficient 2C = 0.14648437500000028 for this p)

            Inputs:                         v(c0), v(c1), v(c2), v(c3) < 1     (worst case, unnormalized input)
                                                                        <= 0.5+ (best case, freshly normalized)
            d0 = c0 + c2                    => v(d0) < 2
            d1 = c0 - c2                    => v(d1) < 2
            d2 = c1 + c3                    => v(d2) < 2
            d3 = c1 - c3                    => v(d3) < 2

            d3 = mul_mod(d3, zeta)          => v(d3) <= 1/2 + 0.15 * 2 * 1 = 0.8

            d0 = normalize(d0)              => v(d0) <= 0.5+   (exactly 0.5 + 5/(2p) = 0.5000000000000038)
            d2 = normalize(d2)              => v(d2) <= 0.5+   (same as v(d0) above)

            a0    = d0 + d2                 => v(a0)    <  0.5 + 0.5  = 1+
            temp1 = d1 + d3                 => v(temp1) <  2   + 0.8  = 2.8
            temp2 = d0 - d2                 => v(temp2) <  0.5 + 0.5  = 1+
            temp3 = d1 - d3                 => v(temp3) <  2   + 0.8  = 2.8

            a1 = mul_mod(temp1, twiddle)    => v(a1) <= 1/2 + 0.15 * 2.8 * 1 = 0.92+ < 1
            a2 = mul_mod(temp2, twiddle)    => v(a2) <= 1/2 + 0.15 * 1   * 1 = 0.65+ < 1
            a3 = mul_mod(temp3, twiddle)    => v(a3) <= 1/2 + 0.15 * 2.8 * 1 = 0.92+ < 1

            NO NORMALIZATION DONE

            Stored outputs:                 v(output) < 1+
        */

        __m256d twd1 = _mm256_loadu_pd(&twd_tbl[j]);
        __m256d twd2 = _mm256_loadu_pd(&twd_tbl[j + 16]);
        __m256d twd3 = _mm256_loadu_pd(&twd_tbl[j + 32]);
        
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
        d2 = normalize_p51_avx_fma(d2, vp, vp_inv);

        __m256d a0 = add_mod_p51_avx(d0, d2, vp);
                
        __m256d temp1 = add_mod_p51_avx(d1, d3, vp);
        __m256d a1 = mul_mod_p51_avx_fma(temp1, twd1, vp, vp_inv);

        __m256d temp2 = sub_mod_p51_avx(d0, d2, vp);
        __m256d a2 = mul_mod_p51_avx_fma(temp2, twd2, vp, vp_inv);
        
        __m256d temp3 = sub_mod_p51_avx(d1, d3, vp);
        __m256d a3 = mul_mod_p51_avx_fma(temp3, twd3, vp, vp_inv);
        
        _mm256_storeu_pd(&op1[j], a0);
        _mm256_storeu_pd(&op1[j + 16], a1);
        _mm256_storeu_pd(&op1[j + 32], a2);
        _mm256_storeu_pd(&op1[j + 48], a3);
    }

    __m256d twd1 = _mm256_loadu_pd(&twd_tbl[48]);
    __m256d twd2 = _mm256_loadu_pd(&twd_tbl[52]);
    __m256d twd3 = _mm256_loadu_pd(&twd_tbl[56]);

APAC_UNROLL(4)
    for (apn_size_t j = 0; j < 64; j += 16)
    {
        /*
            Growth analysis (v(x) = |x| / p),  starting from v_in < 1+

            mul_mod growth coefficient: v(d) <= 1/2 + 0.15 * v(a) * v(b)
            (0.15 safely rounds up the exact coefficient 2C = 0.14648437500000028
            for p = 75*2^43 + 1)

            Inputs:                         v(c0), v(c1), v(c2), v(c3) < 1+

            d0 = c0 + c2                    => v(d0) < 2+
            d1 = c0 - c2                    => v(d1) < 2+
            d2 = c1 + c3                    => v(d2) < 2+
            d3 = c1 - c3                    => v(d3) < 2+

            d3 = mul_mod(d3, zeta)          => v(d3) <= 1/2 + 0.15 * 2 * 1 = 0.8+ < 1
            d0 = normalize(d0)              => v(d0) < 0.5+

            a0    = d0 + d2                 => v(a0)    < 0.5 + 2     = 2.5+
            temp1 = d1 + d3                 => v(temp1) < 2   + 1     = 3+
            temp2 = d0 - d2                 => v(temp2) < 0.5 + 1     = 2.5+
            temp3 = d1 - d3                 => v(temp3) < 2   + 1     = 3+

            a0 = normalize(a0)              => v(a0) <  0.5+
            a1 = mul_mod(temp1, twd1)       => v(a1) <= 1/2 + 0.15 * 3.0 * 1 = 0.95+  < 1
            a2 = mul_mod(temp2, twd2)       => v(a2) <= 1/2 + 0.15 * 2.5 * 1 = 0.875+ < 1
            a3 = mul_mod(temp3, twd3)       => v(a3) <= 1/2 + 0.15 * 3.0 * 1 = 0.95+  < 1

            NO NORMALIZATION STEP

            Stored outputs:                 v(output) < 1+
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
        __m256d temp2 = sub_mod_p51_avx(d0, d2, vp);
        __m256d temp3 = sub_mod_p51_avx(d1, d3, vp);

        a0 = normalize_p51_avx_fma(a0, vp, vp_inv);
        __m256d a1 = mul_mod_p51_avx_fma(temp1, twd1, vp, vp_inv);
        __m256d a2 = mul_mod_p51_avx_fma(temp2, twd2, vp, vp_inv);
        __m256d a3 = mul_mod_p51_avx_fma(temp3, twd3, vp, vp_inv);

        _mm256_storeu_pd(&op1[j], a0);
        _mm256_storeu_pd(&op1[j + 4], a1);
        _mm256_storeu_pd(&op1[j + 8], a2);
        _mm256_storeu_pd(&op1[j + 12], a3);
    }

APAC_UNROLL(4)
    for (apn_size_t j = 0; j < 64; j += 16)
    {
        /*
            Growth analysis (v(x) = |x| / p),  starting from v_in < 1+

            Inputs:                         v(r0), v(r1), v(r2), v(r3) < 1+

            Transpose/shuffle stage:        v(x0), v(x1), v(x2), v(x3) < 1+
                                            (pure permutation, bounds unchanged)

            d0 = x0 + x2                    => v(d0) < 2+
            d1 = x0 - x2                    => v(d1) < 2+
            d2 = x1 + x3                    => v(d2) < 2+
            d3 = x1 - x3                    => v(d3) < 2+

            d3 = mul_mod(d3, zeta)          => v(d3) <= 1/2 + 0.15 * 2 * 1 = 0.8 < 1+
            d0 = normalize(d0)              => v(d0) <  0.5+

            a0 = d0 + d2                    => v(a0) < 0.5 + 2  = 2.5+
            a1 = d1 + d3                    => v(a1) < 2 + 1    = 3+
            a2 = d0 - d2                    => v(a2) < 0.5 + 2  = 2.5+
            a3 = d1 - d3                    => v(a3) < 2 + 1    = 3+

            Before normalize:               v(a0) < 2.5+, v(a1) < 3+,
                                            v(a2) < 2.5+, v(a3) < 3+

            normalize(a0,a1,a2,a3)          => centered residues

            Stored outputs:                 v(output) < 0.5+

            Loop invariant restored.
        */

        __m256d r0 = _mm256_loadu_pd(&op1[j]);
        __m256d r1 = _mm256_loadu_pd(&op1[j + 4]);
        __m256d r2 = _mm256_loadu_pd(&op1[j + 8]);
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

        _mm256_storeu_pd(&op1[j], b0);
        _mm256_storeu_pd(&op1[j + 4], b1);
        _mm256_storeu_pd(&op1[j + 8], b2);
        _mm256_storeu_pd(&op1[j + 12], b3);
    }
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((target("avx,fma")))
#endif
static void
inv_ntt_dit_r4_unroll64(
    double* op1,
    __m256d vp,
    __m256d vp_inv,
    __m256d zeta_inv,
    const double* twd_inv_tbl
)
{
APAC_UNROLL(4)
    for (apn_size_t j = 0; j < 64; j += 16)
    {
    /*
        Growth analysis (v(x) = |x| / p)

        mul_mod growth coefficient: v(d) <= 1/2 + 0.15 * v(a) * v(b)
        (0.15 safely rounds up the exact coefficient 2C = 0.14648437500000028
        for p = 75*2^43 + 1)

        Inputs:                         v(r0), v(r1), v(r2), v(r3) < 1

        Transpose/shuffle stage:        v(c0), v(c1), v(c2), v(c3) < 1
                                        (pure permutation, bounds unchanged)

        d0 = c0 + c2                    => v(d0) < 2
        d1 = c0 - c2                    => v(d1) < 2
        d2 = c1 + c3                    => v(d2) < 2
        d3 = c1 - c3                    => v(d3) < 2

        d3 = mul_mod(d3, zeta_inv)      => v(d3) <= 1/2 + 0.15 * 2 * 1 = 0.8

        d0 = normalize(d0)              => v(d0) < 0.5+

        a0 = d0 + d2                    => v(a0) < 0.5 + 2   = 2.5+
        a1 = d1 + d3                    => v(a1) < 2 + 0.8   = 2.8
        a2 = d0 - d2                    => v(a2) < 0.5 + 2   = 2.5+
        a3 = d1 - d3                    => v(a3) < 2 + 0.8   = 2.8

        NO NORMALIZATION DONE

        Stored outputs:                 v(output) < 2.8 (worst case)

    */

        __m256d r0 = _mm256_loadu_pd(&op1[j]);
        __m256d r1 = _mm256_loadu_pd(&op1[j + 4]);
        __m256d r2 = _mm256_loadu_pd(&op1[j + 8]);
        __m256d r3 = _mm256_loadu_pd(&op1[j + 12]);

        __m256d t0 = _mm256_unpacklo_pd(r0, r1);
        __m256d t1 = _mm256_unpackhi_pd(r0, r1);
        __m256d t2 = _mm256_unpacklo_pd(r2, r3);
        __m256d t3 = _mm256_unpackhi_pd(r2, r3);

        __m256d c0 = _mm256_permute2f128_pd(t0, t2, 0x20);
        __m256d c1 = _mm256_permute2f128_pd(t1, t3, 0x20);
        __m256d c2 = _mm256_permute2f128_pd(t0, t2, 0x31);
        __m256d c3 = _mm256_permute2f128_pd(t1, t3, 0x31);

        __m256d d0 = add_mod_p51_avx(c0, c2, vp);
        __m256d d1 = sub_mod_p51_avx(c0, c2, vp);
        __m256d d2 = add_mod_p51_avx(c1, c3, vp);
        __m256d d3 = sub_mod_p51_avx(c1, c3, vp);

        d3 = mul_mod_p51_avx_fma(d3, zeta_inv, vp, vp_inv);
        d0 = normalize_p51_avx_fma(d0, vp, vp_inv);

        __m256d a0 = add_mod_p51_avx(d0, d2, vp);
        __m256d a1 = add_mod_p51_avx(d1, d3, vp);
        __m256d a2 = sub_mod_p51_avx(d0, d2, vp);
        __m256d a3 = sub_mod_p51_avx(d1, d3, vp);

        t0 = _mm256_unpacklo_pd(a0, a1);
        t1 = _mm256_unpackhi_pd(a0, a1);
        t2 = _mm256_unpacklo_pd(a2, a3);
        t3 = _mm256_unpackhi_pd(a2, a3);

        __m256d b0 = _mm256_permute2f128_pd(t0, t2, 0x20);
        __m256d b1 = _mm256_permute2f128_pd(t1, t3, 0x20);
        __m256d b2 = _mm256_permute2f128_pd(t0, t2, 0x31);
        __m256d b3 = _mm256_permute2f128_pd(t1, t3, 0x31);

        _mm256_storeu_pd(&op1[j],      b0);
        _mm256_storeu_pd(&op1[j + 4],  b1);
        _mm256_storeu_pd(&op1[j + 8],  b2);
        _mm256_storeu_pd(&op1[j + 12], b3);
    }

    __m256d twd1 = _mm256_loadu_pd(&twd_inv_tbl[48]);
    __m256d twd2 = _mm256_loadu_pd(&twd_inv_tbl[52]);
    __m256d twd3 = _mm256_loadu_pd(&twd_inv_tbl[56]);

APAC_UNROLL(4)
    for (apn_size_t j = 0; j < 64; j += 16)
    {
        /*
            Growth analysis (v(x) = |x| / p)

            Inputs:                         v(c0), v(c1), v(c2), v(c3) < 2.8

            d0 = c0                         => v(d0) <  2.8
            d1 = mul_mod(c1, twd1)          => v(d1) <= 1/2 + 0.15 * 2.8 * 1 = 0.92 < 1
            d2 = mul_mod(c2, twd2)          => v(d2) <= 0.92 < 1
            d3 = mul_mod(c3, twd3)          => v(d3) <= 0.92 < 1

            d0 = normalize(d0)              => v(d0) < 0.5+

            t0 = d0 + d2                    => v(t0) < 0.5 + 1 = 1.5+
            t1 = d0 - d2                    => v(t1) < 0.5 + 1 = 1.5+
            t2 = d1 + d3                    => v(t2) < 1 + 1   = 2.0
            t3 = d1 - d3                    => v(t3) < 1 + 1   = 2.0

            t3 = mul_mod(t3, zeta_inv)      => v(t3) <= 1/2 + 0.15 * 2 * 1 = 0.8 < 1

            a0 = t0 + t2                    => v(a0) < 1.5 + 2.0 = 3.5
            a1 = t1 + t3                    => v(a1) < 1.5 + 0.8 = 2.3
            a2 = t0 - t2                    => v(a2) < 1.5 + 2.0 = 3.5
            a3 = t1 - t3                    => v(a3) < 1.5 + 0.8 = 2.3

            NO NORMALIZATION DONE

            Stored outputs:                 v(output) < 3.5 (worst case)
        */

        __m256d c0 = _mm256_loadu_pd(&op1[j]);
        __m256d c1 = _mm256_loadu_pd(&op1[j + 4]);
        __m256d c2 = _mm256_loadu_pd(&op1[j + 8]);
        __m256d c3 = _mm256_loadu_pd(&op1[j + 12]);

        __m256d d0 = c0;
        __m256d d1 = mul_mod_p51_avx_fma(c1, twd1, vp, vp_inv);
        __m256d d2 = mul_mod_p51_avx_fma(c2, twd2, vp, vp_inv);
        __m256d d3 = mul_mod_p51_avx_fma(c3, twd3, vp, vp_inv);

        d0 = normalize_p51_avx_fma(d0, vp, vp_inv);

        __m256d t0 = add_mod_p51_avx(d0, d2, vp);
        __m256d t1 = sub_mod_p51_avx(d0, d2, vp);
        __m256d t2 = add_mod_p51_avx(d1, d3, vp);
        __m256d t3 = sub_mod_p51_avx(d1, d3, vp);

        t3 = mul_mod_p51_avx_fma(t3, zeta_inv, vp, vp_inv);

        __m256d a0 = add_mod_p51_avx(t0, t2, vp);
        __m256d a1 = add_mod_p51_avx(t1, t3, vp);
        __m256d a2 = sub_mod_p51_avx(t0, t2, vp);
        __m256d a3 = sub_mod_p51_avx(t1, t3, vp);

        _mm256_storeu_pd(&op1[j],      a0);
        _mm256_storeu_pd(&op1[j + 4],  a1);
        _mm256_storeu_pd(&op1[j + 8],  a2);
        _mm256_storeu_pd(&op1[j + 12], a3);
    }

APAC_UNROLL(4)
    for (apn_size_t j = 0; j < 16; j += 4)
    {
        /*
            Growth analysis (v(x) = |x| / p)

            Inputs:                         v(c0), v(c1), v(c2), v(c3) < 3.5

            d0 = c0                         => v(d0) < 3.5
            d1 = mul_mod(c1, twd_1j)        => v(d1) <= 1/2 + 0.15 * 3.5 * 1 = 1.025+ < 1.5
            d2 = mul_mod(c2, twd_2j)        => v(d2) <  1.5
            d3 = mul_mod(c3, twd_3j)        => v(d3) <  1.5

            d0 = normalize(d0)              => v(d0) <= 0.5+

            t0 = d0 + d2                    => v(t0) < 0.5 + 1.5 = 2+
            t1 = d0 - d2                    => v(t1) < 0.5 + 1.5 = 2+
            t2 = d1 + d3                    => v(t2) < 1.5 + 1.5 = 3
            t3 = d1 - d3                    => v(t3) < 1.5 + 1.5 = 3

            t2 = normalize(t2)              => v(t2) <= 0.5+
            t3 = mul_mod(t3, zeta_inv)      => v(t3) <= 1/2 + 0.15 * 3 * 1 = 0.95+ < 1

            a0 = t0 + t2                    => v(a0) < 2 + 0.5 = 2.5+
            a1 = t1 + t3                    => v(a1) < 2 + 1   = 3+
            a2 = t0 - t2                    => v(a2) < 2 + 0.5 = 2.5+
            a3 = t1 - t3                    => v(a3) < 2 + 1   = 3+

            Before normalize:               v(a0) < 2.5+, v(a1) < 3+,
                                            v(a2) < 2.5+, v(a3) < 3+

            normalize(a0,a1,a2,a3)          => centered residues

            Stored outputs:                 v(output) < 0.5+

            Loop invariant restored.
        */

        __m256d c0 = _mm256_loadu_pd(&op1[j]);
        __m256d c1 = _mm256_loadu_pd(&op1[j + 16]);
        __m256d c2 = _mm256_loadu_pd(&op1[j + 32]);
        __m256d c3 = _mm256_loadu_pd(&op1[j + 48]);

        __m256d twd_1j = _mm256_loadu_pd(&twd_inv_tbl[j]);
        __m256d twd_2j = _mm256_loadu_pd(&twd_inv_tbl[j + 16]);
        __m256d twd_3j = _mm256_loadu_pd(&twd_inv_tbl[j + 32]);
        
        __m256d d0 = c0;
        __m256d d1 = mul_mod_p51_avx_fma(c1, twd_1j, vp, vp_inv);
        __m256d d2 = mul_mod_p51_avx_fma(c2, twd_2j, vp, vp_inv);
        __m256d d3 = mul_mod_p51_avx_fma(c3, twd_3j, vp, vp_inv);

        d0 = normalize_p51_avx_fma(d0, vp, vp_inv);

        __m256d t0 = add_mod_p51_avx(d0, d2, vp);
        __m256d t1 = sub_mod_p51_avx(d0, d2, vp);
        __m256d t2 = add_mod_p51_avx(d1, d3, vp);
        __m256d t3 = sub_mod_p51_avx(d1, d3, vp);

        t2 = normalize_p51_avx_fma(t2, vp, vp_inv);
        t3 = mul_mod_p51_avx_fma(t3, zeta_inv, vp, vp_inv);

        __m256d a0 = add_mod_p51_avx(t0, t2, vp);
        __m256d a1 = add_mod_p51_avx(t1, t3, vp);
        __m256d a2 = sub_mod_p51_avx(t0, t2, vp);
        __m256d a3 = sub_mod_p51_avx(t1, t3, vp);

        a0 = normalize_p51_avx_fma(a0, vp, vp_inv);
        a1 = normalize_p51_avx_fma(a1, vp, vp_inv);
        a2 = normalize_p51_avx_fma(a2, vp, vp_inv);
        a3 = normalize_p51_avx_fma(a3, vp, vp_inv);

        _mm256_storeu_pd(&op1[j],      a0);
        _mm256_storeu_pd(&op1[j + 16], a1);
        _mm256_storeu_pd(&op1[j + 32], a2);
        _mm256_storeu_pd(&op1[j + 48], a3);
    }
}
