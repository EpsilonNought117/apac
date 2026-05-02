#include "../../include/apac.h"

// libc malloc wrapper
inline void* apac_malloc(size_t new_size, void* ctx)
{
    return malloc(new_size);
}

// libc realloc wrapper
inline void* apac_realloc(void* old_arr, size_t new_size, void* ctx)
{
    return realloc(old_arr, new_size);
}

// libc free wrapper
inline void apac_free(void* old_arr, void* ctx)
{
    return free(old_arr);
}

apac_alloc_t apac_init_alloc(
    apac_malloc_t malloc_ptr,
    apac_realloc_t realloc_ptr,
    apac_free_t free_ptr,
    void* ctx_ptr
)
{
    apac_alloc_t new_alloc;

    if (malloc_ptr && realloc_ptr && free_ptr && ctx_ptr)
    {
        new_alloc.custom_malloc = malloc_ptr;
        new_alloc.custom_realloc = realloc_ptr;
        new_alloc.custom_free = free_ptr;
        new_alloc.ctx = ctx_ptr;
    }
    else if (!malloc_ptr && !realloc_ptr && !free_ptr && !ctx_ptr)
    {
        new_alloc.custom_malloc = apac_malloc;
        new_alloc.custom_realloc = apac_realloc;
        new_alloc.custom_free = apac_free;
        new_alloc.ctx = NULL;
    }
    else
    {
        APAC_LOG_ERR(
            "Invalid arguments passed to apac_init_alloc!\n"
            "Either every argument should be NULL for libc malloc & friends to "
            "be used or every argument should be a valid function pointer!\n"
        );
        APAC_ALWAYS_ASSERT(
            (!malloc_ptr && !realloc_ptr && !free_ptr && !ctx_ptr) ||
            (malloc_ptr && realloc_ptr && free_ptr && ctx_ptr)
        );
    }

    return new_alloc;
}