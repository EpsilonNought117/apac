#include "../header/apac_internal.h"

apac_err 
apz_init(
    apz_t* op1, 
    ap_size_t size
)
{
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(size <= (APN_SIZE_MAX) / sizeof(ap_dig_t));

    apac_err result = APAC_OK;

    op1->num = (ap_dig_t*)apac_malloc(sizeof(ap_dig_t) * size);

    if (!(op1->num))
    {
        result = APAC_OOM;
        goto func_end;
    }

    apn_set(op1->num, size, 0); // zero out array
    op1->max = size;
    op1->curr = 0;
    op1->sign = APZ_POS; // given that 0 is in Z+

func_end:
    return result;
}