#include "../header/apac_internal.h"

apac_err
apz_set_one(
    apz_t* op1,
    apn_dig_t val,
    int sign
)
{
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(sign == APZ_POS || sign == APZ_NEG);

    apac_err retval = APAC_OK;

    if (op1->max_size == 0)
    {
        op1->num = (apn_dig_t*)apac_malloc(1 * sizeof(apn_dig_t));
        if (!op1->num) { retval = APAC_OOM; goto func_end; }
        op1->max_size = 1;
    }

    op1->num[0] = val;
    op1->sign = sign;
    op1->curr_size = 1;

func_end:
    return retval;
}