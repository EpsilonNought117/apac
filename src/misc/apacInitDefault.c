#include "misc.h"

void apacInitDefault(void)
{
    setMemFuncs(NULL, NULL, NULL);
    getCPUSpec();
}