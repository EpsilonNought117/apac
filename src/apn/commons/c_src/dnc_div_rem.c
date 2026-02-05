#include "../c_headers/hidden_div.h"
#include "../c_headers/apn_thresholds.h"

apn_seg_t apn_dnc_div_balanced(
    apn_seg_t* quotient,    // assume (size_divd - size_dvsr) digits in quotient
    apn_seg_t* dividend,
    const apn_seg_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr,
    apn_seg_t* temp
)
{
    APAC_ASSERT(temp != NULL);
    APAC_ASSERT(size_dvsr < size_divd);
    APAC_ASSERT(size_dvsr >= (size_divd - size_dvsr));

    apn_size_t m = size_divd - size_dvsr;
    apn_size_t n = size_dvsr;

    // On the first call, quotient has (m + 1) segments
    // but we only use m segments since the (+1) segments will
    // not be available in recursive calls
    
    apn_seg_t q1_msd = 0, q0_msd = 0;

    if (m < DNC_DIV_THRESHOLD)
    {
        apn_seg_t q_msd = apn_basecase_div(quotient, dividend, divisor, size_divd, size_dvsr);
        return q_msd;
    }

    APAC_ASSERT(m > 2);

    apn_size_t k = m >> 1;  // floor(size_divd - size_dvsr)
    const apn_seg_t* b0 = divisor, * b1 = divisor + k;
    apn_seg_t* q0 = quotient, * q1 = quotient + k;

    // q1_msd will be at most 1-bit
    q1_msd = apn_dnc_div_balanced(q1, dividend + 2 * k, b1, (n + m) - 2 * k, n - k, temp);
    APAC_ASSERT(q1_msd <= 1);
    
    // temp[0 : (m - 1)] = q1 * b0 
    apn_mul(temp, q1, b0, m - k, k);

    // temp[(m - k) : m] = b0 * q1_msd

    if (q1_msd)
    {
        temp[m] += apn_add_n(temp + m - k, temp + m - k, b0, k);
    }

    int cmp_res = apn_cmp(dividend + k, temp, n + 1);

    while (cmp_res == -1)
    {
        q1_msd -= apn_sub_one(q1, q1, m - k, 1);
        dividend[n + k] += apn_add_n(dividend + k, dividend + k, divisor, n);
        cmp_res = apn_cmp(dividend + k, temp, n + 1);
    }

    apn_seg_t borrow_out = apn_sub_n(dividend + k, dividend + k, temp, n + 1);
    APAC_ASSERT(borrow_out == 0);

    apn_set(temp, n + 1, 0ULL);

    q0_msd = apn_dnc_div_balanced(q0, dividend + k, b1, n, n - k, temp);

    apn_mul_n(temp, b0, q0, k);

    if (q0_msd)
    {
        temp[2 * k] += apn_add_n(temp + k, temp + k, b0, k);
    }

    cmp_res = apn_cmp(dividend, temp, n + 1);

    while (cmp_res == -1)
    {
        q0_msd -= apn_sub_one(q0, q0, k, 1);
        dividend[n] += apn_add_n(dividend, dividend, divisor, n);
        cmp_res = apn_cmp(dividend, temp, n + 1);
    }

    borrow_out = apn_sub_n(dividend, dividend, temp, n + 1);
    APAC_ASSERT(borrow_out == 0);
    
    // this could happen in some pathological cases
    if (q0_msd)
    {
        q0_msd = apn_add_one(q1, q1, m - k, q0_msd);
    }

    apn_set(temp, n + 1, 0ULL);

    return (q1_msd | q0_msd);
}

apn_seg_t apn_dnc_div_unbalanced(
    apn_seg_t* quotient,    // assume (size_divd - size_dvsr) digits in quotient
    apn_seg_t* dividend,
    const apn_seg_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr,
    apn_seg_t* temp
)
{
    APAC_ASSERT(temp != NULL);
    APAC_ASSERT(size_dvsr < size_divd);
    APAC_ASSERT(size_dvsr < (size_divd - size_dvsr));

    apn_size_t m = size_divd - size_dvsr;
    apn_size_t orig_m = m;
    apn_size_t n = size_dvsr;
    apn_seg_t q_msd = 0;

    apn_seg_t* q_curr = quotient + m - n;
    apn_seg_t q1_msd = apn_dnc_div_balanced(q_curr, dividend + m - n, divisor, 2 * n, n, temp);

    m -= n;

    while (m > n)
    {
        q_curr = quotient + m - n;
        q_msd = apn_dnc_div_balanced(q_curr, dividend + m - n, divisor, 2 * n, n, temp);
        
        if (q_msd)
        {
            q_msd = apn_add_one(q_curr + n, q_curr + n, orig_m + 1 - m, q_msd);
            q1_msd |= q_msd;
        }

        m -= n;
    }

    q_msd = apn_dnc_div_balanced(quotient, dividend, divisor, m + n, n, temp);

    q_msd = apn_add_one(quotient, quotient, orig_m + 1 - m, q_msd);
    q1_msd |= q_msd;

    return q1_msd;
}