#include "../header/apac_internal.h"

apac_err
apz_init_random(
    apz_t* op1,
    ap_size_t bit_cnt,
    int sign
)
{
    APAC_ASSERT(apac_allocator.custom_malloc != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(bit_cnt != 0);
    APAC_ASSERT(bit_cnt < (APN_SIZE_MAX) / (APN_DIG_BITS * sizeof(ap_dig_t)));

    ap_size_t frac = bit_cnt % 64;
    ap_size_t whole = frac ? (bit_cnt / 64) : (bit_cnt / 64) + 1;
    apac_err result = APAC_OK;

    op1->num = (ap_dig_t*)apac_malloc(sizeof(ap_dig_t) * whole);

    if (!(op1->num))
    {
        result = APAC_OOM;
        goto func_end;
    }

    apn_set_random(op1->num, whole);

    if (frac)
    {
        op1->num[whole - 1] &= ((1ULL << frac) - 1);
    }

    op1->curr = whole;
    op1->max = whole;
    op1->sign = sign;

func_end:
    return result;
}