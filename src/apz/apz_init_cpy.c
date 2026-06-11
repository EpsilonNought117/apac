#include "../header/apac_internal.h"

apac_err
apz_init_cpy(
    apz_t* result,
    const apz_t* op1
)
{
    APAC_ASSERT(apac_allocator.custom_malloc != NULL);
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);

    apac_err retval = APAC_OK;

    result->num = (ap_dig_t*)apac_malloc(sizeof(ap_dig_t) * op1->max);

    if (!result->num) { retval = APAC_OOM; goto func_end; }

    result->max = op1->max;
    result->sign = op1->sign;
    result->curr = op1->curr;

    apn_cpy(result->num, op1->num, op1->curr);
    apn_set(&(result->num[result->curr]), (result->max - result->curr), 0);

func_end:
    return retval;
}