#include "../header/apac_internal.h"

apac_err
apz_free(
    apz_t* op1
)
{
    APAC_ASSERT(op1 != NULL);
    
    apac_free(op1->num);

    op1->num = NULL;
    op1->curr_size = 0;
    op1->max_size = 0;
    op1->sign = APZ_POS;

    return APAC_OK;
}