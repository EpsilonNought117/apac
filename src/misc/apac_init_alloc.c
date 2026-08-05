#include "../header/apac_internal.h"

// libc malloc wrapper
static inline void* libc_malloc_wrapper(void* ctx, size_t new_size)
{
    (void)ctx;
    return malloc(new_size);
}

// libc realloc wrapper
static inline void* libc_realloc_wrapper(void* ctx, void* old_arr, size_t new_size)
{
    (void)ctx;
    return realloc(old_arr, new_size);
}

// libc free wrapper
static inline void libc_free_wrapper(void* ctx, void* old_arr)
{
    (void)ctx;
    free(old_arr);
}

apac_alloc_t apac_allocator = {

    .custom_malloc = &libc_malloc_wrapper,
    .custom_realloc = &libc_realloc_wrapper,
    .custom_free = &libc_free_wrapper,
    .ctx = NULL
};

void apac_init_allocator(
    apac_malloc_t malloc_ptr,
    apac_realloc_t realloc_ptr,
    apac_free_t free_ptr,
    void* ctx_ptr
)
{
    APAC_ASSERT(
        (!malloc_ptr && !realloc_ptr && !free_ptr) ||
        ( malloc_ptr &&  realloc_ptr &&  free_ptr)
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

void* apac_malloc(size_t new_size)
{
    return apac_allocator.custom_malloc(apac_allocator.ctx, new_size);
}

void* apac_realloc(void* old_buffer, size_t new_size)
{
    return apac_allocator.custom_realloc(apac_allocator.ctx, old_buffer, new_size);
}

void apac_free(void* old_buffer)
{
    apac_allocator.custom_free(apac_allocator.ctx, old_buffer);
}