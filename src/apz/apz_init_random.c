#include "../header/apac_internal.h"

apac_err
apz_init_random(
    apz_t* op1,
    apn_size_t bit_cnt,
    int sign
)
{
    APAC_ASSERT(apac_allocator.custom_malloc != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(bit_cnt != 0);
    APAC_ASSERT(bit_cnt <= APN_SIZE_MAX - APN_DIG_BITS + 1);
    APAC_ASSERT(sign == APZ_POS || sign == APZ_NEG);

    apac_err result = APAC_OK;

    apn_size_t frac = bit_cnt % APN_DIG_BITS;
    apn_size_t whole = (bit_cnt + APN_DIG_BITS - 1) / APN_DIG_BITS;

    op1->num = (apn_dig_t*)apac_malloc(sizeof(apn_dig_t) * whole);

    if (!(op1->num))
    {
        result = APAC_OOM;
        goto func_end;
    }

    apn_set_random(op1->num, whole);

    // if top-most limb isn't supposed to
    // be full 64-bits reduce it to frac-bits
    if (frac)
    {
        op1->num[whole - 1] &= ((1ULL << frac) - 1);
    }

    op1->curr_size = whole;
    op1->max_size = whole;
    op1->sign = sign;

func_end:
    return result;
}