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

    result->num = (ap_dig_t*)apac_malloc(sizeof(ap_dig_t) * op1->max_size);

    if (!result->num) { retval = APAC_OOM; goto func_end; }

    result->max_size = op1->max_size;
    result->sign = op1->sign;
    result->curr_size = op1->curr_size;

    apn_cpy(result->num, op1->num, op1->curr_size);
    apn_set(&(result->num[result->curr_size]), (result->max_size - result->curr_size), 0);

func_end:
    return retval;
}