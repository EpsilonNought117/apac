#include "../header/apac_internal.h"

apac_err
apz_set_random(
    apz_t* op1,
    apn_dig_t bitcnt,
    int sign
)
{
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(bitcnt != 0);
    APAC_ALWAYS_ASSERT(op1->num != NULL);
    APAC_ASSERT(sign == APZ_POS || sign == APZ_NEG);

    apn_size_t digits_needed = (bitcnt + APN_DIG_BITS - 1) / APN_DIG_BITS;
    apac_err retval = APAC_OK;

    if (digits_needed > op1->max_size)
    {
        retval = apz_resize(op1, digits_needed);
        if (retval != APAC_OK) { goto func_end; }
    }

    apn_size_t frac = bitcnt % APN_DIG_BITS;

    apn_set_random(op1->num, digits_needed);

    if (frac)
    {
        op1->num[digits_needed - 1] &= ((1ULL << frac) - 1);
    }

    op1->max_size = digits_needed;
    op1->curr_size = digits_needed;
    op1->sign = sign;

func_end:
    return retval;
}