#include "../../../include/apac.h"
#include "hidden_div.h"
#include "hidden_helpers.h"

apn_seg_t apn_div_one(
    apn_seg_t* quotient,    // must be (size_divd) length
    apn_seg_t* dividend,
    apn_seg_t divisor,
    apn_size_t size_divd
)
{
    APAC_ASSERT(quotient != NULL);
    APAC_ASSERT(dividend != NULL);
    APAC_ASSERT(size_divd != 0);
    APAC_NO_OVERLAP(quotient, size_divd, dividend, size_divd);
    APAC_DETAILED_ASSERT(divisor != 0,
        "Malformed divisor: divisor must not be zero"
    );

    apn_seg_t rmdr = 0;

    if (size_divd == 1)
    {
        if (divisor > dividend[0])
        {
            quotient[0] = 0ULL;
            rmdr = dividend[0];
        }
        else if (divisor == dividend[0])
        {
            quotient[0] = 1ULL;
        }
        else
        {
            quotient[0] = dividend[0] / divisor;
            rmdr = dividend[0] % divisor;
        }

        return rmdr;
    }

    uint32_t shift_val = 0;
    apn_seg_t divd_shift_out = 0;

    if (!(divisor & (APN_SEG_HIGH_BIT)))
    {
        CLZ64(divisor, shift_val);
        APAC_ASSERT(shift_val != (APN_SEG_BITS));
    
        divisor <<= (apn_size_t)shift_val;
        rmdr = apn_lshift(dividend, dividend, size_divd, shift_val);
        divd_shift_out = rmdr;
    }

    // actual computation begins here xD

    apn_seg_t dvsr_recip = recip_word64_2by1(divisor);
    apn_seg_t temp_val = 0;

    for (apn_size_t j = size_divd - 1; j < size_divd; j--)
    {
        quotient[j] = udiv64_2by1(rmdr, dividend[j], divisor, dvsr_recip, &rmdr);
    }

    // no need to shift-down the divisor as it is pass by value

    if (shift_val)
    {
        rmdr >>= (apn_size_t)shift_val;
        apn_rshift(dividend, dividend, size_divd, shift_val);
        dividend[size_divd - 1] |= (divd_shift_out << (APN_SEG_BITS - shift_val));
    }

    return rmdr;
}