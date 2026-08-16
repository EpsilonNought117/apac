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
dif_fwd_ntt_r4_unroll64(
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

    __m256d twd1 = _mm256_loadu_pd(&twd_tbl[48]);
    __m256d twd2 = _mm256_loadu_pd(&twd_tbl[52]);
    __m256d twd3 = _mm256_loadu_pd(&twd_tbl[56]);

APAC_UNROLL(4)
    for (apn_size_t j = 0; j < 64; j += 16)
    {
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
    for (apn_size_t j = 0; j < 64; j += 16)
    {
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
dit_inv_ntt_r4_unroll64(
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

        d3 = mul_mod_p51_avx_fma(
            d3,
            zeta_inv,
            vp,
            vp_inv
        );

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
        __m256d c0 = _mm256_loadu_pd(&op1[j]);
        __m256d c1 = _mm256_loadu_pd(&op1[j + 4]);
        __m256d c2 = _mm256_loadu_pd(&op1[j + 8]);
        __m256d c3 = _mm256_loadu_pd(&op1[j + 12]);

        __m256d d0 = c0;
        __m256d d1 = mul_mod_p51_avx_fma(
            c1,
            twd1,
            vp,
            vp_inv
        );
        __m256d d2 = mul_mod_p51_avx_fma(
            c2,
            twd2,
            vp,
            vp_inv
        );
        __m256d d3 = mul_mod_p51_avx_fma(
            c3,
            twd3,
            vp,
            vp_inv
        );

        __m256d t0 = add_mod_p51_avx(d0, d2, vp);
        __m256d t1 = sub_mod_p51_avx(d0, d2, vp);
        __m256d t2 = add_mod_p51_avx(d1, d3, vp);
        __m256d t3 = sub_mod_p51_avx(d1, d3, vp);

        t3 = mul_mod_p51_avx_fma(
            t3,
            zeta_inv,
            vp,
            vp_inv
        );

        __m256d a0 = add_mod_p51_avx(t0, t2, vp);
        __m256d a1 = add_mod_p51_avx(t1, t3, vp);
        __m256d a2 = sub_mod_p51_avx(t0, t2, vp);
        __m256d a3 = sub_mod_p51_avx(t1, t3, vp);

        a0 = normalize_p51_avx_fma(a0, vp, vp_inv);
        a1 = normalize_p51_avx_fma(a1, vp, vp_inv);
        a2 = normalize_p51_avx_fma(a2, vp, vp_inv);
        a3 = normalize_p51_avx_fma(a3, vp, vp_inv);

        _mm256_storeu_pd(&op1[j],      a0);
        _mm256_storeu_pd(&op1[j + 4],  a1);
        _mm256_storeu_pd(&op1[j + 8],  a2);
        _mm256_storeu_pd(&op1[j + 12], a3);
    }

APAC_UNROLL(4)
    for (apn_size_t j = 0; j < 16; j += 4)
    {
        __m256d c0 = _mm256_loadu_pd(&op1[j]);
        __m256d c1 = _mm256_loadu_pd(&op1[j + 16]);
        __m256d c2 = _mm256_loadu_pd(&op1[j + 32]);
        __m256d c3 = _mm256_loadu_pd(&op1[j + 48]);

        __m256d twd_1j =
            _mm256_loadu_pd(&twd_inv_tbl[j]);

        __m256d twd_2j =
            _mm256_loadu_pd(&twd_inv_tbl[j + 16]);

        __m256d twd_3j =
            _mm256_loadu_pd(&twd_inv_tbl[j + 32]);

        __m256d d0 = c0;
        __m256d d1 = mul_mod_p51_avx_fma(
            c1,
            twd_1j,
            vp,
            vp_inv
        );
        __m256d d2 = mul_mod_p51_avx_fma(
            c2,
            twd_2j,
            vp,
            vp_inv
        );
        __m256d d3 = mul_mod_p51_avx_fma(
            c3,
            twd_3j,
            vp,
            vp_inv
        );

        __m256d t0 = add_mod_p51_avx(d0, d2, vp);
        __m256d t1 = sub_mod_p51_avx(d0, d2, vp);
        __m256d t2 = add_mod_p51_avx(d1, d3, vp);
        __m256d t3 = sub_mod_p51_avx(d1, d3, vp);

        t3 = mul_mod_p51_avx_fma(
            t3,
            zeta_inv,
            vp,
            vp_inv
        );

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

/*
    TO WRITE:
    - dif_fwd_ntt_avx_fma
    - dit_inv_ntt_avx_fma
    - pointwise_mul_avx_fma
    - matrix_trans_avx_fma
    - garner_crt_avx_fma
*/