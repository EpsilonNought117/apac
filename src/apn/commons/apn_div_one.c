#include "../../../include/apac.h"
#include "hidden_div.h"
#include "hidden_helpers.h"

apn_seg_t apn_div_one(
    apn_seg_t* quotient,    // must be (size_divd) length
    const apn_seg_t* dividend,
    apn_seg_t divisor64,
    apn_size_t size_divd
)
{
    APAC_ASSERT(quotient != NULL);
    APAC_ASSERT(dividend != NULL);
    APAC_ASSERT(size_divd != 0);
    APAC_NO_OVERLAP(quotient, size_divd, dividend, size_divd);
    APAC_DETAILED_ASSERT(divisor64 != 0,
        "Malformed divisor: divisor64 must not be zero"
    );


    apn_seg_t rmdr = 0ULL;

    if (size_divd == 1)
    {
        if (divisor64 > dividend[0])
        {
            quotient[0] = 0ULL;
            rmdr = dividend[0];
        }
        else if (divisor64 == dividend[0])
        {
            quotient[0] = 1ULL;
        }
        else
        {
            quotient[0] = dividend[0] / divisor64;
            rmdr = dividend[0] % divisor64;
        }

        return rmdr;
    }

    int shift_to_normalize = 0;         // flag if left-shift to normalize happened
    uint32_t shift_val = 0;

    if (!(divisor64 & (1ULL << 63)))
    {
        shift_val = clz64(divisor64);
        APAC_ASSERT(shift_val != 64);

        divisor64 <<= (apn_size_t)shift_val;

        rmdr = dividend[size_divd - 1] >> ((apn_size_t)64 - shift_val);
        shift_to_normalize = 1;
    }

    // actual computation begins here xD

    apn_seg_t dvsr_recip = recip_word_2by1(divisor64);
    apn_seg_t temp_val = 0;

    for (apn_size_t j = size_divd - 1; j >= 1; j--)
    {
        temp_val = (dividend[j] << (apn_size_t)shift_val) | (dividend[j - 1] >> ((apn_size_t)64 - shift_val));
        quotient[j] = udiv21(rmdr, temp_val, divisor64, dvsr_recip, &rmdr);
    }

    temp_val = (dividend[0] << (apn_size_t)shift_val);
    quotient[0] = udiv21(rmdr, temp_val, divisor64, dvsr_recip, &rmdr);

    if (shift_to_normalize)
    {
        rmdr >>= (apn_size_t)shift_val;
    }

    return rmdr;
}