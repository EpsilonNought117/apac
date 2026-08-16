#include "../../header/apac_internal.h"

void
apn_reduce_mod_p_u64(
    apn_dig_t* arr,
    apn_size_t n,
    apn_dig_t prime,
    apn_dig_t magic,
    apn_dig_t shift
)
{
APAC_UNROLL(4)
    for (apn_size_t i = 0; i < n; i++)
    {
        const apn_dig_t x = arr[i];

#if defined(APAC_WIN_X64) || defined(APAC_WIN_ARM64)

        const apn_dig_t q = __umulh(x, magic) >> shift;

#elif defined(APAC_LINUX_X64) || defined(APAC_LINUX_ARM64)  || \
      defined(APAC_MACOS_X64) || defined(APAC_MACOX_ARM64)

        apn_dig_t q = ((__uint128_t)x * magic) >> 64;
        q >>= shift;

#endif

        apn_dig_t r = x - q * prime;
        r = (r >= prime) ? r - prime : r;
        arr[i] = r;
    }
}

void
apn_reduce_mod_p_f64(
    double* arr2,
    apn_dig_t* arr1,
    apn_size_t n,
    apn_dig_t prime,
    apn_dig_t magic,
    apn_dig_t shift
)
{
APAC_UNROLL(4)
    for (apn_size_t i = 0; i < n; i++)
    {
        const apn_dig_t x = arr1[i];

#if defined(APAC_WIN_X64) || defined(APAC_WIN_ARM64)

        const apn_dig_t q = __umulh(x, magic) >> shift;

#elif defined(APAC_LINUX_X64) || defined(APAC_LINUX_ARM64)  || \
      defined(APAC_MACOS_X64) || defined(APAC_MACOX_ARM64)

        apn_dig_t q = ((__uint128_t)x * magic) >> 64;
        q >>= shift;

#endif

        apn_dig_t r = x - q * prime;
        r = (r >= prime) ? r - prime : r;
        arr2[i] = (double)r;
    }
}