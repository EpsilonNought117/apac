#include "../../header/apac_internal.h"
#include "../headers/hidden_div.h"

apn_dig_t apn_basecase_div(
    apn_dig_t* quotient,
    apn_dig_t* dividend,
    const apn_dig_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr
)
{
    APAC_ASSERT(size_dvsr >= 2);
    APAC_ASSERT(size_divd > size_dvsr);
    APAC_ASSERT((divisor[size_dvsr - 1] & (APN_DIG_HIGH_BIT)));
    
    apn_size_t m = size_divd - size_dvsr;
    apn_size_t n = size_dvsr;
    apn_dig_t q_msd = 0ULL;

    int cmp_res = apn_cmp(dividend + m, divisor, n);
    
    if (cmp_res != -1)  // dividend >= (divisor * B ^ m) where B is (2 ^ (APN_DIG_BITS))
    {
        apn_sub_n(dividend + m, dividend + m, divisor, n);
        q_msd = 1ULL;
    }

    apn_dig_t dvsr1 = divisor[n - 1];
    apn_dig_t dvsr0 = divisor[n - 2];
    apn_dig_t dvsr_recip = apn_recip_word_3by2(dvsr1, dvsr0);

    for (apn_size_t j = m - 1; j < m; j--)
    {
        apn_dig_t sel_quot = apn_udiv_3by2_quot(
                                dividend[n + j],
                                dividend[n + j - 1],
                                dividend[n + j - 2],
                                dvsr1,
                                dvsr0,
                                dvsr_recip
                            );
        apn_dig_t borrow_out = apn_submul_one(dividend + j, divisor, n, sel_quot);

        if (borrow_out)
        {
            sel_quot--;
            apn_dig_t carry_out = apn_add(dividend + j, dividend + j, divisor, size_divd - j, n);
            dividend[n + j] += carry_out;
        }
    
        quotient[j] = sel_quot;
    }

    return q_msd;
}
