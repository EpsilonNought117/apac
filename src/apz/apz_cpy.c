#include "../header/apac_internal.h"

apac_err
apz_cpy(
    apz_t* result,
    const apz_t* op1
)
{
    APAC_ASSERT(apac_allocator.custom_realloc != NULL);
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);

    /* assumes result has been validly initalized already */

    apac_err retval = APAC_OK;
}