#include "../../../include/apac.h"
#include "apn_thresholds.h"
#include "hidden_div.h"
#include "hidden_helpers.h"

// Balanced Divide and Conquer division scratch workspace
#define DNC_DIV_BALANCED_WS_SIZE(size)      ((size) + 1)

// Unbalanced Divide and Conquer division scratch workspace
#define DNC_DIV_UNBALANCED_WS_SIZE(size)    DNC_DIV_BALANCED_WS_SIZE(size)

apac_err apn_div(
    apn_seg_t* quotient,        // must be (size_divd - size_dvsr + 1) length
    apn_seg_t* remainder,       // must be size_dvsr length
    const apn_seg_t* dividend,
    const apn_seg_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr
)
{
    APAC_ASSERT(quotient != NULL);
    APAC_ASSERT(remainder != NULL);
    APAC_ASSERT(dividend != NULL);
    APAC_ASSERT(divisor != NULL);
    APAC_ASSERT(size_dvsr != 0);
    APAC_DETAILED_ASSERT(size_divd >= size_dvsr,
        "Expected size_divd >= size_dvsr, got size_divd (%zu) < size_dvsr (%zu)",
        (size_t)size_divd, (size_t)size_dvsr
    );

    apn_size_t size_quot = size_divd - size_dvsr + 1;
    apn_size_t size_rmdr = size_dvsr;

    APAC_NO_OVERLAP(quotient, size_quot, remainder, size_rmdr);
    APAC_NO_OVERLAP(quotient, size_quot, dividend, size_divd);
    APAC_NO_OVERLAP(quotient, size_quot, divisor, size_dvsr);
    APAC_NO_OVERLAP(dividend, size_divd, divisor, size_dvsr);
    APAC_NO_OVERLAP(divisor, size_dvsr, remainder, size_rmdr);
    APAC_DETAILED_ASSERT(divisor[size_dvsr - 1] != 0, 
        "Divisor is malformed, MSD cannot be zero!"
    );

    if ((size_divd == size_dvsr))
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
    else if (size_dvsr == 1)
    {
        remainder[0] = apn_div_one(quotient, remainder, dividend, divisor[0], size_divd);
        return APAC_OK;
    }

full_division:

    int shift_to_normalize = 0;         // flag if left-shift to normalize happened
    int dvsr_shift_val = 0;

    APAC_DETAILED_ASSERT(apac_malloc != NULL && apac_free != NULL,
        "Memory allocator not initialized: apacInit()/apacSetMemFuncs() not invoked!"
    );

    // unconditionally allocate extra segment, idea taken from the book
    // "hacker's delight" 2nd edition's multiprecision division algorithm
    apn_seg_t* temp_divd = apac_malloc((size_divd + 1) * sizeof(apn_seg_t));

    if (!temp_divd)
    {
        APAC_LOG_ERR("Memory allocation failure for temporary dividend in apn_div_rem!");
        return APAC_OOM;
    }

    apn_cpy(temp_divd, dividend, size_divd);
    temp_divd[size_divd] = 0ULL;
    
    if (!(divisor[size_dvsr - 1] & (1ULL << 63)))
    {
        CLZ64(dvsr_shift_val, divisor[size_dvsr - 1]);
        APAC_ASSERT(dvsr_shift_val != -1);

        apn_seg_t out_val = apn_lshift(divisor, divisor, size_dvsr, (apn_seg_t)dvsr_shift_val);
        APAC_ASSERT(out_val == 0);

        // if this step results in no shift-out val, then top segment of dividend stays zero
        temp_divd[size_divd] = apn_lshift(temp_divd, temp_divd, size_divd, (apn_seg_t)dvsr_shift_val);
        shift_to_normalize = 1; // set flag because normalization
    }

    apn_seg_t outval = 0;
    
    if ((size_divd - size_dvsr) < DNC_DIV_THRESHOLD)
    {
        outval = apn_basecase_div(quotient, temp_divd, divisor, size_divd + 1, size_dvsr);
    }
    else if ((size_divd - size_dvsr) <= size_divd)
    {
        apn_size_t ws_size = DNC_DIV_BALANCED_WS_SIZE(size_divd);
        apn_seg_t* temp_ws = apac_malloc(sizeof(apn_seg_t) * ws_size);

        if (!temp_ws)
        {
            apac_free(temp_divd);
            APAC_LOG_ERR("Memory allocation failure for scratch workspace in apn_div_rem!");
            return APAC_OOM;
        }

        apn_set(temp_ws, ws_size, 0);

        outval = apn_dnc_div_balanced(quotient, temp_divd, divisor, size_divd + 1, size_dvsr, temp_ws);
        apac_free(temp_ws);
    }
    else
    {
        apn_size_t ws_size = DNC_DIV_UNBALANCED_WS_SIZE(size_divd);
        apn_seg_t* temp_ws = apac_malloc(sizeof(apn_seg_t) * ws_size);

        if (!temp_ws)
        {
            apac_free(temp_divd);
            APAC_LOG_ERR("Memory allocation failure for scratch workspace in apn_div_rem!");
            return APAC_OOM;
        }

        apn_set(temp_ws, ws_size, 0);

        outval = apn_dnc_div_unbalanced(quotient, temp_divd, divisor, size_divd + 1, size_dvsr, temp_ws);
        apac_free(temp_ws);
    }

    if (!shift_to_normalize)
    {
        quotient[size_quot - 1] = outval;
    }

    apn_cpy(remainder, temp_divd, size_rmdr);
    apac_free(temp_divd);

    if (shift_to_normalize)
    {
        apn_seg_t shift_out = apn_rshift(remainder, remainder, size_rmdr, (apn_seg_t)dvsr_shift_val);
        APAC_ASSERT(shift_out == 0);
    }

    return APAC_OK;
}