#include "../../header/apac_internal.h"

apn_dig_t*
apn_reduce_mod_p_u64(
    apn_dig_t* arr,
    apn_size_t n,
    apn_dig_t prime,
    apn_dig_t magic,
    apn_dig_t shift
)
{
    for (apn_size_t i = 0; i < n; i++)
    {
        const apn_dig_t x = arr[i];

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        const apn_dig_t q = __umulh(x, magic) >> shift;

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        apn_dig_t q = ((__uint128_t)x * magic) >> 64;
        q >>= shift;

#endif

        apn_dig_t r = x - q * prime;
        r = (r >= prime) ? r - prime : r;
        arr[i] = r;
    }

    return arr;
}

double*
apn_reduce_mod_p_f64(
    apn_dig_t* arr,
    apn_size_t n,
    apn_dig_t prime,
    apn_dig_t magic,
    apn_dig_t shift
)
{
    double* arr2 = (double*)arr;

    for (apn_size_t i = 0; i < n; i++)
    {
        const apn_dig_t x = arr[i];

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        const apn_dig_t q = __umulh(x, magic) >> shift;

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

        apn_dig_t q = ((__uint128_t)x * magic) >> 64;
        q >>= shift;

#endif

        apn_dig_t r = x - q * prime;
        r = (r >= prime) ? r - prime : r;
        arr2[i] = (double)r;
    }

    return arr2;
}