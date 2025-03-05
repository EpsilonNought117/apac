#include "misc.h"
#include <stdlib.h>

void apacInitDefault(void)
{
    setMemFuncs(&malloc, &realloc, &free);
    getCPUSpec();
}