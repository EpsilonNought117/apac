#include "../../include/apac.h"

apac_cpu_params curr_cpu = { 0 };

// for x86-64/AMD64 ISA

#if (defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) || defined(__amd64))

#include "../apn/x64/x64_hidden_funcs.h"

#if defined(_MSC_VER)
	
	#define CPUID(cpuInfo, leaf)			__cpuid(cpuInfo, leaf)
	#define CPUIDEX(cpuInfo, leaf, subleaf)	__cpuidex(cpuInfo, leaf, subleaf)

#elif defined(__GNUC__) || defined(__clang__)

	#include <cpuid.h>

	#define CPUID(cpuInfo, leaf)			__get_cpuid(leaf, &cpuInfo[0], &cpuInfo[1], &cpuInfo[2], &cpuInfo[3])
	#define CPUIDEX(cpuInfo, leaf, subleaf) __get_cpuid_count(leaf, subleaf, &cpuInfo[0], &cpuInfo[1], &cpuInfo[2], &cpuInfo[3])

#endif
	

extern void zen4_set_params(void);
extern void alderlake_set_params(void);

// x64/AMD64 Version

void apacGetCPUSpec(void)
{
	int cpuInfo[4] = { 0 };

	CPUID(cpuInfo, 0x0);

	if (
		cpuInfo[1] == 0x68747541 &&		// 'Auth'
		cpuInfo[3] == 0x69746E65 &&		// 'enti'
		cpuInfo[2] == 0x444D4163		// 'cAMD'
		)
	{
		// Get CPU signature and extract family
		CPUID(cpuInfo, 0x1);
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
		CPUID(cpuInfo, 0x1);
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
			case 0x9A:      // Alder Lake-P (154 decimal = 0x9A hex)
				alderlake_set_params();
				break;

			default:
				break;
			}
		}
		break;

		default:
			break;
		}
	}

	return;
}

#endif