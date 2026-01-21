#include "../../include/apac.h"

void apac_init(void)
{
	apac_set_mem_funcs(NULL, NULL, NULL);
	apac_get_cpu_spec();
	return;
}