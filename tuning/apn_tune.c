#include "../include/apac.h"

/* ============================================================
   RD_CPU_CNT(out)

   Reads a high-resolution, fixed-frequency time counter.

   - x86-64: invariant TSC (time at reference frequency)
   - ARM64 : CNTVCT_EL0 (time at CNTFRQ frequency)

   NOTE:
   - Units are NOT architectural CPU cycles.
   - Values represent elapsed time in counter ticks.
   - Safe for relative comparisons and threshold tuning.
   ============================================================ */

/* ============================================================
   1) x86-64 — Windows (MSVC / Clang-cl)
   ============================================================ */
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64))

    #define RD_CPU_CNT(out)     \
        do                      \
        {                       \
            _mm_lfence();       \
            (out) = __rdtsc();  \
            _mm_lfence();       \
        } while (0)

/* ============================================================
   2) x86-64 — Unix / Linux / macOS (GCC / Clang)
   ============================================================ */
#elif defined(__x86_64__) && ((defined(__GNUC__) || defined(__clang__))

    #define RD_CPU_CNT(out)                         \
        do                                          \
        {                                           \
            uint32_t __lo, __hi;                    \
            asm volatile (                          \
                "lfence\n\t"                        \
                "rdtsc\n\t"                         \
                "lfence\n\t"                        \
                : "=a"(__lo), "=d"(__hi)            \
                :                                   \
                : "memory"                          \
            );                                      \
            (out) = ((uint64_t)__hi << 32) | __lo;  \
        } while (0)

/* ============================================================
   3) ARM64 — Windows / MSVC
   ============================================================ */
#elif defined(_M_ARM64) && defined(_MSC_VER)

    #define RD_CPU_CNT(out)                                     \
            do                                                  \
            {                                                   \
                uint64_t __cnt, __frq;                          \
                __isb(0xF);                                     \
                __cnt = _ReadStatusReg(ARM64_CNTVCT_EL0);       \
                __frq = _ReadStatusReg(ARM64_CNTFRQ_EL0);       \
                (out) = (__cnt * 1000000000ULL) / __frq;        \
            } while (0)


/* ============================================================
   4) ARM64 — Unix / Linux / macOS (GCC / Clang)
   ============================================================ */
#elif defined(__aarch64__) && (defined(__GNUC__) || defined(__clang__))

#elif defined(__aarch64__) && (defined(__GNUC__) || defined(__clang__))

    #define RD_CPU_CNT(out)                                 \
            do                                              \
            {                                               \
                uint64_t __cnt, __frq;                      \
                asm volatile (                              \
                    "isb\n\t"                               \
                    "mrs %0, cntvct_el0\n\t"                \
                    "mrs %1, cntfrq_el0"                    \
                    : "=r"(__cnt), "=r"(__frq)              \
                    :                                       \
                    : "memory"                              \
                );                                          \
                (out) = (__cnt * 1000000000ULL) / __frq;    \
            } while (0)


#else
    #error "RD_CPU_CNT: unsupported architecture or compiler"
#endif

#if defined(__linux__) || defined(__linux)
    #include <sched.h>
#endif

/* ============================================================
    PIN_THREAD_TO_CORE(core_id)

    Pins the *current thread* to a single logical CPU.

    - Windows: SetThreadAffinityMask
    - Linux  : pthread_setaffinity_np
    - macOS  : unsupported (returns -1)

    Returns:
    0  -> success
    -1  -> unsupported or failure
    ============================================================ */

static inline int pin_thread_to_core(uint32_t core_id)
{
#if defined(_WIN32)

    DWORD_PTR mask = (DWORD_PTR)1 << core_id;
    HANDLE thread = GetCurrentThread();

    return (SetThreadAffinityMask(thread, mask) != 0) ? 0 : -1;

#elif defined(__linux__) || defined(__linux)

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t thread = pthread_self();

    return (pthread_setaffinity_np(
        thread,
        sizeof(cpu_set_t),
        &cpuset) == 0) ? 0 : -1;

#else

    /* macOS / others: intentionally unsupported */
    (void)core_id;
    return -1;

#endif
}

#define BALANCED_KARATSUBA_MUL_START    (apn_size_t)10
#define BALANCED_KARATSUBA_MUL_END      (apn_size_t)100

#define UNBALANCED_KARATSUBA_MUL_START  (apn_size_t)10
#define UNBALANCED_KARATSUBA_MUL_END    (apn_size_t)100

#define KARATSUBA_SQR_START             (apn_size_t)10
#define KARATSUBA_SQR_END               (apn_size_t)100

#define DNC_DIV_START                   (apn_size_t)10
#define DNC_DIV_END                     (apn_size_t)100

#define RUNTIME                         2ULL * 1000 * 1000 * 1000

static void getOptimalBalancedKaratsubaMulThreshold(void)
{
    
}

int main(void)
{
    return EXIT_SUCCESS;
}