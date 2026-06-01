#include "../x64_hidden_funcs.h"
#include "../../headers/ntt_primes.h"

// ========================================================
//  SCALAR CODE (for x64 CPUs without AVX-FMA minimum)
// ========================================================

static inline ap_dig_t
mod_mul_p50_x64(
    const ap_dig_t a,
    const ap_dig_t b,
    const ap_dig_t prime,
    const ap_dig_t barrett_m
)
{
#if defined(APAC_X64_WIN)

    ap_dig_t ab_lo      = a * b;
    ap_dig_t ab_hi      = __umulh(a, b);
    ap_dig_t ab_lo_m_hi = __umulh(ab_lo, barrett_m);
    ap_dig_t ab_hi_m_lo = ab_hi * barrett_m;
    ap_dig_t q          = (ab_hi_m_lo + ab_lo_m_hi) >> 36;
    ap_dig_t r          = ab_lo - q * prime;

#elif defined(APAC_X64_UNIX)

    __uint128_t ab = (__uint128_t)a * b;
    ap_dig_t ab_hi = (ab >> 64);
    ap_dig_t ab_lo = (ab);
    ap_dig_t q     = (((__uint128_t)ab_hi * barrett_m
                     + ((__uint128_t)ab_lo * barrett_m >> 64)) >> 36);
    ap_dig_t r     = (ab - (__uint128_t)q * prime);

#endif

    r = (r >= prime) ? r - prime : r;
    return r;
}

static inline ap_dig_t
mod_add_p50_x64(
    const ap_dig_t a,
    const ap_dig_t b,
    const ap_dig_t prime
)
{
    ap_dig_t r = a + b;
    r = (r >= prime) ? r - prime : r;
    return r;
}

static inline ap_dig_t
mod_sub_p50_x64(
    const ap_dig_t a,
    const ap_dig_t b,
    const ap_dig_t prime
)
{
    ap_dig_t r = a - b;
    r = (r > a) ? r + prime : r;
    return r;
}

/**
 * Radix-2 Gentleman-Sande DIF Forward-NTT
 * No need for Scramble Step
 */
void
dif_forward_ntt_x64(
    ap_dig_t* op1,      /* in-place DIF-FNTT        */
    ap_size_t size,     /* size must be power of 2  */
    const ntt_prime_t* p,
    ntt_type transform_type
)
{
    APAC_ASSERT((size & (size - 1)) == 0);
    APAC_ASSERT(size <= (1ULL << 42));

    ap_size_t twiddle_idx = transform_type == CYCLIC ? 0 : 1;

    for (ap_size_t stage = size / 2; stage >= 1; stage /= 2, twiddle_idx++)
    {
        ap_dig_t omega = p->twiddle[twiddle_idx];
        ap_dig_t twiddle = 1;

        for (ap_size_t stride = 0; stride < stage; stride++)
        {
            for (ap_size_t i = stride; i < size; i += (stage << 1))
            {
                ap_dig_t u = op1[i];
                ap_dig_t v = op1[i + stage];

                const ap_dig_t sum = mod_add_p50_x64(u, v, p->prime);
                const ap_dig_t diff = mod_sub_p50_x64(u, v, p->prime);

                op1[i] = sum;
                op1[i + stage] = mod_mul_p50_x64(diff, twiddle, p->prime, p->barrett_m);
            }

            twiddle = mod_mul_p50_x64(twiddle, omega, p->prime, p->barrett_m);
        }
    }
}

/**
 * Radix-2 Cooley-Tukey DIT Inverse-NTT
 * No need for Scramble Step (dual of Gentleman-Sande DIF)
 */
void
dit_inverse_ntt_x64(
    ap_dig_t* op1,
    ap_size_t size,
    const ntt_prime_t* p,
    ntt_type transform_type
)
{
    APAC_ASSERT((size & (size - 1)) == 0);
    APAC_ASSERT(size <= (1ULL << 42));

    ap_size_t num_stages = 0;
    CTZ(size, num_stages);

    ap_size_t twiddle_idx = (transform_type == CYCLIC ? 0 : 1) + num_stages - 1;

    for (ap_size_t stage = 1; stage <= size / 2; stage <<= 1, twiddle_idx--)
    {
        ap_dig_t omega = p->twiddle[twiddle_idx];
        ap_dig_t twiddle = 1;

        for (ap_size_t stride = stage - 1; stride != (ap_size_t)-1; stride--)
        {
            for (ap_size_t i = stride; i < size; i += (stage << 1))
            {
                ap_dig_t u = op1[i];
                ap_dig_t v = mod_mul_p50_x64(op1[i + stage], twiddle, p->prime, p->barrett_m);

                op1[i] = mod_add_p50_x64(u, v, p->prime);
                op1[i + stage] = mod_sub_p50_x64(u, v, p->prime);
            }

            twiddle = mod_mul_p50_x64(twiddle, omega, p->prime, p->barrett_m);
        }
    }

    // Final scaling: multiply every element by N^{-1} mod p
    ap_dig_t n_inv = p->n_inv[num_stages];

    for (ap_size_t i = 0; i < size; i++)
    {
        op1[i] = mod_mul_p50_x64(op1[i], n_inv, p->prime, p->barrett_m);
    }
}

void 
pointwise_mul_x64(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size,
    const ntt_prime_t* p
)
{
    APAC_ASSERT((size & (size - 1)) == 0);
    APAC_ASSERT(size <= (1ULL << 42));

    for (ap_size_t i = 0; i < size; i++)
    {
        result[i] = mod_mul_p50_x64(op1[i], op2[i], p->prime, p->barrett_m);
    }

    return;
}
