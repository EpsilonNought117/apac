#include "misc.h"

void setMemFuncs(
    void* (*ptr1)(size_t),
    void* (*ptr2)(void*, size_t),
    void (*ptr3)(void*)
)
{
    if (!ptr1 && !ptr2 && !ptr3)
    {
        apac_malloc = &malloc;
        apac_realloc = &realloc;
        apac_free = &free;
    }
    else if (ptr1 && ptr2 && ptr3)
    {
        apac_malloc = ptr1;
        apac_realloc = ptr2;
        apac_free = ptr3;
    }
    else
    {
        APAC_REPORT_ERR("Improper arguments passed in setMemFuncs.\nDefaulting to malloc and friends.");
        apac_malloc = &malloc;
        apac_realloc = &realloc;
        apac_free = &free;   
    }

    return;
}
