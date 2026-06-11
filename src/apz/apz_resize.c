#include "../header/apac_internal.h"

apac_err
apz_resize(
    apz_t* op1,
    ap_size_t new_size
)
{
    APAC_ASSERT(apac_allocator.custom_realloc != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(new_size != 0);
    APAC_ASSERT(new_size <= (APN_SIZE_MAX / sizeof(ap_dig_t)));

    apac_err result = APAC_OK;

    ap_dig_t* temp = (ap_dig_t*)apac_realloc(op1->num, new_size * sizeof(ap_dig_t));

    // failed to resize
    if (!temp) { result = APAC_OOM; goto func_end; }

    op1->num = temp;
    temp = NULL;
    op1->max_size = new_size;
    op1->curr_size = op1->curr_size < new_size ? op1->curr_size : new_size;

func_end:
    return result;
}