#include "../header/apac_internal.h"

#if defined(_WIN32) && defined(_MSC_VER)
    #include <powrprof.h>
    #pragma comment(lib, "powrprof")
#endif

uint64_t apac_cpu_timer(void)
{
#if defined(APAC_WIN_X64)   || \
    defined(APAC_LINUX_X64) || \
    defined(APAC_MACOS_X64)

    _mm_lfence();
    uint64_t t = __rdtsc();
    _mm_lfence();
    return t;

#elif defined(APAC_WIN_ARM64)

    uint64_t cnt, frq;
    __isb(0xF);
    cnt = _ReadStatusReg(ARM64_CNTVCT_EL0);
    frq = _ReadStatusReg(ARM64_CNTFRQ_EL0);
    return (cnt * 1000000000ULL) / frq;

#elif defined(APAC_LINUX_ARM64) || defined(APAC_MACOS_ARM64)

    __isb();
    uint64_t cnt = __arm_rsr64("cntvct_el0");
    uint64_t frq = __arm_rsr64("cntfrq_el0");
    return (cnt * 1000000000ULL) / frq;

#else
    return 0;
#endif
}

/*
 * Pins the calling thread to a logical processor.
 *
 * core_id is a zero-based global logical processor index.
 *
 * Windows:
 *   Maps core_id to a processor group and group-relative CPU index.
 *
 * Linux:
 *   Uses dynamically allocated CPU sets, supporting CPU IDs
 *   beyond CPU_SETSIZE.
 *
 * Returns:
 *    0  Success
 *   -1  Failure or unsupported platform
 */
int apac_pin_thread_to_core(uint32_t core_id)
{
#if defined(APAC_WIN_X64) || defined(APAC_WIN_ARM64)

    WORD num_groups = GetActiveProcessorGroupCount();

    uint32_t remaining = core_id;

    for (WORD group = 0; group < num_groups; ++group)
    {
        DWORD num_cores = GetActiveProcessorCount(group);

        if (remaining < num_cores)
        {
            if (remaining >= (sizeof(KAFFINITY) * CHAR_BIT))
            {
                return -1;
            }

            GROUP_AFFINITY affinity = { 0 };

            affinity.Group = group;
            affinity.Mask = ((KAFFINITY)1) << remaining;

            return SetThreadGroupAffinity(
                GetCurrentThread(),
                &affinity,
                NULL
            ) ? 0 : -1;
        }

        remaining -= num_cores;
    }

    return -1;
    
#elif defined(APAC_LINUX_X64) || defined(APAC_LINUX_ARM64)

    if (core_id > INT_MAX)
        return -1;

    size_t ncpus = (size_t)core_id + 1;

    if (ncpus < CPU_SETSIZE)
        ncpus = CPU_SETSIZE;

    cpu_set_t* cpuset;
    size_t setsize;

    /*
     * Allocate a sufficiently large CPU set.
     */
    for (;;)
    {
        setsize = CPU_ALLOC_SIZE(ncpus);
        cpuset = CPU_ALLOC(ncpus);

        if (cpuset == NULL)
            return -1;

        CPU_ZERO_S(setsize, cpuset);

        if (sched_getaffinity(0, setsize, cpuset) == 0)
            break;

        int error = errno;

        CPU_FREE(cpuset);

        if (error != EINVAL || ncpus > SIZE_MAX / 2)
            return -1;

        ncpus *= 2;
    }

    /*
     * Check whether the requested CPU is available.
     */
    if (!CPU_ISSET_S(core_id, setsize, cpuset))
    {
        CPU_FREE(cpuset);
        return -1;
    }

    /*
     * Restrict the CPU set to the requested CPU.
     */
    CPU_ZERO_S(setsize, cpuset);
    CPU_SET_S(core_id, setsize, cpuset);

    /*
     * Apply affinity to the calling thread.
     */
    int result = pthread_setaffinity_np(
        pthread_self(),
        setsize,
        cpuset
    );

    CPU_FREE(cpuset);

    return result == 0 ? 0 : -1;

#else

    (void)core_id;

    APAC_WARN("Core affinity cannot be set on this platform!\n");

    return -1;

#endif
}

#if defined(APAC_WIN_X64) || defined(APAC_WIN_ARM64)
    static GUID* CurrentScheme;
    static DWORD CurrentMode;
#endif

/**
 * dfs = dynamic frequency scaling
 */
void apac_disable_dfs(void)
{
#if defined(APAC_WIN_X64) || defined(APAC_WIN_ARM64)

    PowerGetActiveScheme(NULL, &CurrentScheme);
    PowerReadACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, &CurrentMode);
    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, PROCESSOR_PERF_BOOST_MODE_DISABLED);
    PowerSetActiveScheme(NULL, CurrentScheme);

#else

    APAC_WARN(
        "\n"
        "WARNING: Dynamic Frequency Scaling (Turbo Boost) cannot be disabled on this platform.\n"
        "         Disable it manually for optimal, consistent, and accurate results.          \n"
        "\n"
        "Continue anyway? (y/n): "
    );

    char buffer[16];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        fprintf(stderr, "\nError reading input. Aborting...\n");
        exit(EXIT_FAILURE);
    }

    if (buffer[0] != 'y' && buffer[0] != 'Y')
    {
        fprintf(stderr, "Exiting ...\n");
        exit(EXIT_SUCCESS);
    }

    fprintf(stderr, "\n");

#endif
}

/**
 * dfs = dynamic frequency scaling
 */
void apac_restore_dfs(void)
{
#if defined(APAC_WIN_X64) || defined(APAC_WIN_ARM64)

    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, CurrentMode);
    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, PROCESSOR_PERF_BOOST_MODE_ENABLED);
    PowerSetActiveScheme(NULL, CurrentScheme);
    LocalFree(CurrentScheme);
    CurrentScheme = NULL;

#endif
}