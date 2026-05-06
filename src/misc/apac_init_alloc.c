#include "../headers/apac_internal.h"

// libc malloc wrapper
inline void* apac_malloc(size_t new_size, void* ctx)
{
    (void*)ctx;
    return malloc(new_size);
}

// libc realloc wrapper
inline void* apac_realloc(void* old_arr, size_t new_size, void* ctx)
{
    (void*)ctx;
    return realloc(old_arr, new_size);
}

// libc free wrapper
inline void apac_free(void* old_arr, void* ctx)
{
    (void*)ctx;
    return free(old_arr);
}

void apac_init_allocator(
    apac_malloc_t malloc_ptr,
    apac_realloc_t realloc_ptr,
    apac_free_t free_ptr,
    void* ctx_ptr
)
{
    if (malloc_ptr && realloc_ptr && free_ptr && ctx_ptr)
    {
        apac_allocator.custom_malloc = malloc_ptr;
        apac_allocator.custom_realloc = realloc_ptr;
        apac_allocator.custom_free = free_ptr;
        apac_allocator.ctx = ctx_ptr;
    }
    else if (!malloc_ptr && !realloc_ptr && !free_ptr && !ctx_ptr)
    {
        apac_allocator.custom_malloc = apac_malloc;
        apac_allocator.custom_realloc = apac_realloc;
        apac_allocator.custom_free = apac_free;
        apac_allocator.ctx = NULL;
    }
    else
    {
        APAC_LOG_ERR(
            "Invalid arguments passed to apac_init_alloc.\n"
            "Either every argument should be NULL for libc malloc & friends to "
            "be used or every argument should be a valid function pointer.\n"
        );
        APAC_ALWAYS_ASSERT(
            (!malloc_ptr && !realloc_ptr && !free_ptr && !ctx_ptr) ||
            (malloc_ptr && realloc_ptr && free_ptr && ctx_ptr)
        );
    }
}