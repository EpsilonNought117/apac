#include "../header/apac_internal.h"

apac_err
apz_set_to_zero(
    apz_t* op1
)
{
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(op1->max_size != 0);

    apn_set(op1->num, op1->max_size, 0);

    op1->curr_size = 0;
    op1->sign = APZ_POS;

    return APAC_OK;
}