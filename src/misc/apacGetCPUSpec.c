#include "../../include/apac.h"

apac_cpu_params curr_cpu = { 0 };

#if (defined(_M_X64) || defined(_M_AMD64))

#include "../apn/x64/x64_hidden_funcs.h"

extern void zen4_set_params(void);
extern void generic_x64_set_params(void);

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
			generic_x64_set_params();
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
		{
			int baseModel = (signature >> 4) & 0xF;
			int extendedModel = (signature >> 16) & 0xF;
			int model = (extendedModel << 4) | baseModel;

			switch (model)
			{
			default:
				generic_x64_set_params();
				break;
			}
		}
		break;

		default:
			generic_x64_set_params();
			break;
		}
	}
	else
	{
		generic_x64_set_params();
	}

	return;
}

#endif