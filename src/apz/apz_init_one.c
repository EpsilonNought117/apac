#include "../header/apac_internal.h"

apac_err
apz_init_one(
    apz_t* op1,
    ap_dig_t val,
    int sign
)
{
    APAC_ASSERT(apac_allocator.custom_malloc != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT((sign == APZ_POS) || (sign == APZ_NEG));
    APAC_ASSERT(val != 0);

    apac_err result = APAC_OK;
    op1->num = (ap_dig_t*)apac_malloc(sizeof(ap_dig_t));

    if(!(op1->num))
    {
        result = APAC_OOM;
        goto func_end;
    }

    op1->num[0] = val;
    op1->max_size = 1;
    op1->curr_size = 1;
    op1->sign = sign;

func_end:
    return result;
}   
