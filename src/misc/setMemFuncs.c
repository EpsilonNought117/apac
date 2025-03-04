#include "misc.h"

void setMemFuncs(
    void* (*ptr1)(size_t),
    void* (*ptr2)(void*, size_t),
    void (*ptr3)(void*)
)
{
    if (!ptr1 && !ptr2 && !ptr3)
    {
        apac_malloc = malloc;
        apac_realloc = realloc;
        apac_free = free;
    }
    else
    {
        
    }
}
