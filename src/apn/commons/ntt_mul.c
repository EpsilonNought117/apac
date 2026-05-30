#include "../headers/hidden_mul.h"

static inline double*
apn_reduce_mod_p50(
    ap_dig_t *arr,
    ap_size_t n,
    ap_dig_t  prime,
    ap_dig_t  magic,
    unsigned short shift,
    unsigned short add
)
{
    double* dst = (double *)arr;

    for (ap_dig_t i = 0; i < n; i++)
    {
        ap_dig_t x = arr[i];

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        ap_dig_t q;

        if (add)
        {
            ap_dig_t hi = __umulh(x, magic);
            q = (hi + ((x - hi) >> 1)) >> (shift - 1);
        }
        else
        {
            q = __umulh(x, magic) >> shift;
        }

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        ap_dig_t q;

        if (add)
        {
            ap_dig_t hi = (ap_dig_t)(((__uint128_t)x * magic) >> 64);
            q = (hi + ((x - hi) >> 1)) >> (shift - 1);
        }
        else
        {
            q = (ap_dig_t)(((__uint128_t)x * magic) >> (64 + shift));
        }

#endif

        ap_dig_t r = x - q * prime;
        if (r >= prime) r -= prime;

        dst[i] = (double)r;
    }

    return dst;
}

