#include "../../header/apac_internal.h"
#include "../headers/hidden_div.h"

apac_err_t
apn_div_one(
    apn_dig_t* quotient,        // must be (size_divd + size_divd_frac) length
    apn_dig_t* remainder,       // pointer to a single word
    const apn_dig_t* dividend,  // must be (size_divd) length
    apn_dig_t divisor,          // a single word
    apn_size_t size_divd,       // number of limbs in dividend
    apn_size_t size_divd_frac   // number of fractional limbs to consider
)
{
    APAC_ASSERT(quotient != NULL);
    APAC_ASSERT(remainder != NULL);
    APAC_ASSERT(dividend != NULL);
    APAC_ASSERT(size_divd != 0);
    APAC_ASSERT(size_divd < APN_SIZE_MAX - size_divd_frac);
    APAC_PARTIAL_OVERLAP_BELOW(
        quotient, 
        size_divd + size_divd_frac, 
        dividend, 
        size_divd
    );

    if (divisor == (apn_dig_t)0ULL)
    {
        return APAC_DIV_BY_ZERO;
    }

    apn_dig_t rmdr = 0ULL;
    apn_dig_t shift_val = 0ULL;

    if (!(divisor & APN_DIG_HIGH_BIT))
    {
        CLZ(divisor, shift_val);
        APAC_ASSERT(shift_val != APN_DIG_BITS);

        divisor <<= shift_val;
        rmdr = shift_val ? dividend[size_divd - 1] >> ((APN_DIG_BITS - shift_val) & 63) : rmdr;
    }

    apn_dig_t dvsr_recip = apn_recip_word_2by1(divisor);
    apn_dig_t temp_val = 0ULL;

    for (apn_size_t j = size_divd - 1; j >= 1; j--)
    {
        apn_dig_t shifted = (dividend[j] << shift_val) | (dividend[j - 1] >> ((APN_DIG_BITS - shift_val) & 63));
        temp_val = shift_val ? shifted : dividend[j];
        quotient[j + size_divd_frac] = apn_udiv_2by1(rmdr, temp_val, divisor, dvsr_recip, &rmdr);
    }

    temp_val = dividend[0] << shift_val;
    quotient[size_divd_frac] = apn_udiv_2by1(rmdr, temp_val, divisor, dvsr_recip, &rmdr);

    // now for the frac part of the loop
    // nothing to shift and adjust here as it's all zeros
    for (apn_size_t j = size_divd_frac - 1; j < size_divd_frac; j--)
    {
        quotient[j] = apn_udiv_2by1(rmdr, 0, divisor, dvsr_recip, &rmdr);
    }

    if (shift_val)
    {
        rmdr >>= (apn_size_t)shift_val;
    }

    *remainder = rmdr;
    return APAC_OK;
}