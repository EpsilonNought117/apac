#include "../../include/apac.h"

void apacDefaultInit(void)
{
	apacSetMemFuncs(NULL, NULL, NULL);
	apacGetCPUSpec();
	return;
}