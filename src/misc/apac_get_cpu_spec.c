#include "../headers/apac_internal.h"

// x64/AMD64 Version

#if (defined(APAC_X64_WIN) || defined(APAC_X64_UNIX))

extern void zen4_set_params(void);
extern void generic_x64_set_params(void);

#if defined(APAC_X64_WIN)

	#define CPUID(cpuInfo, Leaf)			__cpuid(cpuInfo, Leaf)
	#define CPUIDEX(cpuInfo, Leaf, SubLeaf) __cpuidex(cpuInfo, Leaf, SubLeaf)

#elif defined(APAC_X64_UNIX)

    #define CPUID(cpuInfo, leaf)    \
        __cpuid((leaf),             \
                (cpuInfo)[0],       \
                (cpuInfo)[1],       \
                (cpuInfo)[2],       \
                (cpuInfo)[3]		\
			)

    #define CPUIDEX(cpuInfo, leaf, subleaf) \
        __cpuid_count((leaf), (subleaf),    \
                    (cpuInfo)[0],           \
                    (cpuInfo)[1],           \
                    (cpuInfo)[2],           \
                    (cpuInfo)[3]			\
				)

#else
    #error "Unsupported Compiler!"
#endif

void apac_get_cpu_spec(void)
{
	int cpuInfo[4] = { 0 };

	CPUID(cpuInfo, 0x0);

	// ================= AMD x86-64 =================
	if (
		cpuInfo[1] == 0x68747541 &&		// 'Auth'
		cpuInfo[3] == 0x69746E65 &&		// 'enti'
		cpuInfo[2] == 0x444D4163		// 'cAMD'
	)
	{
		CPUID(cpuInfo, 0x1);
		int signature = cpuInfo[0];

		int baseFamily = (signature >> 8) & 0xF;
		int extendedFamily = (signature >> 20) & 0xFF;
		int family = (baseFamily < 0xF)
			? baseFamily
			: baseFamily + extendedFamily;

		int model = ((signature >> 4) & 0xF) |
			(((signature >> 16) & 0xF) << 4);

		CPUID(cpuInfo, 0x7);
		uint8_t has_avx512 = (cpuInfo[1] & (1 << 16)) != 0;

		switch (family)
		{
		case 0x1A:   // Zen 5
			zen4_set_params();
			break;

		case 0x19:   // Zen 3 / Zen 4
			if (model >= 0x61)
			{
				// Unambiguous Zen 4 / Zen 4c
				zen4_set_params();
			}
			else if (model >= 0x10 && has_avx512)
			{
				// Storm Peak (TR 7000), Genoa (EPYC 9004)
				zen4_set_params();
			}
			else
			{
				// Zen 3 / Zen 3+
				generic_x64_set_params();
			}
			break;

		case 0x17:   // Zen 1 / Zen 2
			generic_x64_set_params();
			break;

		default:
			generic_x64_set_params();
			break;
		}
	}

	// ================= Intel x86-64 =================
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
		int family = (baseFamily != 0xF)
			? baseFamily
			: baseFamily + extendedFamily;

		switch (family)
		{
		case 0x06:
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

	// ================= Unknown x86-64 =================
	else
	{
		generic_x64_set_params();
	}
}

#endif
