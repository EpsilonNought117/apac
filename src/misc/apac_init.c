#include "../header/apac_internal.h"

void apac_init(void)
{
    apac_init_allocator(NULL, NULL, NULL, NULL);
    apac_get_cpu_spec();
    return;
}