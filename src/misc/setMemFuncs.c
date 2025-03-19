#include "misc.h"

void* (*apac_malloc)(size_t) = NULL;
void* (*apac_realloc)(void*, size_t) = NULL;
void (*apac_free)(void*) = NULL;

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
        APAC_REPORT_ERR("Improper arguments passed in setMemFuncs.\nDefaulting to <stdlib.h> malloc and friends.");
        apac_malloc = &malloc;
        apac_realloc = &realloc;
        apac_free = &free;   
    }

    return;
}
