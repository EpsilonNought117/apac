#include "../headers/hidden_div.h"

// Balanced Divide and Conquer division scratch workspace
#define DNC_DIV_BALANCED_WS_SIZE(size)      ((size) + 1)

apac_err apn_div(
    apn_dig_t* quotient,        // must be (size_divd + size_divd_frac - size_dvsr + 1) length
    apn_dig_t* remainder,       // must be size_dvsr length
    const apn_dig_t* dividend,
    const apn_dig_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_divd_frac,  // this size can be zero
    apn_size_t size_dvsr
)
{
    APAC_ASSERT(quotient != NULL);
    APAC_ASSERT(remainder != NULL);
    APAC_ASSERT(dividend != NULL);
    APAC_ASSERT(divisor != NULL);
    APAC_ASSERT(size_dvsr != 0);
    APAC_ASSERT(size_divd >= size_dvsr);

    apn_size_t size_quot = size_divd + size_divd_frac - size_dvsr + 1;
    apn_size_t size_rmdr = size_dvsr;

    APAC_NO_OVERLAP(quotient, size_quot, remainder, size_rmdr);
    APAC_NO_OVERLAP(quotient, size_quot, dividend, size_divd);
    APAC_NO_OVERLAP(quotient, size_quot, divisor, size_dvsr);
    APAC_NO_OVERLAP(dividend, size_divd, divisor, size_dvsr);
    APAC_NO_OVERLAP(divisor, size_dvsr, remainder, size_rmdr);
    APAC_ASSERT(divisor[size_dvsr - 1] != 0);

    apn_dig_t dvsr_shift_val = 0;

    if (size_dvsr == 1)
    {
        remainder[0] = apn_div_one(quotient, dividend, divisor[0], size_divd, size_divd_frac);
        return APAC_OK;
    }
    if (size_divd == size_dvsr)
    {
        int cmp_res = apn_cmp(dividend, divisor, size_divd);
        
        if (cmp_res == -1)
        {
            quotient[0] = 0ULL;
            apn_cpy(remainder, dividend, size_rmdr);
        }
        else if (cmp_res == 0)
        {
            quotient[0] = 1ULL;
            apn_set(remainder, size_rmdr, 0ULL);
        }
        else
        {
            goto full_division;
        }

        return APAC_OK;
    }

full_division:

    APAC_ASSERT(apac_allocator.custom_malloc != NULL && apac_allocator.custom_free != NULL);

    // unconditionally allocate extra segment, idea taken from the book
    // "hacker's delight" 2nd edition's multiprecision division algorithm
    apn_dig_t* temp_space = apac_malloc((size_dvsr + size_divd_frac + size_divd + 1) * sizeof(apn_dig_t));

    if (!temp_space) { return APAC_OOM; }

    apn_dig_t* temp_dvsr = temp_space;
    apn_dig_t* temp_divd = &temp_space[size_dvsr];
    
    apn_cpy(temp_dvsr, divisor, size_dvsr);
    if (size_divd_frac != 0) { apn_set(temp_divd, size_divd_frac, 0); } // must check
    apn_cpy(&temp_divd[size_divd_frac], dividend, size_divd);
    temp_divd[size_divd + size_divd_frac] = 0ULL;
    
    apn_size_t new_size_divd = size_divd + size_divd_frac + 1;

    if (!(temp_dvsr[size_dvsr - 1] & (APN_DIG_HIGH_BIT)))
    {
        CLZ(temp_dvsr[size_dvsr - 1], dvsr_shift_val);
        APAC_ASSERT(dvsr_shift_val != (APN_DIG_BITS));

        apn_dig_t out_val = apn_lshift(temp_dvsr, temp_dvsr, size_dvsr, (apn_dig_t)dvsr_shift_val);
        APAC_ASSERT(out_val == 0);

        // if this step results in no shift-out val, then top segment of dividend stays zero
        temp_divd[new_size_divd - 1] = apn_lshift(temp_divd, temp_divd, new_size_divd - 1, (apn_dig_t)dvsr_shift_val);
    }

    // actual division below
    
    if ((new_size_divd - size_dvsr) < DNC_DIV_THRESHOLD)
    {
        apn_basecase_div(quotient, temp_divd, temp_dvsr, new_size_divd, size_dvsr);
    }
    else if (size_dvsr >= (new_size_divd - size_dvsr))
    {
        apn_size_t ws_size = DNC_DIV_BALANCED_WS_SIZE(size_dvsr);
        apn_dig_t* temp_ws = apac_malloc(sizeof(apn_dig_t) * ws_size);

        if (!temp_ws)
        {
            apac_free(temp_space);
            return APAC_OOM;
        }

        apn_set(temp_ws, ws_size, 0);

        apn_dnc_div_balanced(quotient, temp_divd, temp_dvsr, new_size_divd, size_dvsr, temp_ws);
        apac_free(temp_ws);
    }
    else
    {
        apn_size_t ws_size = DNC_DIV_BALANCED_WS_SIZE(size_dvsr);
        apn_dig_t* temp_ws = apac_malloc(sizeof(apn_dig_t) * ws_size);

        if (!temp_ws)
        {
            apac_free(temp_space);
            return APAC_OOM;
        }

        apn_set(temp_ws, ws_size, 0);

        apn_dnc_div_unbalanced(quotient, temp_divd, temp_dvsr, new_size_divd, size_dvsr, temp_ws);
        apac_free(temp_ws);
    }

    if (dvsr_shift_val)
    {
        apn_dig_t shift_down = apn_rshift(temp_divd, temp_divd, size_rmdr, (apn_dig_t)dvsr_shift_val);
        APAC_ASSERT(shift_down == 0);
    }

    apn_cpy(remainder, temp_divd, size_rmdr);
    apac_free(temp_space);

    return APAC_OK;
}