#include "../../include/apac.h"

struct apac_alloc_t
{
    apac_malloc_t custom_malloc;
    apac_realloc_t custom_realloc;
    apac_free_t custom_free;
    void* ctx;
};

apac_alloc_t apac_allocator = { 0 };

// libc malloc wrapper
static inline void* libc_malloc_wrapper(size_t new_size, void* ctx)
{
    (void)ctx;
    return malloc(new_size);
}

// libc realloc wrapper
static inline void* libc_realloc_wrapper(void* old_arr, size_t new_size, void* ctx)
{
    (void)ctx;
    return realloc(old_arr, new_size);
}

// libc free wrapper
static inline void libc_free_wrapper(void* old_arr, void* ctx)
{
    (void)ctx;
    return free(old_arr);
}

void apac_init_allocator(
    apac_malloc_t malloc_ptr,
    apac_realloc_t realloc_ptr,
    apac_free_t free_ptr,
    void* ctx_ptr
)
{
    APAC_ASSERT(
        (!malloc_ptr && !realloc_ptr && !free_ptr && !ctx_ptr) ||
        (malloc_ptr && realloc_ptr && free_ptr && ctx_ptr)
    );

    if (malloc_ptr && realloc_ptr && free_ptr && ctx_ptr)
    {
        apac_allocator.custom_malloc = malloc_ptr;
        apac_allocator.custom_realloc = realloc_ptr;
        apac_allocator.custom_free = free_ptr;
        apac_allocator.ctx = ctx_ptr;
    }
    else
    {
        apac_allocator.custom_malloc = libc_malloc_wrapper;
        apac_allocator.custom_realloc = libc_realloc_wrapper;
        apac_allocator.custom_free = libc_free_wrapper;
        apac_allocator.ctx = NULL;
    }
}

inline void* apac_malloc(size_t in_size)
{
    return apac_allocator.custom_malloc(in_size, apac_allocator.ctx);
}

inline void* apac_realloc(void* old_arr, size_t in_size)
{
    return apac_allocator.custom_realloc(old_arr, in_size, apac_allocator.ctx);
}

inline void apac_free(void* old_arr)
{
    return apac_allocator.custom_free(old_arr, apac_allocator.ctx);
}