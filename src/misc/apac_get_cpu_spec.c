#include "../header/apac_internal.h"

apn_dig_t (*apn_add_n_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    const apn_dig_t*,
    apn_size_t
) = NULL;

apn_dig_t (*apn_sub_n_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    const apn_dig_t*,
    apn_size_t
) = NULL;

apn_dig_t (*apn_add_one_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t,
    apn_dig_t
) = NULL;

apn_dig_t (*apn_sub_one_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t,
    apn_dig_t
) = NULL;

void (*apn_neg_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t
) = NULL;

void (*apn_mul_one_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t,
    apn_dig_t
) = NULL;

apn_dig_t (*apn_addmul_one_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t,
    apn_dig_t
) = NULL;

apn_dig_t (*apn_submul_one_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t,
    apn_dig_t
) = NULL;

apn_dig_t (*apn_lshift_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t,
    apn_dig_t
) = NULL;

apn_dig_t(*apn_rshift_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t,
    apn_dig_t
) = NULL;

void (*apn_mul_bc_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    const apn_dig_t*,
    apn_size_t,
    apn_size_t
) = NULL;

void (*apn_sqr_bc_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t
) = NULL;

void (*apn_cpy_ptr)(
    apn_dig_t*,
    const apn_dig_t*,
    apn_size_t
) = NULL;

void (*apn_set_ptr)(
    apn_dig_t*,
    apn_size_t,
    apn_dig_t
) = NULL;

int (*apn_cmp_ptr)(
    const apn_dig_t*,
    const apn_dig_t*,
    apn_size_t
) = NULL;

int (*apn_is_zero_ptr)(
    const apn_dig_t*,
    apn_size_t
) = NULL;

apac_cpu_params curr_cpu =
{
    .karatsuba_mul_threshold = (apn_size_t)30,
    .karatsuba_sqr_threshold = (apn_size_t)50,
    .dnc_div_threshold       = (apn_size_t)35
};

#if defined(APAC_WIN_X64)

    #define CPUID(cpuInfo, leaf)	\
        	__cpuid(cpuInfo, leaf)

    #define CPUIDEX(cpuInfo, leaf, subleaf)		\
        	__cpuidex(cpuInfo, leaf, subleaf)

#elif defined(APAC_LINUX_X64) || defined(APAC_MACOS_X64)

    #define CPUID(cpuInfo, leaf) 	\
			__cpuid(leaf,           \
					(cpuInfo)[0],   \
					(cpuInfo)[1],   \
					(cpuInfo)[2],   \
					(cpuInfo)[3]	\
			)

    #define CPUIDEX(cpuInfo, leaf, subleaf) \
			__cpuid_count(leaf, subleaf,    \
						(cpuInfo)[0],       \
						(cpuInfo)[1],       \
						(cpuInfo)[2],      	\
						(cpuInfo)[3]		\
			)

#endif

#if defined(APAC_WIN_X64) || defined(APAC_LINUX_X64) || defined(APAC_MACOS_X64)

#include "../apn/x64/x64_hidden_funcs.h"

extern void zen4_set_params(void);

static uint64_t apac_xgetbv0(void)
{
#if defined(APAC_WIN_X64)
    return (uint64_t)_xgetbv(0);
#else
    uint32_t eax;
    uint32_t edx;

    __asm__ volatile
    (
        "xgetbv"
        : "=a"(eax), "=d"(edx)
        : "c"(0)
    );

    return ((uint64_t)edx << 32) | eax;
#endif
}

void apac_get_cpu_spec(void)
{
    int cpuInfo[4] = { 0 };

    /* ------------------------------------------------------------------------------------------ */
    /*                                      CPUID LEAF 0                                            */
    /* ------------------------------------------------------------------------------------------ */

    CPUID(cpuInfo, 0);

    const uint32_t max_basic_leaf = (uint32_t)cpuInfo[0];
    const uint32_t vendor_ebx = (uint32_t)cpuInfo[1];
    const uint32_t vendor_edx = (uint32_t)cpuInfo[3];
    const uint32_t vendor_ecx = (uint32_t)cpuInfo[2];

    const int is_amd =
        (vendor_ebx == 0x68747541) &&  /* Auth */
        (vendor_edx == 0x69746E65) &&  /* enti */
        (vendor_ecx == 0x444D4163);    /* cAMD */

    const int is_intel =
        (vendor_ebx == 0x756E6547) &&  /* Genu */
        (vendor_edx == 0x49656E69) &&  /* ineI */
        (vendor_ecx == 0x6C65746E);    /* ntel */


    /* ------------------------------------------------------------------------------------------ */
    /*                                      CPUID LEAF 1                                            */
    /* ------------------------------------------------------------------------------------------ */

    uint32_t signature = 0;
    uint32_t leaf1_ecx = 0;

    if (max_basic_leaf >= 1)
    {
        CPUID(cpuInfo, 1);

        signature = (uint32_t)cpuInfo[0];
        leaf1_ecx = (uint32_t)cpuInfo[2];
    }


    /* ------------------------------------------------------------------------------------------ */
    /*                                      CPUID LEAF 7                                            */
    /* ------------------------------------------------------------------------------------------ */

    uint32_t leaf7_ebx = 0;

    if (max_basic_leaf >= 7)
    {
        CPUIDEX(cpuInfo, 7, 0);
        leaf7_ebx = (uint32_t)cpuInfo[1];
    }

    const int has_avx512_hw =
        (leaf7_ebx & (1u << 16)) != 0;


    /* ------------------------------------------------------------------------------------------ */
    /*                                  MICROARCHITECTURE DISPATCH                                  */
    /* ------------------------------------------------------------------------------------------ */

    if (is_amd && max_basic_leaf >= 1)
    {
        const uint32_t base_family = (signature >> 8) & 0xF;
        const uint32_t extended_family = (signature >> 20) & 0xFF;
        const uint32_t family =
            (base_family < 0xF)
                ? base_family
                : base_family + extended_family;

        const uint32_t model =
            ((signature >> 4) & 0xF) |
            (((signature >> 16) & 0xF) << 4);

        switch (family)
        {
            case 0x1A:  /* Zen 5 */
                zen4_set_params();
                break;

            case 0x19:  /* Zen 3 / Zen 4 */
                if (model >= 0x61)
                {
                    /* Zen 4 / Zen 4c */
                    zen4_set_params();
                }
                else if (model >= 0x10 && has_avx512_hw)
                {
                    /* Storm Peak (Threadripper 7000), Genoa (EPYC 9004) */
                    zen4_set_params();
                }
                else
                {
                    /* Add Zen 3 / Zen 3+ tuning here if required. */
                }
                break;

            case 0x17:  /* Zen 1 / Zen 2 */
                /* Add Zen 1 / Zen 2 tuning here if required. */
                break;

            default:
                /* Add other AMD family handling here. */
                break;
        }
    }
    else if (is_intel && max_basic_leaf >= 1)
    {
        const uint32_t base_family = (signature >> 8) & 0xF;
        const uint32_t extended_family = (signature >> 20) & 0xFF;
        const uint32_t family =
            (base_family != 0xF)
                ? base_family
                : base_family + extended_family;

        switch (family)
        {
            case 0x06:
            {
                const uint32_t base_model = (signature >> 4) & 0xF;
                const uint32_t extended_model = (signature >> 16) & 0xF;
                const uint32_t model = (extended_model << 4) | base_model;

                switch (model)
                {
                    /* Add Intel model-specific tuning cases here. */
                    default:
                        break;
                }
                break;
            }

            default:
                /* Add other Intel family handling here. */
                break;
        }
    }


    /* ------------------------------------------------------------------------------------------ */
    /*                                      ISA FEATURE DETECTION                                   */
    /* ------------------------------------------------------------------------------------------ */

    const int has_osxsave =
        (leaf1_ecx & (1u << 27)) != 0;

    const int has_avx_hw =
        (leaf1_ecx & (1u << 28)) != 0;

    uint64_t xcr0 = 0;

    if (has_osxsave && has_avx_hw)
    {
        xcr0 = apac_xgetbv0();
    }

    const int has_avx =
        has_osxsave &&
        has_avx_hw &&
        ((xcr0 & 0x06) == 0x06);

    const int has_avx2 =
        has_avx &&
        ((leaf7_ebx & (1u << 5)) != 0);

    const int has_avx512f =
        has_avx &&
        has_avx512_hw &&
        ((xcr0 & 0xE6) == 0xE6);

    const int has_bmi2 =
        (leaf7_ebx & (1u << 8)) != 0;

    const int has_adx =
        (leaf7_ebx & (1u << 19)) != 0;


    /* ------------------------------------------------------------------------------------------ */
    /*                                  BASELINE FUNCTION POINTERS                                 */
    /* ------------------------------------------------------------------------------------------ */

    /* Arithmetic: baseline x64. */
    apn_add_n_ptr      = add_n_x64;
    apn_sub_n_ptr      = sub_n_x64;
    apn_add_one_ptr    = add_one_x64;
    apn_sub_one_ptr    = sub_one_x64;
    apn_neg_ptr        = neg_x64;
    apn_mul_one_ptr    = mul_one_x64;
    apn_addmul_one_ptr = addmul_one_x64;
    apn_submul_one_ptr = submul_one_x64;
    apn_lshift_ptr     = lshift_lt64_x64;
    apn_rshift_ptr     = rshift_lt64_x64;
    apn_mul_bc_ptr     = mul_bc_x64;
    apn_sqr_bc_ptr     = sqr_bc_x64;

    /* SSE2 is part of the x86-64 baseline. */
    apn_cpy_ptr     = cpy_sse2;
    apn_set_ptr     = set_sse2;
    apn_cmp_ptr     = cmp_sse2;
    apn_is_zero_ptr = is_zero_sse2;


    /* ------------------------------------------------------------------------------------------ */
    /*                                  ADX + BMI2 DISPATCH                                         */
    /* ------------------------------------------------------------------------------------------ */

    if (has_adx && has_bmi2)
    {
        apn_addmul_one_ptr = addmul_one_adx_bmi2_x64;
        apn_submul_one_ptr = submul_one_adx_bmi2_x64;
        apn_mul_bc_ptr     = mul_bc_adx_bmi2_x64;
        apn_sqr_bc_ptr     = sqr_bc_adx_bmi2_x64;
    }

    if (has_bmi2)
    {
        apn_mul_one_ptr = mul_one_bmi2_x64;
        apn_lshift_ptr  = lshift_lt64_bmi2_x64;
        apn_rshift_ptr  = rshift_lt64_bmi2_x64;
    }

    if (has_avx)
    {
        apn_cpy_ptr = cpy_avx;
        apn_set_ptr = set_avx;
    }

    if (has_avx2)
    {
        apn_cmp_ptr     = cmp_avx2;
        apn_is_zero_ptr = is_zero_avx2;
    }

    if (has_avx512f)
    {
        apn_cpy_ptr     = cpy_avx512f;
        apn_set_ptr     = set_avx512f;
        apn_is_zero_ptr = is_zero_avx512f;
    }
}

#endif
