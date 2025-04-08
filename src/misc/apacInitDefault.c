#include "../../include/apac.h"

void apacInitDefault(void)
{
    setMemFuncs(NULL, NULL, NULL);
    getCPUSpec();
}