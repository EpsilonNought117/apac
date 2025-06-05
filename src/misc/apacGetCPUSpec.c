#include "../../include/apac.h"

__apac_cpu_params curr_cpu = { 0 };

extern void zen4_set_params(void);

#if defined(_M_X64) || defined(_M_AMD64)

// x64/AMD64 Version

void apacGetCPUSpec(void)
{
	int specBuffer[4] = { 0 };

	__cpuid(specBuffer, 0x0);



}

#elif defined(_M_ARM64) 

// ARM64 version

#endif