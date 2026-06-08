#include "../../header/apac_internal.h"

ap_dig_t*
apn_reduce_mod_p_u64(
    ap_dig_t* arr,
    ap_size_t n,
    ap_dig_t prime,
    ap_dig_t magic,
    ap_dig_t shift
)
{
    for (ap_size_t i = 0; i < n; i++)
    {
        const ap_dig_t x = arr[i];

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        const ap_dig_t q = __umulh(x, magic) >> shift;

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        ap_dig_t q = ((__uint128_t)x * magic) >> 64;
        q >>= shift;

#endif

        ap_dig_t r = x - q * prime;
        r = (r >= prime) ? r - prime : r;
        arr[i] = r;
    }

    return arr;
}

double*
apn_reduce_mod_p_f64(
    ap_dig_t* arr,
    ap_size_t n,
    ap_dig_t prime,
    ap_dig_t magic,
    ap_dig_t shift
)
{
    double* arr2 = (double*)arr;

    for (ap_size_t i = 0; i < n; i++)
    {
        const ap_dig_t x = arr[i];

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        const ap_dig_t q = __umulh(x, magic) >> shift;

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        ap_dig_t q = ((__uint128_t)x * magic) >> 64;
        q >>= shift;

#endif

        ap_dig_t r = x - q * prime;
        r = (r >= prime) ? r - prime : r;
        arr2[i] = (double)r;
    }

    return arr2;
}