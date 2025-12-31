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
        do {                    \
            _mm_lfence();       \
            (out) = __rdtsc();  \
            _mm_lfence();       \
        } while (0)

/* ============================================================
   2) x86-64 — Unix / Linux / macOS (GCC / Clang)
   ============================================================ */
#elif defined(__x86_64__) && (defined(__GNUC__) || defined(__clang__))

    #define RD_CPU_CNT(out)                         \
        do {                                        \
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

    #define RD_CPU_CNT(out)                                           \
        do {                                                          \
            __isb(0xF);                                               \
            (out) = _ReadStatusReg(ARM64_CNTVCT_EL0);                 \
        } while (0)

/* ============================================================
   4) ARM64 — Unix / Linux / macOS (GCC / Clang)
   ============================================================ */
#elif defined(__aarch64__) && (defined(__GNUC__) || defined(__clang__))

    #define RD_CPU_CNT(out)             \
        do {                            \
            uint64_t __val;             \
            asm volatile (              \
                "isb\n\t"               \
                "mrs %0, cntvct_el0"    \
                : "=r"(__val)           \
                :                       \
                : "memory"              \
            );                          \
            (out) = __val;              \
        } while (0)

#else
    #error "RD_CPU_CNT: unsupported architecture or compiler"
#endif

static void getOptimalKaratsubaBalancedThreshold(void)
{
    
}
