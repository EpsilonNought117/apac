#include "../../include/apac.h"
#include "../apn/x64/x64_hidden_funcs.h"
#include "../apn/x64/x64_uarch_params.h"

__apac_cpu_params curr_cpu = { 0 };

#if defined(_M_X64) || defined(_M_AMD64)

// x64/AMD64 Version

void apacGetCPUSpec(void)
{
	int cpuInfo[4] = { 0 };

	__cpuid(cpuInfo, 0x0);

	if (
		cpuInfo[1] == 0x68747541 &&		// 'Auth'
		cpuInfo[3] == 0x69746E65 &&		// 'enti'
		cpuInfo[2] == 0x444D4163		// 'cAMD'
		)
	{
		// Get CPU signature and extract family
		__cpuid(cpuInfo, 0x1);
		int signature = cpuInfo[0];

		int baseFamily = (signature >> 8) & 0xF;
		int extendedFamily = (signature >> 20) & 0xFF;
		int family = (baseFamily < 0xF) ? baseFamily : baseFamily + extendedFamily;

		switch (family)
		{
		case 0x19:				// Zen 4 Uarch
			zen4_set_params();
			break;

		default:
			break;
		}
	}
	else if (
		cpuInfo[1] == 0x756E6547 &&		// 'Genu'
		cpuInfo[3] == 0x49656E69 &&		// 'ineI'
		cpuInfo[2] == 0x6C65746E		// 'ntel'
		)
	{
		__cpuid(cpuInfo, 0x1);
		int signature = cpuInfo[0];

		int baseFamily = (signature >> 8) & 0xF;
		int extendedFamily = (signature >> 20) & 0xFF;
		int family = (baseFamily != 0xF) ? baseFamily : baseFamily + extendedFamily;
	
		switch (family)
		{
		case 0x6:

		default:
			break;
		}
	}

	return;
}

#elif defined(_M_ARM64) 

// ARM64 version

#endif