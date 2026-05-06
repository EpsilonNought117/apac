#include "apac_utilities.h"

uint64_t cpu_timer(void)
{
#if (defined(APAC_X64_WIN) || defined(APAC_X64_UNIX))

    _mm_lfence();
    uint64_t t = __rdtsc();
    _mm_lfence();
    return t;

#elif defined(APAC_ARM64_WIN)

    uint64_t cnt, frq;
    __isb(0xF);
    cnt = _ReadStatusReg(ARM64_CNTVCT_EL0);
    frq = _ReadStatusReg(ARM64_CNTFRQ_EL0);
    return (cnt * 1000000000ULL) / frq;

#elif defined(APAC_ARM64_UNIX)

    __isb();
    uint64_t cnt = __arm_rsr64("cntvct_el0");
    uint64_t frq = __arm_rsr64("cntfrq_el0");
    return (cnt * 1000000000ULL) / frq;

#else

    #error "cpu_timer(): unsupported CPU Architecture, OS or Compiler!"

#endif
}

uint64_t os_timer(void)
{
#if defined(_WIN32)

    APAC_THREAD_LOCAL static uint64_t freq = 0;
    LARGE_INTEGER t;

    if (!freq)
    {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        freq = (uint64_t)f.QuadPart;
    }

    QueryPerformanceCounter(&t);

    return (uint64_t)((t.QuadPart * 1000000000ULL) / freq);

#elif (defined(__linux__)  || defined(__linux) || \
       defined(__unix__)   || defined(__unix))

    struct timespec ts;

    /* Prefer RAW if available (no NTP adjustments) */
    #ifdef CLOCK_MONOTONIC_RAW
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts) != 0)
            return 0;
    #else
        if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
            return 0;
    #endif

    return ((uint64_t)ts.tv_sec * 1000000000ULL) +
           (uint64_t)ts.tv_nsec;

#elif defined(__APPLE__) && defined(__MACH__)

    static uint64_t timebase = 0;
    uint64_t t = mach_absolute_time();

    if (!timebase)
    {
        mach_timebase_info_data_t tb;
        mach_timebase_info(&tb);
        timebase = ((uint64_t)tb.numer << 32) | tb.denom;
    }

    return (t * (timebase >> 32)) /
        (timebase & 0xffffffffu);

#else

    #error "os_timer(): Unsupported Operating System!"

#endif
}

int pin_curr_thread_to_core(uint32_t core_id)
{
#if defined(_WIN32)

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    uint32_t num_cores = (uint32_t)sysinfo.dwNumberOfProcessors;
    core_id = core_id % num_cores;

    DWORD_PTR mask = (DWORD_PTR)1 << core_id;
    HANDLE thread = GetCurrentThread();
    return (SetThreadAffinityMask(thread, mask) != 0) ? 0 : -1;

#elif defined(__linux__) || defined(__linux)

    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cores > 0)
    {
        core_id = core_id % (uint32_t)num_cores;
    }

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    pthread_t thread = pthread_self();
    return (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) == 0) ? 0 : -1;

#else

    return -1;

#endif
}


#if defined(_WIN32)

    static GUID* CurrentScheme;
    static DWORD CurrentMode;

#endif

void disable_turbo_boost(void)
{
#if defined(_WIN32)

    PowerGetActiveScheme(NULL, &CurrentScheme);

    PowerReadACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, &CurrentMode);

    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, PROCESSOR_PERF_BOOST_MODE_DISABLED);

    PowerSetActiveScheme(NULL, CurrentScheme);

#else

    fprintf(stderr, "\n");
    fprintf(stderr, "WARNING: Turbo boost cannot be disabled on this platform.\n");
    fprintf(stderr, "         Disable it manually for optimal, consistent, and accurate results.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Continue anyway? (y/n): ");
    fflush(stderr);

    char buffer[16];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        fprintf(stderr, "\nError reading input. Aborting...\n");
        exit(EXIT_FAILURE);
    }

    if (buffer[0] != 'y' && buffer[0] != 'Y')
    {
        fprintf(stderr, "Exiting ...\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "\n");

#endif
}

void restore_turbo_boost(void)
{
#if defined(_WIN32) && (defined(_M_X64) || defined(_M_AMD64))

    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, CurrentMode);

    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, PROCESSOR_PERF_BOOST_MODE_ENABLED);

    PowerSetActiveScheme(NULL, CurrentScheme);

    LocalFree(CurrentScheme);

    CurrentScheme = NULL;

#else
     /* Nothing to restore on non-Windows platforms */
#endif
}