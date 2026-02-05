#include "../../../../include/apac.h"
#include "../c_headers/hidden_div.h"
#include "../c_headers/hidden_helpers.h"

apn_seg_t apn_div_one(
    apn_seg_t* quotient,    // must be (size_divd) length
    const apn_seg_t* dividend,
    apn_seg_t divisor,
    apn_size_t size_divd
)
{
    APAC_ASSERT(quotient != NULL);
    APAC_ASSERT(dividend != NULL);
    APAC_ASSERT(size_divd != 0);
    APAC_NO_OVERLAP(quotient, size_divd, dividend, size_divd);
    APAC_ASSERT(divisor != 0);

    apn_seg_t rmdr = 0;
    uint32_t shift_val = 0;

    if (!(divisor & APN_SEG_HIGH_BIT))
    {
        CLZ64(divisor, shift_val);
        APAC_ASSERT(shift_val != APN_SEG_BITS);

        divisor <<= shift_val;
        rmdr = dividend[size_divd - 1] >> (APN_SEG_BITS - shift_val);
    }

    apn_seg_t dvsr_recip = recip_word64_2by1(divisor);
    apn_seg_t temp_val = 0;

    for (apn_size_t j = size_divd - 1; j >= 1; j--)
    {
        apn_seg_t valid_shift = (dividend[j] << shift_val) | (dividend[j - 1] >> (APN_SEG_BITS - shift_val));

        temp_val = (shift_val) ? valid_shift : dividend[j];
        quotient[j] = udiv64_2by1(rmdr, temp_val, divisor, dvsr_recip, &rmdr);
    }

    temp_val = dividend[0] << shift_val;
    quotient[0] = udiv64_2by1(rmdr, temp_val, divisor, dvsr_recip, &rmdr);

    if (shift_val)
    {
        rmdr >>= (apn_size_t)shift_val;
    }

    return rmdr;
}