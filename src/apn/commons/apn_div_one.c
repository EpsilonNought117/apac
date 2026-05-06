#include "../headers/hidden_div.h"

ap_dig_t apn_div_one(
    ap_dig_t* quotient,    // must be (size_divd) length
    const ap_dig_t* dividend,
    ap_dig_t divisor,
    ap_size_t size_divd
)
{
    APAC_ASSERT(quotient != NULL);
    APAC_ASSERT(dividend != NULL);
    APAC_ASSERT(size_divd != 0);
    APAC_FULL_ALIAS_ONLY(quotient, size_divd, dividend, size_divd);
    APAC_ASSERT(divisor != 0);

    ap_dig_t rmdr = 0;
    uint32_t shift_val = 0;

    if (!(divisor & APN_DIG_HIGH_BIT))
    {
        CLZ(divisor, shift_val);
        APAC_ASSERT(shift_val != APN_DIG_BITS);

        divisor <<= shift_val;
        rmdr = dividend[size_divd - 1] >> (APN_DIG_BITS - shift_val);
    }

    ap_dig_t dvsr_recip = apn_recip_word_2by1(divisor);
    ap_dig_t temp_val = 0;

    for (ap_size_t j = size_divd - 1; j >= 1; j--)
    {
        ap_dig_t valid_shift = (dividend[j] << shift_val) | (dividend[j - 1] >> (APN_DIG_BITS - shift_val));

        temp_val = (shift_val) ? valid_shift : dividend[j];
        quotient[j] = apn_udiv_2by1(rmdr, temp_val, divisor, dvsr_recip, &rmdr);
    }

    temp_val = dividend[0] << shift_val;
    quotient[0] = apn_udiv_2by1(rmdr, temp_val, divisor, dvsr_recip, &rmdr);

    if (shift_val)
    {
        rmdr >>= (ap_size_t)shift_val;
    }

    return rmdr;
}