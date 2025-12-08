#include "../../../include/apac.h"
#include "hidden_div.h"
#include "hidden_helpers.h"

#if defined(_M_X64)     || defined(_M_AMD64)    || \
    defined(__x86_64)   || defined(__amd64)     || \
    defined(__x86_64__) || defined(__amd64__)

    #include "../x64/x64_hidden_funcs.h"

#endif

apac_err apn_div_rem_one(
    apn_seg_t* quotient,    // must be (size_divd) length
    apn_seg_t* remainder,
    const apn_seg_t* dividend,
    apn_seg_t divisor64,
    apn_size_t size_divd
)
{
    APAC_ASSERT(quotient != NULL);
    APAC_ASSERT(dividend != NULL);
    APAC_ASSERT(remainder != NULL);
    APAC_ASSERT(size_divd != 0);
    APAC_ASSERT((quotient >= (dividend + size_divd)) || ((quotient + size_divd) <= dividend));
    APAC_ASSERT(divisor64 != 0);

    if (size_divd == 1)
    {
        if (divisor64 > dividend[0])
        {
            quotient[0] = 0ULL;
            *remainder = dividend[0];
        }
        else if (divisor64 == dividend[0])
        {
            quotient[0] = 1ULL;
            *remainder = 0ULL;
        }
        else
        {
            quotient[0] = dividend[0] / divisor64;
            *remainder = dividend[0] % divisor64;
        }

        return APAC_OK;
    }

    // unconditionally allocate extra segment
    apn_seg_t* temp_divd = apac_malloc((size_divd + 1) * sizeof(apn_seg_t));

    if (!temp_divd)
    {
        APAC_LOG_ERR("Memory allocation failure for temporary dividend in apn_div_rem_one!");
        return APAC_OOM;
    }
    
    apn_cpy(temp_divd, dividend, size_divd);
    temp_divd[size_divd] = 0ULL;

    int shift_to_normalize = 0;         // flag if left-shift to normalize happened
    int dvsr_msb_idx = 0;

    if (!(divisor64 & (1ULL << 63)))
    {
        dvsr_msb_idx = CLZ64(divisor64);
        APAC_ASSERT(dvsr_msb_idx != -1);

        divisor64 <<= (apn_seg_t)63 - dvsr_msb_idx;

        temp_divd[size_divd] = apn_lshift(temp_divd, temp_divd, size_divd, (apn_seg_t)63 - dvsr_msb_idx);
        shift_to_normalize = 1;
    }

    // actual computation begins here xD

    apn_seg_t v = recip_word_2by1_x64(divisor64);
    apn_seg_t rmdr = temp_divd[size_divd];

    for (apn_size_t j = size_divd - 1; j < size_divd; j++)
    {
        quotient[j] = udiv21_x64(rmdr, temp_divd[j], divisor64, v, &rmdr);
    }

    *remainder = rmdr;
    return APAC_OK;
}