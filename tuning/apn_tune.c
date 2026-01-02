#include "../include/apac.h"

/* ============================================================
 * rdcpu_cnt - Read CPU timestamp/counter
 *
 * Returns high-resolution CPU counter in nanoseconds.
 * Uses platform-specific instructions:
 * - x86-64: TSR (Time Stamp Counter)
 * - ARM64:  Virtual counter registers
 * ============================================================ */
static inline uint64_t cpu_timer(void)
{
    /* --------------------------------------------------------
     * x86-64 — Windows (MSVC / Clang-cl)
     * -------------------------------------------------------- */
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64))

    _mm_lfence();
    uint64_t t = __rdtsc();
    _mm_lfence();
    return t;

    /* --------------------------------------------------------
     * x86-64 — Unix / Linux / macOS (GCC / Clang)
     * -------------------------------------------------------- */
#elif (defined(__GNUC__) || defined(__clang__)) && \
      (defined(__x86_64__) || defined(__amd64__))

    _mm_lfence();
    uint64_t t = __rdtsc();
    _mm_lfence();
    return t;

    /* --------------------------------------------------------
     * ARM64 — Windows / MSVC
     * -------------------------------------------------------- */
#elif defined(_M_ARM64) && defined(_MSC_VER)

    uint64_t cnt, frq;
    __isb(0xF);
    cnt = _ReadStatusReg(ARM64_CNTVCT_EL0);
    frq = _ReadStatusReg(ARM64_CNTFRQ_EL0);
    return (cnt * 1000000000ULL) / frq;

    /* --------------------------------------------------------
     * ARM64 — Unix / Linux / macOS (GCC / Clang)
     * -------------------------------------------------------- */
#elif (defined(__GNUC__) || defined(__clang__)) && \
      (defined(__aarch64__) || defined(__arm64__))

    __isb();
    uint64_t cnt = __arm_rsr64("cntvct_el0");
    uint64_t frq = __arm_rsr64("cntfrq_el0");
    return (cnt * 1000000000ULL) / frq;

#else
    #error "rdcpu_cnt(): unsupported architecture or compiler"
#endif
}

/* --------------------------------------------------------
 * Platform-specific headers
 * -------------------------------------------------------- */
#if defined(_WIN32)

    #include <powrprof.h>
    #pragma comment(lib, "powrprof")

elif defined(__linux__) || defined(__linux)

    #include <sched.h>

#endif

 /* ============================================================
  * pinThreadToCore - Pin current thread to specific CPU core
  *
  * Parameters:
  *   core_id - Logical CPU core ID (0-based)
  *
  * Returns:
  *   0  - Success
  *   -1 - Unsupported platform or failure
  *
  * Platform support:
  *   - Windows: SetThreadAffinityMask
  *   - Linux:   pthread_setaffinity_np
  *   - macOS:   Unsupported (returns -1)
  * ============================================================ */
static int pin_curr_thread_to_core(uint32_t core_id)
{
    /* --------------------------------------------------------
     * Windows implementation
     * -------------------------------------------------------- */
#if defined(_WIN32)

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    uint32_t num_cores = (uint32_t)sysinfo.dwNumberOfProcessors;
    core_id = core_id % num_cores;

    DWORD_PTR mask = (DWORD_PTR)1 << core_id;
    HANDLE thread = GetCurrentThread();
    return (SetThreadAffinityMask(thread, mask) != 0) ? 0 : -1;

    /* --------------------------------------------------------
     * Linux implementation
     * -------------------------------------------------------- */
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

    /* --------------------------------------------------------
     * macOS and other platforms - unsupported
     * -------------------------------------------------------- */
#else

    (void)core_id;
    return -1;

#endif
}

#if defined(_WIN32)

    static GUID* CurrentScheme;
    static DWORD CurrentMode;

#endif

/* ============================================================
 * disable_turbo_boost - Disable CPU dynamic frequency scaling
 *
 * Disables dynamic frequency scaling for consistent benchmark
 * results. Only supported on Windows x86-64.
 *
 * WARNING: On non-Windows platforms, turbo boost cannot be
 * disabled through this API. For optimal, consistent, and
 * accurate benchmark results, manually disable turbo boost
 * through your operating system's power settings.
 * ============================================================ */
static void disable_turbo_boost(void)
{
    /* --------------------------------------------------------
     * Windows x86-64 implementation
     * -------------------------------------------------------- */
#if defined(_WIN32) && (defined(_M_X64) || defined(_M_AMD64))

    PowerGetActiveScheme(NULL, &CurrentScheme);

    PowerReadACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, &CurrentMode);

    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, PROCESSOR_PERF_BOOST_MODE_DISABLED);
    
    PowerSetActiveScheme(NULL, CurrentScheme);

    /* --------------------------------------------------------
     * Other platforms - unsupported
     * -------------------------------------------------------- */
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
        fprintf(stderr, "Aborting...\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "\n");

#endif
}

/* ============================================================
 * restore_turbo_boost - Restore CPU turbo boost settings
 *
 * Restores the original turbo boost configuration that was
 * active before disableTurboBoost() was called.
 * Only functional on Windows x86-64.
 * ============================================================ */
static void restore_turbo_boost(void)
{
    /* --------------------------------------------------------
     * Windows x86-64 implementation
     * -------------------------------------------------------- */
#if defined(_WIN32) && (defined(_M_X64) || defined(_M_AMD64))

    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, CurrentMode);
    
    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, PROCESSOR_PERF_BOOST_MODE_ENABLED);
    
    PowerSetActiveScheme(NULL, CurrentScheme);
    
    LocalFree(CurrentScheme);
    
    CurrentScheme = NULL;

    /* --------------------------------------------------------
     * Other platforms - no action needed
     * -------------------------------------------------------- */
#else
     /* Nothing to restore on non-Windows platforms */
#endif
}

/* PRNG - SFC64 */

static uint64_t prng_state[4] = { 0 };

static inline uint64_t random_sfc64(void)
{
    uint64_t out = prng_state[1] + prng_state[2] + prng_state[0];
    prng_state[0]++;
    prng_state[1] = prng_state[2] ^ (prng_state[2] >> 11);
    prng_state[2] = prng_state[3] + (prng_state[3] << 3);
    prng_state[3] = ROTL64(prng_state[3], 24) + out;
    return out;
}

static inline void random_sfc64_seed(uint64_t seed)
{
    prng_state[0] = 1;
    prng_state[1] = seed;
    prng_state[2] = seed;
    prng_state[3] = seed;
    for (int i = 0; i < 16; i++)
        random_sfc64();
}

static void set_to_random(apn_seg_t* op1, apn_size_t size)
{
    for (apn_size_t i = 0; i < size; i++)
    {
        op1[i] = random_sfc64();
    }
}

#define MALLOC_CHECK(op, size)                                                          \
        do                                                                              \
        {                                                                               \
            (op) = apac_malloc(sizeof(apn_seg_t) * (size));                             \
            if (!(op))                                                                  \
            {                                                                           \
                restore_turbo_boost();                                                  \
                APAC_LOG_ERR("Memory allocation failure in tuning func! Aborting ..."); \
                abort();                                                                \
            }                                                                           \
        } while (0)

#define RUN_COUNT       ((uint64_t)1 << 13)

static void get_karatsuba_mul_balanced_thresh(void)
{
    const apn_size_t thresh_start = 10;
    const apn_size_t thresh_end = 100;
    const apn_size_t size_start = 1;
    const apn_size_t size_end = 384;

    /* Tuning test name */
    printf("=== Karatsuba Balanced Multiplication Threshold Tuning ===\n");

    /* Informational output */
    printf("Test size range     : %u to %u\n",
        (unsigned)size_start,
        (unsigned)size_end);

    printf("Threshold range     : %u to %u\n\n",
        (unsigned)thresh_start,
        (unsigned)thresh_end);

    apn_seg_t* op1 = NULL;
    apn_seg_t* op2 = NULL;
    apn_seg_t* op3 = NULL;

    MALLOC_CHECK(op1, size_end);
    MALLOC_CHECK(op2, size_end);
    MALLOC_CHECK(op3, 2 * size_end);

    FILE* fp = fopen("karatsuba_mul_balanced_threshold.csv", "w");
    if (!fp)
    {
        restore_turbo_boost();
        APAC_LOG_ERR("Failed to open karatsuba_mul_balanced_threshold.csv");
        abort();
    }

    fprintf(fp, "threshold,size,avg_cycles\n");

    for (apn_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing threshold %u ... ", (unsigned)thresh);
        fflush(stdout);

        curr_cpu.karatsuba_mul_balanced_threshold = thresh;

        for (apn_size_t size = size_start; size <= size_end; size++)
        {
            set_to_random(op1, size);
            set_to_random(op2, size);
            apn_set(op3, 2 * size, 0);

            /* Warmup */
            for (int w = 0; w < 16; w++)
                apn_mul_n(op3, op1, op2, size);

            uint64_t start, end;
            RD_CPU_CNT(start);

            for (uint64_t k = 0; k < RUN_COUNT; k++)
                apn_mul_n(op3, op1, op2, size);

            RD_CPU_CNT(end);

            double avg_cycles =
                (double)(end - start) / (double)RUN_COUNT;

            fprintf(fp, "%u,%u,%.3f\n",
                (unsigned)thresh,
                (unsigned)size,
                avg_cycles);
        }

        printf("Done\n");
    }

    fclose(fp);

    free(op1);
    free(op2);
    free(op3);
}

int main(void)
{
    apacInit();

    /* Ask user which core to pin to */
    uint32_t core_id = 0;
    printf("Enter logical CPU core to pin the benchmark thread to: ");
    fflush(stdout);

    if (scanf("%u", &core_id) != 1)
    {
        fprintf(stderr, "Invalid core id input. Aborting...\n");
        return EXIT_FAILURE;
    }

    if (pin_curr_thread_to_core(core_id) != 0)
    {
        fprintf(stderr,
            "Warning: failed to pin thread to core %u (continuing anyway)\n",
            core_id);
    }

    /* Deterministic PRNG seed for reproducibility */
    random_sfc64_seed(0x123456789ABCDEF0ULL);

    /* Disable turbo boost (or prompt user on unsupported platforms) */
    disable_turbo_boost();

    /* Run Karatsuba balanced-threshold sweep */
    get_karatsuba_mul_balanced_thresh();

    /* Restore original turbo boost state */
    restore_turbo_boost();

    return EXIT_SUCCESS;
}