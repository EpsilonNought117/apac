#ifndef APAC_H
#define APAC_H

/****************************************************************************************************/
/********************************      REQUIRED STANDARD HEADERS      *******************************/
/****************************************************************************************************/

#if defined(__GNUC__)
    #define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <memory.h>
#include <inttypes.h>

/****************************************************************************************************/
/******************   COMPILER SPECIFIC HEADERS AND DLL/STATIC IMPORT/EXPORTS    ********************/
/****************************************************************************************************/

#if defined(_WIN32)

    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>

    #if defined(_MSC_VER)

        #if defined(_M_X64) || defined(_M_AMD64)

            #include <immintrin.h>
            #include <intrin.h>

        #elif defined(_M_ARM64) || defined(_M_ARM64EC)

			// TODO

		#else
			#error "Unsupported Architecture on Windows and MSVC!"	
		#endif

    #else
        #error "Unknown Compiler on Windows!"
    #endif

    #if defined(BUILD_SHARED_LIB)
        // Export symbols when building the DLL
        #define APAC_API __declspec(dllexport)
    #elif defined(LIBAPAC_SHARED)
        // Import symbols when using the DLL
        #define APAC_API __declspec(dllimport)
    #else
        // Static library, no import/export needed
        #define APAC_API
    #endif

#elif defined(__linux__) || defined(__linux)    ||  \
      defined(__unix__)  || defined(__unix)     ||  \
      defined(__APPLE__) || defined(__MACH__)

    #if defined(__GNUC__) || defined(__clang__)

        #include <unistd.h>
        #include <time.h>
        #include <sched.h>
        #include <pthread.h>

        #if defined(__x86_64)   || defined(__amd64)   || \
            defined(__x86_64__) || defined(__amd64__)

            #include <x86intrin.h>
            #include <cpuid.h>
            #include <immintrin.h>
        
        #elif defined(__aarch64__) || defined(__arm64__)

            #include <arm_acle.h>                

		#else
			#error "Unsupported Architecture on Linux/Unix/MacOS and GCC/Clang!"
        #endif

    #else
        #error "Unknown Compiler on Linux/Unix/MacOS!"
    #endif

    #if defined(BUILD_SHARED_LIB)
        // Export symbols when building the shared object/dylib
        #define APAC_API __attribute__((visibility("default")))
    #else
        // Static library, no visibility attributes needed
        #define APAC_API
    #endif

#else

    #error "Unknown Platform!"

#endif

typedef uint64_t            apn_seg_t;
typedef size_t              apn_size_t;

#define PRI_APN_PTR         "p"
#define PRI_APN_SIZE        "zu"
#define APN_SIZE_MAX        SIZE_MAX
#define APN_SEG_MAX         UINT64_MAX
#define PRI_APN_SEGU        PRIu64
#define PRI_APN_SEGX        PRIx64
#define APN_SEG_BITS        64U
#define APN_SEG_HIGH_BIT    (1ULL << 63)

/****************************************************************************************************/
/*********************************      ERROR HANDLING MACROS      **********************************/
/****************************************************************************************************/

/**
 * @defgroup apac_error Error Handling and Debug Assertions
 * @brief Error codes, logging, and debug-time validation macros.
 *
 * This module defines:
 *
 * - Error codes returned by libapac functions
 * - Assertion macros used for validating API preconditions in debug builds
 * - Memory overlap checking helpers
 *
 * Assertion macros are intended for internal use and for catching programming
 * errors during development. They are disabled in release builds when
 * `APAC_DISABLE_ASSERT` is defined.
 *
 * @{
 */

/* ============================================================================
 * Error codes
 * ========================================================================== */

/**
 * @ingroup apac_error
 * @brief Error codes returned by libapac functions.
 */
typedef enum
{
    /** Operation completed successfully. */
    APAC_OK,

    /** Operation failed due to out-of-memory condition. */
    APAC_OOM,

    /** Division by zero was detected. */
    APAC_DIV_BY_ZERO

} apac_err;


/* ============================================================================
 * Always-enabled assertion
 * ========================================================================== */

/**
 * @ingroup apac_error
 * @def APAC_ALWAYS_ASSERT(expr)
 * @brief Assertion macro that is always enabled.
 *
 * Evaluates the expression and terminates the program if it evaluates to false.
 * Prints diagnostic information including the failed expression, source file,
 * and line number.
 *
 * This macro is used internally as the base implementation for other assertion
 * helpers.
 *
 * @param expr Boolean expression to validate.
 */
#define APAC_ALWAYS_ASSERT(expr)                            \
    do                                                      \
    {                                                       \
        if (!(expr))                                        \
        {                                                   \
            fprintf(                                        \
                stderr,                                     \
                "\n\nAPAC ASSERTION FAILED!\n"              \
                "ASSERTION: %s\n"                           \
                "FILE: %s\nLINE: %d\n",                     \
                #expr, __FILE__, __LINE__                   \
            );                                              \
            fprintf(stderr, "EXITING ...\n\n");             \
            exit(EXIT_FAILURE);                             \
        }                                                   \
    } while (0)


/* ============================================================================
 * Debug-only assertions
 * ========================================================================== */

#ifndef APAC_DISABLE_ASSERT

    /**
     * @ingroup apac_error
     * @def APAC_ASSERT(expr)
     * @brief Debug assertion for validating API preconditions.
     *
     * Behaves like ::APAC_ALWAYS_ASSERT when assertions are enabled.
     * When `APAC_DISABLE_ASSERT` is defined, this macro expands to a no-op.
     *
     * @param expr Boolean expression to validate.
     */
    #define APAC_ASSERT(expr) APAC_ALWAYS_ASSERT(expr)

    /**
     * @ingroup apac_error
     * @def APAC_NO_OVERLAP(op1, size1, op2, size2)
     * @brief Assert that two memory regions do not overlap.
     *
     * Verifies that the ranges:
     *
     *   [op1, op1 + size1) and [op2, op2 + size2)
     *
     * do not overlap in memory.
     *
     * Intended for validating aliasing constraints in low-level arithmetic kernels.
     */
    #define APAC_NO_OVERLAP(op1, size1, op2, size2)                 \
            APAC_ALWAYS_ASSERT(                                     \
                ((uintptr_t)(op1 + size1) <= (uintptr_t)(op2)) ||   \
                ((uintptr_t)(op2 + size2) <= (uintptr_t)(op1))      \
            )

    /**
     * @ingroup apac_error
     * @def APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)
     * @brief Assert that overlap is allowed only if op1 does not extend above op2.
     *
     * Allows overlap only when:
     *
     *   op1 + size1 <= op2 + size2
     *
     * or when the regions do not overlap at all.
     *
     * Used for kernels that process data from low to high addresses.
     */
    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)              \
            APAC_ALWAYS_ASSERT(                                             \
                ((uintptr_t)(op1 + size1) <= (uintptr_t)(op2 + size2)) ||   \
                ((uintptr_t)(op2 + size2) <= (uintptr_t)(op1))              \
            )

    /**
     * @ingroup apac_error
     * @def APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)
     * @brief Assert that overlap is allowed only if op1 does not extend below op2.
     *
     * Allows overlap only when:
     *
     *   op1 >= op2
     *
     * or when the regions do not overlap at all.
     *
     * Used for kernels that process data from high to low addresses.
     */
    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)      \
            APAC_ALWAYS_ASSERT(                                     \
                ((uintptr_t)(op1)) >= ((uintptr_t)(op2)) ||         \
                ((uintptr_t)(op1 + size1)) <= ((uintptr_t)(op2))    \
            )

#else

    #define APAC_ASSERT(expr)                                   do { /* nothing */ } while (0)
    #define APAC_NO_OVERLAP(op1, size1, op2, size2)             do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)  do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)  do { /* nothing */ } while (0)

#endif


/* ============================================================================
 * Error logging
 * ========================================================================== */

/**
 * @ingroup apac_error
 * @def APAC_LOG_ERR(msg)
 * @brief Log an error message to standard error.
 *
 * Prints the given message to `stderr` prefixed with `"APAC ERROR: "`.
 *
 * @param msg Null-terminated error message string.
 */
#define APAC_LOG_ERR(msg) fprintf(stderr, "APAC ERROR: %s\n", msg)

/** @} */ /* end of apac_error */


/****************************************************************************************************/
/*********************************         MISCELLANEOUS          ***********************************/
/****************************************************************************************************/

/**
 * @defgroup apac_runtime Runtime Initialization and CPU Dispatch
 * @brief Memory management hooks, CPU feature detection, and dispatch control.
 *
 * This module provides functions and global state used to configure libapac
 * at runtime, including:
 *
 * - Custom memory allocation functions
 * - CPU feature detection
 * - Selection of optimized arithmetic kernels
 *
 * Most users should only call ::apac_init() before using any arithmetic
 * functions. Advanced users may inspect or modify ::curr_cpu to experiment
 * with algorithm thresholds or kernel selection.
 *
 * @{
 */

/* ============================================================================
 * Memory management hooks
 * ========================================================================== */

/**
 * @ingroup apac_runtime
 * @brief Function pointer used by libapac for memory allocation.
 *
 * By default, this points to the system `malloc()` implementation, but can be
 * overridden via ::apac_set_mem_funcs().
 */
APAC_API extern void* (*apac_malloc)(size_t);

/**
 * @ingroup apac_runtime
 * @brief Function pointer used by libapac for memory reallocation.
 *
 * By default, this points to the system `realloc()` implementation, but can be
 * overridden via ::apac_set_mem_funcs().
 */
APAC_API extern void* (*apac_realloc)(void*, size_t);

/**
 * @ingroup apac_runtime
 * @brief Function pointer used by libapac for freeing memory.
 *
 * By default, this points to the system `free()` implementation, but can be
 * overridden via ::apac_set_mem_funcs().
 */
APAC_API extern void (*apac_free)(void*);

/**
 * @ingroup apac_runtime
 * @brief Set custom memory management functions for libapac.
 *
 * Replaces the internal memory allocation hooks used by libapac.
 * This affects all subsequent allocations performed by the library.
 *
 * @param[in] ptr1 Function used for allocation (malloc-like)
 * @param[in] ptr2 Function used for reallocation (realloc-like)
 * @param[in] ptr3 Function used for deallocation (free-like)
 *
 * @note This function should be called before ::apac_init() and before any
 *       other libapac API functions are used.
 */
APAC_API void apac_set_mem_funcs(
    void* (*ptr1)(size_t),
    void* (*ptr2)(void*, size_t),
    void (*ptr3)(void*)
);

/* ============================================================================
 * CPU detection and initialization
 * ========================================================================== */

/**
 * @ingroup apac_runtime
 * @brief Detect CPU features and select optimized kernels.
 *
 * Performs runtime CPU feature detection and configures internal dispatch
 * tables to use the most appropriate implementations available for the
 * current microarchitecture.
 *
 * @note This function is automatically called by ::apac_init().
 *       It is exposed primarily for testing and benchmarking purposes.
 */
APAC_API void apac_get_cpu_spec(void);

/**
 * @ingroup apac_runtime
 * @brief Initialize libapac runtime state.
 *
 * Initializes internal global state, including:
 *
 * - Setting default memory allocation functions
 * - Performing CPU feature detection
 * - Selecting optimized arithmetic kernels
 *
 * This function must be called before using any arithmetic routines in libapac.
 */
APAC_API void apac_init(void);

/* ============================================================================
 * CPU dispatch parameters
 * ========================================================================== */

/**
 * @ingroup apac_runtime
 * @brief CPU-specific dispatch parameters and algorithm thresholds.
 *
 * This structure contains:
 *
 * - Algorithm selection thresholds
 * - Function pointers to optimized kernel implementations
 *
 * The active instance is stored in the global ::curr_cpu variable and is used
 * internally by all arithmetic routines.
 *
 * Advanced users may modify fields in this structure to experiment with:
 *
 * - Different algorithm cutover thresholds
 * - Forcing use of specific kernel implementations
 *
 * @warning Modifying this structure may lead to incorrect results or crashes
 *          if incompatible kernels or invalid thresholds are selected.
 */
typedef struct apac_cpu_params
{
    /** Threshold for switching to balanced Karatsuba multiplication. */
    apn_size_t karatsuba_mul_balanced_threshold;

    /** Threshold for switching to unbalanced Karatsuba multiplication. */
    apn_size_t karatsuba_mul_unbalanced_threshold;

    /** Threshold for switching to Karatsuba squaring. */
    apn_size_t karatsuba_sqr_threshold;

    /** Threshold for recursive divide-and-conquer division. */
    apn_size_t dnc_div_threshold;

    /** Pointer to equal-size addition kernel. */
    apn_seg_t (*apn_add_n_ptr)(apn_seg_t*, const apn_seg_t*, const apn_seg_t*, apn_size_t);

    /** Pointer to equal-size subtraction kernel. */
    apn_seg_t (*apn_sub_n_ptr)(apn_seg_t*, const apn_seg_t*, const apn_seg_t*, apn_size_t);

    /** Pointer to add-single-limb kernel. */
    apn_seg_t (*apn_add_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);

    /** Pointer to subtract-single-limb kernel. */
    apn_seg_t (*apn_sub_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);

    /** Pointer to fused multiply-add by single limb kernel. */
    apn_seg_t (*apn_addmul_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);

    /** Pointer to fused multiply-sub by single limb kernel. */
    apn_seg_t (*apn_submul_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);

    /** Pointer to left-shift-by-<64-bits kernel. */
    apn_seg_t (*apn_lshift_lt64_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);

    /** Pointer to right-shift-by-<64-bits kernel. */
    apn_seg_t (*apn_rshift_lt64_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);

    /** Pointer to basecase multiplication kernel. */
    void (*apn_mul_bc_ptr)(apn_seg_t*, const apn_seg_t*, const apn_seg_t*, apn_size_t, apn_size_t);

    /** Pointer to basecase squaring kernel. */
    void (*apn_sqr_bc_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t);

    /** Pointer to negation kernel. */
    void (*apn_neg_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t);

    /** Pointer to copy kernel. */
    void (*apn_cpy_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t);

    /** Pointer to set/fill kernel. */
    void (*apn_set_ptr)(apn_seg_t*, apn_size_t, apn_seg_t);

    /** Pointer to comparison kernel. */
    int (*apn_cmp_ptr)(const apn_seg_t*, const apn_seg_t*, apn_size_t);

    /** Pointer to zero-test kernel. */
    int (*apn_is_zero_ptr)(const apn_seg_t*, apn_size_t);

} apac_cpu_params;

/** @} */ /* end of apac_runtime */

/****************************************************************************************************/
/*********************************          APN FUNCTIONS         ***********************************/
/****************************************************************************************************/

/**
 * @defgroup apn_api APN Low-Level Segment Arithmetic
 * @brief Low-level arbitrary-precision integer operations on limb arrays.
 *
 * These functions operate directly on arrays of limbs (`apn_seg_t`) and do not
 * perform any memory allocation or resizing.
 *
 * @par Common Requirements
 * The following conditions apply to all functions in this module unless stated otherwise:
 *
 * - All pointer arguments must point to valid memory regions.
 * - No size argument may be zero.
 * - Output buffers must already be large enough to hold the result.
 * - No bounds checking is performed.
 * - In debug builds, `APAC_ASSERT` and its variants are used to catch invalid
 *   arguments and illegal operand overlaps; these checks are disabled in
 *   release builds.
 *
 * @par Return Types
 * Functions in this module use the following return conventions:
 *
 * - `void` — No return value and no runtime memory allocation is performed.
 * - `apac_err` — Operation may fail (e.g. division or internally allocated
 *   temporaries); return value indicates success or error condition.
 * - `apn_seg_t` — Function returns a computed single-limb value such as
 *   carry-out or borrow-out.
 * - `int` — Pure comparison result with no allocation or arithmetic side effects.
 * - `apn_size_t` — Returns a computed size value (currently only `apn_clamp`).
 *
 * @{
 */

/**
 * @ingroup apn_api
 * @brief Add two arbitrary-precision numbers of equal sizes.
 *
 * Computes:
 *   result = op1 + op2
 *
 * Carry is propagated across all `size` limbs from least to most significant.
 *
 * @param[out] result Output array receiving the sum (length = size)
 * @param[in]  op1    First operand (length = size)
 * @param[in]  op2    Second operand (length = size)
 * @param[in]  size   Number of limbs in each operand
 *
 * @return Carry-out from the most significant limb (0 or 1).
 *
 * @pre If the arrays overlap, the start of `result` must not be greater than the
 *      start of either operand (i.e., `result <= op1` and `result <= op2`).
 */
APAC_API apn_seg_t apn_add_n(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

/**
 * @ingroup apn_api
 * @brief Add a single-limb value to an arbitrary-precision number.
 *
 * Computes:
 *   result = op1 + val
 *
 * Carry is propagated across all `size` limbs from least to most significant.
 *
 * @param[out] result Output array receiving the sum (length = size)
 * @param[in]  op1    Input operand (length = size)
 * @param[in]  size   Number of limbs in the operand
 * @param[in]  val    Single-limb value added to the least significant limb
 *
 * @return Carry-out from the most significant limb (0 or 1).
 *
 * @pre If the arrays overlap, the start of `result` must not be greater than the
 *      start of `op1` (i.e., `result <= op1`).
 */
APAC_API apn_seg_t apn_add_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @ingroup apn_api
 * @brief Add two arbitrary-precision numbers of unequal sizes.
 *
 * Computes:
 *   result = op1 + op2
 *
 * Carry is propagated across the overlapping limb range and into the remaining
 * limbs of `op1` as needed.
 *
 * @param[out] result Output array receiving the sum (length = size1)
 * @param[in]  op1    First operand (length = size1)
 * @param[in]  op2    Second operand (length = size2)
 * @param[in]  size1  Number of limbs in op1 and result (must be >= size2)
 * @param[in]  size2  Number of limbs in op2
 *
 * @return Carry-out from the most significant limb (0 or 1).
 *
 * @pre size1 >= size2
 * @pre If the arrays overlap, the start of `result` must not be greater than the
 *      start of either operand (i.e., `result <= op1` and `result <= op2`).
 */
APAC_API apn_seg_t apn_add(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

/**
 * @ingroup apn_api
 * @brief Subtract two arbitrary-precision numbers of equal sizes.
 *
 * Computes:
 *   result = op1 - op2
 *
 * Borrow is propagated across all `size` limbs from least to most significant.
 *
 * @param[out] result Output array receiving the difference (length = size)
 * @param[in]  op1    Minuend operand (length = size)
 * @param[in]  op2    Subtrahend operand (length = size)
 * @param[in]  size   Number of limbs in each operand
 *
 * @return Borrow-out from the most significant limb (0 or 1).
 *         A return value of 1 indicates underflow.
 *
 * @note If the magnitude of `op1` is less than the magnitude of `op2`, the result
 *       contains the two's-complement representation of `abs(op1 - op2)` modulo
 *       2^(size * APN_SEG_BITS).
 *
 * @pre If the arrays overlap, the start of `result` must not be greater than the
 *      start of either operand (i.e., `result <= op1` and `result <= op2`).
 */
APAC_API apn_seg_t apn_sub_n(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

/**
 * @ingroup apn_api
 * @brief Subtract a single-limb value from an arbitrary-precision number.
 *
 * Computes:
 *   result = op1 - val
 *
 * Borrow is propagated across all `size` limbs from least to most significant.
 *
 * @param[out] result Output array receiving the difference (length = size)
 * @param[in]  op1    Minuend operand (length = size)
 * @param[in]  size   Number of limbs in op1
 * @param[in]  val    Single-limb value subtracted from the least significant limb
 *
 * @return Borrow-out from the most significant limb (0 or 1).
 *         A return value of 1 indicates underflow.
 *
 * @note If the magnitude of `op1` is less than `val`, the result contains the
 *       two's-complement representation of `abs(op1 - val)` modulo
 *       2^(size * APN_SEG_BITS).
 *
 * @pre If the arrays overlap, the start of `result` must not be greater than the
 *      start of `op1` (i.e., `result <= op1`).
 */
APAC_API apn_seg_t apn_sub_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @ingroup apn_api
 * @brief Subtract two arbitrary-precision numbers where the first operand is not smaller.
 *
 * Computes:
 *   result = op1 - op2
 *
 * Borrow is propagated across the overlapping limb range and into the remaining
 * limbs of `op1` as needed.
 *
 * @param[out] result Output array receiving the difference (length = size1)
 * @param[in]  op1    Minuend operand (length = size1)
 * @param[in]  op2    Subtrahend operand (length = size2)
 * @param[in]  size1  Number of limbs in op1 and result (must be >= size2)
 * @param[in]  size2  Number of limbs in op2
 *
 * @return Borrow-out from the most significant limb (0 or 1).
 *         A return value of 1 indicates underflow.
 *
 * @note If the magnitude of `op1` is less than the magnitude of `op2`, the result
 *       contains the two's-complement representation of `abs(op1 - op2)` modulo
 *       2^(size1 * APN_SEG_BITS).
 *
 * @pre size1 >= size2
 * @pre If the arrays overlap, the start of `result` must not be greater than the
 *      start of either operand (i.e., `result <= op1` and `result <= op2`).
 */
APAC_API apn_seg_t apn_sub(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

/**
 * @ingroup apn_api
 * @brief Copy an arbitrary-precision number of equal size.
 *
 * Computes:
 *   result = op1
 *
 * The copy is performed from limb 0 through limb `size - 1` in increasing order.
 *
 * @param[out] result Output array receiving the copied value (length = size)
 * @param[in]  op1    Source operand (length = size)
 * @param[in]  size   Number of limbs to copy
 *
 * @pre If the arrays overlap, the start of `result` must not be greater than the
 *      start of `op1` (i.e., `result <= op1`).
 */
APAC_API void apn_cpy(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size
);

/**
 * @ingroup apn_api
 * @brief Compute the two's-complement negation of an arbitrary-precision number.
 *
 * Computes:
 *   result = -op1   (mod 2^(size * APN_SEG_BITS))
 *
 * Negation is performed using two's-complement arithmetic with carry propagation
 * from least to most significant limb.
 *
 * @param[out] result Output array receiving the negated value (length = size)
 * @param[in]  op1    Input operand to negate (length = size)
 * @param[in]  size   Number of limbs in the operand
 *
 * @pre If the arrays overlap, the start of `result` must not be greater than the
 *      start of `op1` (i.e., `result <= op1`).
 */
APAC_API void apn_neg(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size
);

/**
 * @ingroup apn_api
 * @brief Multiply two arbitrary-precision numbers of equal size.
 *
 * Computes:
 *   result = op1 * op2
 *
 * The full product is written to `result` using carry-propagating multiplication.
 *
 * @param[out] result Output array receiving the product (length = 2 * size)
 * @param[in]  op1    First operand (length = size)
 * @param[in]  op2    Second operand (length = size)
 * @param[in]  size   Number of limbs in each operand
 *
 * @return APAC_SUCCESS on success, or an error code on failure.
 *
 * @pre The `result` array must not overlap with either `op1` or `op2`.
 * @note `op1` and `op2` may overlap arbitrarily.
 */
APAC_API apac_err apn_mul_n(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

/**
 * @ingroup apn_api
 * @brief Multiply two arbitrary-precision numbers of possibly different sizes.
 *
 * Computes:
 *   result = op1 * op2
 *
 * The full product is written to `result` using carry-propagating multiplication.
 *
 * @param[out] result Output array receiving the product (length = size1 + size2)
 * @param[in]  op1    First operand (length = size1)
 * @param[in]  op2    Second operand (length = size2)
 * @param[in]  size1  Number of limbs in op1
 * @param[in]  size2  Number of limbs in op2
 *
 * @return APAC_SUCCESS on success, or an error code on failure.
 *
 * @pre The `result` array must not overlap with either `op1` or `op2`.
 * @note `op1` and `op2` may overlap arbitrarily.
 */
APAC_API apac_err apn_mul(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

/**
 * @ingroup apn_api
 * @brief Multiply an arbitrary-precision number by a single limb and add to another number.
 *
 * Computes:
 *   result += op1 * val
 *
 * Carry is propagated across `size + 1` limbs to account for the full product
 * and accumulated sum.
 *
 * @param[in,out] result Accumulator array (length = size + 1)
 * @param[in]     op1    Multiplicand operand (length = size)
 * @param[in]     size   Number of limbs in op1
 * @param[in]     val    Single-limb multiplier
 *
 * @return Carry-out from the most significant limb (0 or 1).
 *
 * @pre If the arrays overlap, the start of `result` must be strictly less than
 *      the start of `op1` (i.e., `result < op1`).
 */
APAC_API apn_seg_t apn_addmul_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @ingroup apn_api
 * @brief Multiply an arbitrary-precision number by a single limb and subtract from another number.
 *
 * Computes:
 *   result -= op1 * val
 *
 * Borrow is propagated across `size + 1` limbs to account for the full product
 * and accumulated subtraction.
 *
 * @param[in,out] result Accumulator array (length = size + 1)
 * @param[in]     op1    Multiplicand operand (length = size)
 * @param[in]     size   Number of limbs in op1
 * @param[in]     val    Single-limb multiplier
 *
 * @return Borrow-out from the most significant limb (0 or 1).
 *
 * @pre If the arrays overlap, the start of `result` must be strictly less than
 *      the start of `op1` (i.e., `result < op1`).
 */
APAC_API apn_seg_t apn_submul_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @ingroup apn_api
 * @brief Square an arbitrary-precision number.
 *
 * Computes:
 *   result = op1 * op1
 *
 * The full square is written to `result` using carry-propagating multiplication.
 *
 * @param[out] result Output array receiving the square (length = 2 * size)
 * @param[in]  op1    Input operand to square (length = size)
 * @param[in]  size   Number of limbs in op1
 *
 * @return APAC_SUCCESS on success, or an error code on failure.
 *
 * @pre The `result` array must not overlap with `op1`.
 */
APAC_API apac_err apn_sqr(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size
);

/**
 * @ingroup apn_api
 * @brief Fill an arbitrary-precision number with a constant value.
 *
 * Computes:
 *   for i in [0, size): result[i] = val
 *
 * @param[out] result Output array to fill (length = size)
 * @param[in]  size   Number of limbs to set
 * @param[in]  val    Value assigned to each limb
 */
APAC_API void apn_set(
    apn_seg_t* result,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @ingroup apn_api
 * @brief Compare two arbitrary-precision numbers by magnitude.
 *
 * Computes the magnitude comparison of two numbers of equal length:
 *
 *   compare(op1, op2)
 *
 * where comparison is performed from most significant limb to least significant.
 *
 * @param[in] op1  First operand (length = size)
 * @param[in] op2  Second operand (length = size)
 * @param[in] size Number of limbs to compare
 *
 * @return
 *   -  0 if the magnitudes are equal  
 *   -  1 if the magnitude of `op1` is greater than the magnitude of `op2`  
 *   - -1 if the magnitude of `op1` is less than the magnitude of `op2`
 *
 * @note `op1` and `op2` may overlap arbitrarily.
 */
APAC_API int apn_cmp(
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

/**
 * @ingroup apn_api
 * @brief Divide one arbitrary-precision number by another, producing quotient and remainder.
 *
 * Computes:
 *   dividend = quotient * divisor + remainder
 *
 * with:
 *   0 <= remainder < divisor
 *
 * @param[out] quotient  Output array receiving the quotient
 *                       (length = size_divd - size_dvsr + 1)
 * @param[out] remainder Output array receiving the remainder
 *                       (length = size_dvsr)
 * @param[in]  dividend  Dividend operand (length = size_divd)
 * @param[in]  divisor   Divisor operand (length = size_dvsr)
 * @param[in]  size_divd Number of limbs in dividend
 * @param[in]  size_dvsr Number of limbs in divisor
 *
 * @return APAC_SUCCESS on success, or an error code on failure.
 *
 * @pre size_divd >= size_dvsr
 * @pre The `dividend` and `divisor` arrays must not overlap.
 * @pre The `quotient` array must not overlap with any input or with `remainder`.
 * @pre The `remainder` array may overlap with `dividend` but must not overlap with `divisor`.
 */
APAC_API apac_err apn_div(
    apn_seg_t* quotient,
    apn_seg_t* remainder,
    const apn_seg_t* dividend,
    const apn_seg_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr
);

/**
 * @ingroup apn_api
 * @brief Divide an arbitrary-precision number by a single-limb divisor.
 *
 * Computes:
 *   dividend = quotient * divisor + remainder
 *
 * where `remainder` is returned as the function result and:
 *   0 <= remainder < divisor
 *
 * @param[out] quotient Output array receiving the quotient (length = size_divd)
 * @param[in]  dividend Dividend operand (length = size_divd)
 * @param[in]  divisor  Single-limb divisor
 * @param[in]  size_divd Number of limbs in dividend
 *
 * @return Single-limb remainder of the division.
 *
 * @pre The `quotient` array must not overlap with `dividend`.
 */
APAC_API apn_seg_t apn_div_one(
    apn_seg_t* quotient,
    const apn_seg_t* dividend,
    apn_seg_t divisor,
    apn_size_t size_divd
);

/**
 * @ingroup apn_api
 * @brief Right-shift an arbitrary-precision number by a small number of bits.
 *
 * Computes:
 *   result = op1 >> bit_cnt
 *
 * The shift is performed across all limbs with carry propagation from most to
 * least significant limb.
 *
 * @param[out] result Output array receiving the shifted value (length = size)
 * @param[in]  op1    Input operand to shift (length = size)
 * @param[in]  size   Number of limbs in op1
 * @param[in]  bit_cnt Number of bits to shift right (must be < APN_SEG_BITS)
 *
 * @return Bits shifted out from the least significant limb.
 *
 * @pre bit_cnt < APN_SEG_BITS
 * @pre If the arrays overlap, the start of `result` must not be less than the
 *      start of `op1` (i.e., `result >= op1`).
 */
APAC_API apn_seg_t apn_rshift(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t bit_cnt
);

/**
 * @ingroup apn_api
 * @brief Left-shift an arbitrary-precision number by a small number of bits.
 *
 * Computes:
 *   result = op1 << bit_cnt
 *
 * The shift is performed across all limbs with carry propagation from least to
 * most significant limb.
 *
 * @param[out] result Output array receiving the shifted value (length = size)
 * @param[in]  op1    Input operand to shift (length = size)
 * @param[in]  size   Number of limbs in op1
 * @param[in]  bit_cnt Number of bits to shift left (must be < APN_SEG_BITS)
 *
 * @return Bits shifted out from the most significant limb.
 *
 * @pre bit_cnt < APN_SEG_BITS
 * @pre If the arrays overlap, the start of `result` must not be greater than the
 *      start of `op1` (i.e., `result <= op1`).
 */
APAC_API apn_seg_t apn_lshift(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t bit_cnt
);

/**
 * @ingroup apn_api
 * @brief Check whether an arbitrary-precision number is zero.
 *
 * Computes whether all limbs of the input are zero:
 *
 *   return (op1 == 0)
 *
 * @param[in] op1  Input operand (length = size)
 * @param[in] size Number of limbs in op1
 *
 * @return
 *   - 0 if the value is exactly zero  
 *   - 1 if the value is non-zero
 *
 * @note `op1` may overlap arbitrarily with any other array since no writes occur.
 */
APAC_API int apn_is_zero(
    const apn_seg_t* op1,
    apn_size_t size
);

/**
 * @ingroup apn_api
 * @brief Compute the effective size of an arbitrary-precision number.
 *
 * Returns the number of most-significant limbs excluding leading zero limbs.
 * In other words, it finds the largest index `k` such that:
 *
 *   op1[k-1] != 0
 *
 * and returns `k`. If all limbs are zero, the function returns 0.
 *
 * @param[in] op1  Input operand (length = size)
 * @param[in] size Maximum number of limbs to examine
 *
 * @return Number of significant limbs (0 <= return value <= size).
 *
 * @note `op1` may overlap arbitrarily with any other array since no writes occur.
 */
APAC_API apn_size_t apn_clamp(
    const apn_seg_t* op1,
    apn_size_t size
);

/** @} */ /* end of apn_api */

/****************************************************************************************************/
/*********************************          APZ FUNCTIONS         ***********************************/
/****************************************************************************************************/

#define APZ_POS         ((int8_t)1)
#define APZ_NEG         ((int8_t)-1)
#define APZ_ZERO        ((int8_t)0)

#define APZ_MAX_SIZE    ((apn_size_t)1 << 50)

typedef struct
{
    apn_seg_t* data;
    apn_size_t max_, used_segs;
    int8_t is_neg;

} apz_t;

typedef enum
{

    BASE10 = 10,
    BASE16 = 16

} apac_str_base;

APAC_API apac_err apz_init(
    apz_t* op,
    apn_size_t size
);

APAC_API apac_err apz_free(
    apz_t* op
);

APAC_API apac_err apz_resize(
    apz_t* op,
    apn_size_t new_size
);

APAC_API apac_err apz_set(
    apz_t* op,
    apn_seg_t val
);

#endif