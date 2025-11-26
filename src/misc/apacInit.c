#include "../../include/apac.h"

void apacInit(void)
{
	apacSetMemFuncs(NULL, NULL, NULL);
	apacGetCPUSpec();
	return;
}