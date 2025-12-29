#include "apac.h"

/* ============================================================
              1) x86-64 — Windows (MSVC / Clang-cl)
   ============================================================ */
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64))

    #define RD_CPU_CNT(out)     \
        do {                    \
            _mm_lfence();       \
            (out) = __rdtsc();  \
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
                "rdtsc"                             \
                : "=a"(__lo), "=d"(__hi)            \
                :                                   \
                : "memory"                          \
            );                                      \
            (out) = ((uint64_t)__hi << 32) | __lo;  \
        } while (0)

/* ============================================================
            3) ARM64 — Windows / MSVC (NO inline asm)
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
                "isb\n"                 \
                "mrs %0, cntvct_el0"    \
                : "=r"(__val)           \
                :                       \
                : "memory"              \
            );                          \
            (out) = __val;              \
        } while (0)

#else
    #error "Unsupported architecture or compiler"
#endif

static void getKaratsubaBalancedThreshold(void)
{

}

static void getKaratsubaUnbalancedThreshold(void)
{

}