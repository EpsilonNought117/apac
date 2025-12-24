#ifndef APAC_H
#define APAC_H

/****************************************************************************************************/
/********************************      REQUIRED STANDARD HEADERS      *******************************/
/****************************************************************************************************/

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

    #if defined(_MSC_VER)

        #include <Windows.h> // for win32 threads

        #if defined(_M_X64) || defined(_M_AMD64)

            #define SIZE_OF_VOID_PTR 8U

            #include <immintrin.h>
            #include <intrin.h>

        #elif defined(_M_ARM64) || defined(_M_ARM64EC)
		
            #define SIZE_OF_VOID_PTR 8U

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

        #include <pthread.h>  // POSIX Thread

        #if defined(__x86_64)   || defined(__amd64)   || \
            defined(__x86_64__) || defined(__amd64__)
        
            #define SIZE_OF_VOID_PTR 8U

            #include <x86intrin.h>
            #include <cpuid.h>
            #include <immintrin.h>
        
        #elif defined(__aarch64__) || defined(__arm64__)

            #define SIZE_OF_VOID_PTR 8U

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

typedef size_t              apn_size_t;
#define PRI_APN_PTR         "p"
#define PRI_APN_SIZE        "zu"
#define APN_SIZE_MAX        SIZE_MAX

#if (SIZE_OF_VOID_PTR == 8U)

    typedef uint64_t            apn_seg_t;
    #define APN_SEG_MAX         UINT64_MAX
    #define PRI_APN_SEG         PRIu64
    #define APN_SEG_BITS        64U
    #define APN_SEG_HIGH_BIT    (1ULL << 63)

#endif

/****************************************************************************************************/
/*********************************      ERROR HANDLING MACROS      **********************************/
/****************************************************************************************************/

typedef enum apac_err
{
    APAC_OK,
    APAC_OOM

}   apac_err;

/**
 * @brief Internal core assertion implementation used by all APAC_ASSERT variants.
 *
 * This macro performs the actual assertion check and logs failure information
 * to `stderr` when the condition fails. It reports the failed expression,
 * file name, and line number, and optionally prints a formatted message
 * when a message string is provided.
 *
 * @param expr
 *     Expression to evaluate. If false, the assertion fails.
 * @param fmt
 *     Optional format string for an additional message.
 *     If this is an empty string, no message is printed.
 * @param ...
 *     Optional arguments to format according to `fmt`.
 *
 * @note
 *     This macro should not be used directly — use one of the
 *     higher-level wrappers such as @ref APAC_ASSERT,
 *     @ref APAC_DETAILED_ASSERT, or @ref APAC_ALWAYS_ASSERT.
 */
#define APAC_ASSERT_IMPL(expr, fmt, ...)                    \
        do                                                  \
        {                                                   \
            if (!(expr))                                    \
            {                                               \
                fprintf(                                    \
                    stderr,                                 \
                    "\n\nAPAC ASSERTION FAILED!\n"          \
                    "ASSERTION: %s\n"                       \
                    "FILE: %s\nLINE: %d\n",                 \
                    #expr, __FILE__, __LINE__               \
                );                                          \
                if (*(fmt))                                 \
                {                                           \
                    fprintf(                                \
                        stderr,                             \
                        "DETAILS: "                         \
                        fmt "\n",                           \
                        ##__VA_ARGS__                       \
                    );                                      \
                }                                           \
                fprintf(stderr, "ABORTING ...\n\n");        \
                abort();                                    \
            }                                               \
        }                                                   \
        while (0)

#ifndef APAC_DISABLE_ASSERT

    /**
     * @def APAC_ASSERT(expr)
     * @brief Base assertion macro for simple expression validation.
     *
     * Evaluates an expression and aborts the program if it evaluates to false.
     * Prints the expression, file name, and line number on failure.
     *
     * @param expr
     *     Boolean expression to check.
     *
     * @note
     *     This macro is disabled if @ref APAC_DISABLE_ASSERT is defined.
     */
    #define APAC_ASSERT(expr)           \
            APAC_ASSERT_IMPL(expr, "")

    /**
     * @def APAC_DETAILED_ASSERT(expr, fmt, ...)
     * @brief Assertion with an optional formatted message.
     *
     * Similar to @ref APAC_ASSERT, but allows an additional custom message
     * to be printed when the assertion fails.
     *
     * @param expr
     *     Expression to check.
     * @param fmt
     *     Format string for an optional message (printf-style).
     * @param ...
     *     Optional arguments corresponding to the format string.
     *
     * @note
     *     Disabled if @ref APAC_DISABLE_ASSERT is defined.
     */
    #define APAC_DETAILED_ASSERT(expr, fmt, ...)        \
            APAC_ASSERT_IMPL(expr, fmt, ##__VA_ARGS__)

    /**
     * @def APAC_NO_OVERLAP(op1, size1, op2, size2)
     * @brief Ensures that two memory regions do not overlap.
     *
     * Verifies that the memory range `[op1, op1 + size1)` does not intersect
     * with `[op2, op2 + size2)`. If overlap is detected, prints diagnostic
     * details and aborts execution.
     *
     * @param op1
     *     Pointer to the start of the first memory region.
     * @param size1
     *     Size of the first region, in elements or bytes.
     * @param op2
     *     Pointer to the start of the second memory region.
     * @param size2
     *     Size of the second region, in elements or bytes.
     *
     * @note
     *     Disabled if @ref APAC_DISABLE_ASSERT is defined.
     */
    #define APAC_NO_OVERLAP(op1, size1, op2, size2)                             \
        APAC_ASSERT_IMPL(                                                       \
            ((uintptr_t)(op1) + (size1) <= (uintptr_t)(op2)) ||                 \
            ((uintptr_t)(op2) + (size2) <= (uintptr_t)(op1)),                   \
            "Memory regions overlap:\n"                                         \
            "\t op1: (%" PRI_APN_PTR ") size1: (%" PRI_APN_SIZE ")\n"           \
            "\t op2: (%" PRI_APN_PTR ") size2: (%" PRI_APN_SIZE ")\n",          \
            (void *)(op1), (apn_size_t)(size1),                                 \
            (void *)(op2), (apn_size_t)(size2)                                  \
        )

    /**
     * @def APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)
     * @brief Ensures that any partial overlap occurs only below `op2`'s end.
     *
     * Checks that if two memory regions overlap, the first region (`op1`)
     * does not extend beyond the end of the second (`op2`). In other words,
     * `op1 + size1` must be less than or equal to `op2 + size2` when overlap
     * occurs.
     *
     * @param op1
     *     Pointer to the start of the first region.
     * @param size1
     *     Size of the first region.
     * @param op2
     *     Pointer to the start of the second region.
     * @param size2
     *     Size of the second region.
     *
     * @note
     *     Disabled if @ref APAC_DISABLE_ASSERT is defined.
     */

    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)                                  \
        APAC_ASSERT_IMPL(                                                                       \
            ((uintptr_t)(op1) + (size1) <= (uintptr_t)(op2)) ||                                 \
            ((uintptr_t)(op1) + (size1) <=                                                      \
             (uintptr_t)(op2) + (size2)),                                                       \
            "Invalid partial overlap: op1 extends beyond op2's end.\n"                          \
            "  op1: (%" PRI_APN_PTR ")  size1: (%" PRI_APN_SIZE ")  end: (%" PRI_APN_PTR ")\n"  \
            "  op2: (%" PRI_APN_PTR ")  size2: (%" PRI_APN_SIZE ")  end: (%" PRI_APN_PTR ")\n", \
            (void *)(op1), (apn_size_t)(size1),                                                 \
            (void *)((uintptr_t)(op1) + (size1)),                                               \
            (void *)(op2), (apn_size_t)(size2),                                                 \
            (void *)((uintptr_t)(op2) + (size2))                                                \
        )

    /**
     * @def APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)
     * @brief Ensures that any partial overlap occurs only above op2’s start.
     *
     * This check is the inverse of @ref APAC_PARTIAL_OVERLAP_BELOW.
     * It allows overlap only if the first region (`op1`) starts at or after `op2`,
     * and ensures that `op1` does not extend below `op2`'s start.
     *
     * @param op1
     *     Pointer to the start of the first region (e.g., result).
     * @param size1
     *     Size of the first region.
     * @param op2
     *     Pointer to the start of the second region (e.g., op1).
     * @param size2
     *     Size of the second region.
     */
    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)                                      \
        APAC_ASSERT_IMPL(                                                                           \
            ((uintptr_t)(op1) >= (uintptr_t)(op2) + (size2)) ||                                     \
            ((uintptr_t)(op1) + (size1) >= (uintptr_t)(op2) + (size2)),                             \
            "Invalid partial overlap: op1 extends below op2's start.\n"                             \
            "  op1: (%" PRI_APN_PTR ")  size1: (%" PRI_APN_SIZE ")  start: (%" PRI_APN_PTR ")\n"    \
            "  op2: (%" PRI_APN_PTR ")  size2: (%" PRI_APN_SIZE ")  start: (%" PRI_APN_PTR ")\n",   \
            (void *)(op1), (apn_size_t)(size1),                                                     \
            (void *)(op1),                                                                          \
            (void *)(op2), (apn_size_t)(size2),                                                     \
            (void *)(op2)                                                                           \
        )


#else

    #define APAC_ASSERT(expr)                                   do { } while (0)
    #define APAC_DETAILED_ASSERT(expr, fmt, ...)                do { } while (0)
    #define APAC_NO_OVERLAP(op1, size1, op2, size2)             do { } while (0)
    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)  do { } while (0)
    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)  do { } while (0)

#endif


/**
 * @def APAC_ALWAYS_ASSERT(expr, fmt, ...)
 * @brief Assertion that is never disabled, even when @ref APAC_DISABLE_ASSERT is defined.
 *
 * Always performs the assertion check and aborts on failure.
 * Useful for critical runtime invariants that must always be enforced.
 *
 * @param expr
 *     Expression to validate.
 * @param fmt
 *     Optional format string for a message.
 * @param ...
 *     Optional printf-style arguments.
 */
#define APAC_ALWAYS_ASSERT(expr, fmt, ...) APAC_ASSERT_IMPL(expr, fmt, __VA_ARGS__)

/**
 * @def APAC_LOG_ERR(msg)
 * @brief Logs an error message to `stderr` with the "APAC ERROR" prefix.
 *
 * @param msg
 *     Null-terminated string message to print.
 */
#define APAC_LOG_ERR(msg) fprintf(stderr, "APAC ERROR: %s\n", msg)

/****************************************************************************************************/
/*********************************         MISCELLANEOUS          ***********************************/
/****************************************************************************************************/

APAC_API void* (*apac_malloc)(size_t);
APAC_API void* (*apac_realloc)(void*, size_t);
APAC_API void (*apac_free)(void*);

APAC_API void apacSetMemFuncs(
	void* (*ptr1)(size_t),
	void* (*ptr2)(void*, size_t),
	void (*ptr3)(void*)
);

APAC_API void apacGetCPUSpec(void);

APAC_API void apacInit(void);

typedef struct apac_cpu_params
{
	apn_size_t karatsuba_mul_balanced_threshold;
	apn_size_t karatsuba_mul_unbalanced_threshold;
	apn_size_t karatsuba_sqr_threshold;
    apn_size_t dnc_div_threshold;   // recursive divide-&-conquer division threshold

	apn_seg_t(*apn_add_n_ptr)(apn_seg_t*, const apn_seg_t*, const apn_seg_t*, apn_size_t);
	apn_seg_t(*apn_sub_n_ptr)(apn_seg_t*, const apn_seg_t*, const apn_seg_t*, apn_size_t);
    apn_seg_t(*apn_add_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);
    apn_seg_t(*apn_sub_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);
    apn_seg_t(*apn_addmul_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);
    apn_seg_t(*apn_submul_one_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);
    apn_seg_t(*apn_lshift_lt64_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);
    apn_seg_t(*apn_rshift_lt64_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t, apn_seg_t);

    void (*apn_mul_bc_ptr)(apn_seg_t*, const apn_seg_t*, const apn_seg_t*, apn_size_t, apn_size_t);
	void (*apn_sqr_bc_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t);
    void (*apn_neg_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t);
	void (*apn_cpy_ptr)(apn_seg_t*, const apn_seg_t*, apn_size_t);
    void (*apn_set_ptr)(apn_seg_t*, apn_size_t, apn_seg_t);

    int (*apn_cmp_ptr)(const apn_seg_t*, const apn_seg_t*, apn_size_t);
    int (*apn_is_zero_ptr)(const apn_seg_t*, apn_size_t);

}   apac_cpu_params;

/****************************************************************************************************/
/*********************************          APN FUNCTIONS         ***********************************/
/****************************************************************************************************/

/**
* ---------------------------------------- IMPORTANT NOTES -----------------------------------------
*
* 1) THESE FUNCTIONS DO NOT PERFORM ANY MEMORY ALLOCATIONS IN INPUT OPERAND/S AND RESULT/S.
*
* 2) ASSERTS ARE USED IN DEBUG MODE FOR CATCHING INVALID ARGUMENT ERRORS.
* 
* 3) THEY DO NOT PERFORM ANY BOUNDS CHECKING.
* 
* 4) NO POINTER INPUT SHOULD BE NULL.
* 
* 5) THE SIZE INPUTS SHOULD NOT BE ZERO.
* 
* 6) IN CASES OF TWO INPUT SIZES, SIZE-1 IS ALWAYS GREATER THAN SIZE-2
*
* 7) SOME BUT NOT ALL FUNCTIONS ALLOW INPUTS/RESULTS OVERLAP (Will be covered in documentation soon)
*/

/**
* -------------------------------------- RETURN TYPES ----------------------------------------
*
* 1) void (functions that don't perform runtime memory allocation and don't return anything)
* 
* 2) apac_err (functions that either perform division and/or perform memory allocations)
* 
* 3) apn_seg_t (functions that have some computed return value such as carry/borrow out)
* 
* 4) int (functions that don't perform memory allocation or computation, but do comparisions)
* 
* 5) apn_size_t (only apn_clamp returns this)
*/

/**
 * @brief Add two arbitrary-precision numbers of `size` segments each (carry-propagating).
 *
 * This function performs a carry-propagating addition of two multi-segment
 * arbitrary-precision numbers (`op1` and `op2`), each consisting of `size`
 * segments of type apn_seg_t.
 * Addition begins at the least-significant segment and carries are propagated
 * through the segment array as needed until the most-significant segment.
 *
 * The sum is written to the `result` array.
 *
 * @pre `result`, `op1`, and `op2` each point to valid memory containing at least
 *      `size` segments.
 * @pre Partial overlaps between arrays are not allowed; only full aliasing
 *      (`result == op1` or `result == op2`) is permitted.
 *
 * @param[out] result
 *     Pointer to the output array receiving the sum (length = `size` segments).
 *
 * @param[in] op1
 *     Pointer to the first arbitrary-precision number (length = `size` segments).
 *
 * @param[in] op2
 *     Pointer to the second arbitrary-precision number (length = `size` segments).
 *
 * @param[in] size
 *     Number of segments in each operand.
 *
 * @return apn_seg_t
 *     Final carry-out propagated from the most significant segment (0 or 1).
 */
APAC_API apn_seg_t apn_add_n(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

/**
 * @brief Add a single-segment value to an arbitrary-precision number (carry-propagating).
 *
 * This function performs a carry-propagating addition of an arbitrary-precision
 * number (`op1`), consisting of `size` segments, with a single-segment value
 * (`val`).
 * The least-significant segment is incremented by `val`, and any resulting
 * carries are propagated through the segment array as needed.
 *
 * The sum is written to the `result` array.
 *
 * @pre `result` and `op1` each point to valid memory containing at least
 *      `size` segments.
 * @pre Partial overlaps are not allowed; only full aliasing
 *      (`result == op1`) is permitted.
 *
 * @param[out] result
 *     Pointer to the output array receiving the sum (length = `size` segments).
 *
 * @param[in] op1
 *     Pointer to the arbitrary-precision number (length = `size` segments).
 *
 * @param[in] size
 *     Number of segments in the operand.
 *
 * @param[in] val
 *     Single-segment value to add to the least-significant segment of `op1`.
 *
 * @return apn_seg_t
 *     Final carry-out propagated from the most significant segment (0 or 1).
 */
APAC_API apn_seg_t apn_add_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @brief Add two arbitrary-precision numbers of sizes `size1` and `size2`
 *        (carry-propagating).
 *
 * This function performs a carry-propagating addition of two arbitrary-precision
 * numbers (`op1` and `op2`) whose segment counts may differ.
 * The addition begins at the least-significant segment and carries are
 * propagated through the overlapping segment range.
 * If one operand is longer than the other, its remaining segments are copied
 * while continuing to propagate carries as needed.
 *
 * The sum is written to the `result` array.
 *
 * @pre `result`, `op1`, and `op2` each point to valid memory containing at
 *      least `size1` and `size2` segments respectively.
 * @pre Partial overlaps are not allowed; only full aliasing
 *      (`result == op1` or `result == op2`) is permitted.
 *
 * @param[out] result
 *     Pointer to the output array receiving the sum (length = max(size1, size2)).
 *
 * @param[in] op1
 *     Pointer to the first arbitrary-precision number (`size1` segments).
 *
 * @param[in] op2
 *     Pointer to the second arbitrary-precision number (`size2` segments).
 *
 * @param[in] size1
 *     Number of segments in `op1`.
 *
 * @param[in] size2
 *     Number of segments in `op2`.
 *
 * @return apn_seg_t
 *     Final carry-out propagated from the most significant segment (0 or 1).
 */
APAC_API apn_seg_t apn_add(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

/**
 * @brief Subtract two arbitrary-precision numbers of `size` segments each
 *        (borrow-propagating).
 *
 * This function performs a borrow-propagating subtraction of two equal-sized
 * arbitrary-precision numbers (`op1 - op2`).
 * Subtraction starts at the least-significant segment and borrows are
 * propagated as necessary through the entire segment array.
 *
 * The difference is written to the `result` array.
 *
 * @pre `result`, `op1`, and `op2` each point to valid memory containing at least
 *      `size` segments.
 * @pre Partial overlaps are not allowed; only full aliasing
 *      (`result == op1` or `result == op2`) is permitted.
 *
 * @param[out] result
 *     Output array receiving the difference (length = `size` segments).
 *
 * @param[in] op1
 *     Minuend operand (`size` segments).
 *
 * @param[in] op2
 *     Subtrahend operand (`size` segments).
 *
 * @param[in] size
 *     Number of segments in each operand.
 *
 * @return apn_seg_t
 *     Final borrow-out from the most significant segment (0 or 1).
 *     (1 means underflow occurred.)
 */
APAC_API apn_seg_t apn_sub_n(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

/**
 * @brief Subtract a single-segment value from an arbitrary-precision number
 *        (borrow-propagating).
 *
 * This function subtracts a single-segment value (`val`) from the least-
 * significant segment of an arbitrary-precision number (`op1`, length = `size`),
 * propagating borrows as needed through the segment array.
 *
 * The result is written to the `result` array.
 *
 * @pre `result` and `op1` each point to valid memory containing at least
 *      `size` segments.
 * @pre Partial overlaps are not allowed; only full aliasing
 *      (`result == op1`) is permitted.
 *
 * @param[out] result
 *     Output array receiving the difference (length = `size` segments).
 *
 * @param[in] op1
 *     Minuend operand (`size` segments).
 *
 * @param[in] size
 *     Number of segments in `op1`.
 *
 * @param[in] val
 *     Single-segment value to subtract from the least-significant segment.
 *
 * @return apn_seg_t
 *     Final borrow-out from the most significant segment (0 or 1).
 */
APAC_API apn_seg_t apn_sub_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @brief Subtract two arbitrary-precision numbers of sizes `size1` and `size2`
 *        (borrow-propagating).
 *
 * This function performs a borrow-propagating subtraction of two arbitrary-
 * precision numbers (`op1 - op2`) whose sizes may differ.
 * Subtraction begins at the least-significant segment of both operands.
 *
 * Borrow propagation continues through the overlapping segments, and if
 * `op1` is longer than `op2`, its remaining segments are processed while
 * continuing to propagate borrows as needed.
 *
 * The difference is written to the `result` array.
 *
 * @pre `result` and `op1` each point to valid memory containing at least
 *      `size1` segments.
 * @pre `op2` points to valid memory containing at least `size2` segments.
 * @pre Partial overlaps are not allowed; only full aliasing
 *      (`result == op1`) is permitted.
 *
 * @param[out] result
 *     Output array receiving the difference (length = size1 segments).
 *
 * @param[in] op1
 *     Minuend operand (`size1` segments).
 *
 * @param[in] op2
 *     Subtrahend operand (`size2` segments).
 *
 * @param[in] size1
 *     Number of segments in `op1`.
 *
 * @param[in] size2
 *     Number of segments in `op2`.
 *
 * @return apn_seg_t
 *     Final borrow-out from the most significant segment (0 or 1).
 */
APAC_API apn_seg_t apn_sub(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

/**
 * @brief Copy an arbitrary-precision number of `size` segments.
 *
 * This function copies an arbitrary-precision number (`op1`) consisting of
 * `size` segments into the destination array (`result`).
 *
 * The copy is performed from segment 0 through segment `size - 1` in order.
 *
 * @pre `result` and `op1` each point to valid memory containing at least
 *      `size` segments.
 * @pre Partial overlaps between arrays are not allowed; full aliasing
 *      (`result == op1`) is permitted.
 *
 * @param[out] result
 *     Pointer to the output array receiving the copied segments.
 *
 * @param[in] op1
 *     Pointer to the source arbitrary-precision number (length = `size` segments).
 *
 * @param[in] size
 *     Number of segments to copy.
 */
APAC_API void apn_cpy(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size
);

/**
 * @brief Compute the two's complement negation of an arbitrary-precision number.
 *
 * This function negates an arbitrary-precision number (`op1`) of `size` segments
 * using two's complement arithmetic, storing the result in `result`.
 * The negation is equivalent to computing `(0 - op1)` in a carry-propagating manner.
 *
 * @pre `result` and `op1` each point to valid memory containing at least
 *      `size` segments.
 * @pre Partial overlaps between arrays are not allowed; full aliasing
 *      (`result == op1`) is permitted.
 *
 * @param[out] result
 *     Pointer to the output array receiving the negated value (length = `size` segments).
 *
 * @param[in] op1
 *     Pointer to the input arbitrary-precision number to negate (length = `size` segments).
 *
 * @param[in] size
 *     Number of segments in the operand.
 */
APAC_API void apn_neg(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size
);

/**
 * @brief Multiply two arbitrary-precision numbers of `size` segments each.
 *
 * This function performs a carry-propagating multiplication of two arbitrary-
 * precision numbers (`op1` and `op2`), each consisting of `size` segments.
 * The full product is written to the `result` array.
 *
 * @pre `result`, `op1`, and `op2` each point to valid memory containing at least
 *      `size` segments for `op1` and `op2`, and `2 * size` segments for `result`.
 * @pre The `result` array **must not overlap** with `op1` or `op2`.
 *
 * @param[out] result
 *     Pointer to the output array receiving the product (length = `2 * size` segments).
 *
 * @param[in] op1
 *     Pointer to the first operand (length = `size` segments).
 *
 * @param[in] op2
 *     Pointer to the second operand (length = `size` segments).
 *
 * @param[in] size
 *     Number of segments in each operand.
 *
 * @return apac_err
 *     Error code indicating success or failure of the multiplication.
 */
APAC_API apac_err apn_mul_n(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

/**
 * @brief Multiply two arbitrary-precision numbers of potentially different sizes.
 *
 * This function performs a carry-propagating multiplication of two arbitrary-
 * precision numbers (`op1` of `size1` segments and `op2` of `size2` segments).
 * The full product is written to the `result` array.
 *
 * @pre `result` must point to valid memory containing at least `size1 + size2` segments.
 * @pre `op1` and `op2` must point to valid memory containing at least `size1` and
 *      `size2` segments, respectively.
 * @pre The `result` array **must not overlap** with `op1` or `op2`.
 *
 * @param[out] result
 *     Pointer to the output array receiving the product (length = `size1 + size2` segments).
 *
 * @param[in] op1
 *     Pointer to the first operand (length = `size1` segments).
 *
 * @param[in] op2
 *     Pointer to the second operand (length = `size2` segments).
 *
 * @param[in] size1
 *     Number of segments in the first operand.
 *
 * @param[in] size2
 *     Number of segments in the second operand.
 *
 * @return apac_err
 *     Error code indicating success or failure of the multiplication.
 */
APAC_API apac_err apn_mul(
    apn_seg_t* result,
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

/**
 * @brief Multiply an arbitrary-precision number by a single-segment value and add the result to another number.
 *
 * This function computes a carry-propagating operation equivalent to:
 * ```
 * result[] += op1[] * val
 * ```
 * where `op1` has `size` segments and `val` is a single-segment value.
 * The result is written to the `result` array, which must have `size + 1` segments
 * to accommodate potential carry-out.
 *
 * @pre `result` and `op1` each point to valid memory containing at least
 *      `size + 1` and `size` segments, respectively.
 * @pre Partial or full aliasing between `op1` and `result` is allowed.
 *
 * @param[out] result
 *     Pointer to the output array receiving the sum (length = `size + 1` segments).
 *
 * @param[in] op1
 *     Pointer to the arbitrary-precision operand (length = `size` segments).
 *
 * @param[in] size
 *     Number of segments in `op1`.
 *
 * @param[in] val
 *     Single-segment multiplier to multiply with `op1` before addition.
 *
 * @return apn_seg_t
 *     Carry-out from the most significant segment (0 or 1).
 */
APAC_API apn_seg_t apn_addmul_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @brief Multiply an arbitrary-precision number by a single-segment value and subtract the result from another number.
 *
 * This function computes a borrow-propagating operation equivalent to:
 * ```
 * result[] -= op1[] * val
 * ```
 * where `op1` has `size` segments and `val` is a single-segment value.
 * The result is written to the `result` array, which must have `size + 1` segments
 * to accommodate potential borrow-out.
 *
 * @pre `result` and `op1` each point to valid memory containing at least
 *      `size + 1` and `size` segments, respectively.
 * @pre Partial or full aliasing between `op1` and `result` is allowed.
 *
 * @param[out] result
 *     Pointer to the output array receiving the difference (length = `size + 1` segments).
 *
 * @param[in] op1
 *     Pointer to the arbitrary-precision operand (length = `size` segments).
 *
 * @param[in] size
 *     Number of segments in `op1`.
 *
 * @param[in] val
 *     Single-segment multiplier to multiply with `op1` before subtraction.
 *
 * @return apn_seg_t
 *     Borrow-out from the most significant segment (0 or 1).
 */
APAC_API apn_seg_t apn_submul_one(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @brief Square an arbitrary-precision number.
 *
 * Computes the carry-propagating square of `op1` (length = `size` segments)
 * and stores the result in `result` (length = `2 * size` segments).
 *
 * @pre `result` must not overlap with `op1`.
 *
 * @param[out] result
 *     Output array receiving the square.
 *
 * @param[in] op1
 *     Input number to square.
 *
 * @param[in] size
 *     Number of segments in `op1`.
 *
 * @return apac_err
 *     Error code indicating success or failure.
 */
APAC_API apac_err apn_sqr(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size
);

/**
 * @brief Fill an arbitrary-precision number with a constant value.
 *
 * Sets all `size` segments of `result` to `val`.
 *
 * @param[out] result
 *     Array to fill.
 *
 * @param[in] size
 *     Number of segments to set.
 *
 * @param[in] val
 *     Value to assign to each segment.
 */
APAC_API void apn_set(
    apn_seg_t* result,
    apn_size_t size,
    apn_seg_t val
);

/**
 * @brief Compare two arbitrary-precision numbers.
 *
 * Compares `op1` and `op2` (length = `size` segments each).
 *
 * @param[in] op1
 *     First number.
 *
 * @param[in] op2
 *     Second number.
 *
 * @param[in] size
 *     Number of segments to compare.
 *
 * @return int
 *     0 if equal, 1 if `op1 > op2`, -1 if `op1 < op2`.
 */
APAC_API int apn_cmp(
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
);

/**
 * @brief Divide an arbitrary-precision number by another, producing quotient and remainder.
 *
 * Computes the quotient and remainder of `dividend / divisor`.
 *
 * @pre `quotient` must have length = (size_divd - size_dvsr + 1).
 * @pre `remainder` must have length = size_dvsr.
 *
 * @param[out] quotient
 *     Array receiving the quotient.
 *
 * @param[out] remainder
 *     Array receiving the remainder.
 *
 * @param[in] dividend
 *     Dividend array (length = size_divd).
 *
 * @param[in] divisor
 *     Divisor array (length = size_dvsr).
 *
 * @param[in] size_divd
 *     Number of segments in dividend.
 *
 * @param[in] size_dvsr
 *     Number of segments in divisor.
 *
 * @return apac_err
 *     Error code indicating success or failure.
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
 * @brief Divide an arbitrary-precision number by a single-segment divisor.
 *
 * Computes quotient and remainder of `dividend / divisor64`.
 *
 * @pre `quotient` must have length = size_divd.
 *
 * @param[out] quotient
 *     Array receiving the quotient.
 *
 * @param[in] dividend
 *     Dividend array (length = size_divd).
 *
 * @param[in] divisor64
 *     Single-segment divisor.
 *
 * @param[in] size_divd
 *     Number of segments in dividend.
 *
 * @return apn_seg_t
 *     Single-segment remainder.
 */
APAC_API apn_seg_t apn_div_one(
    apn_seg_t* quotient,
    const apn_seg_t* dividend,
    apn_seg_t divisor64,
    apn_size_t size_divd
);

/**
 * @brief Right-shift an arbitrary-precision number by a number of bits.
 *
 * Performs a carry-propagating right shift of `op1` by `bit_cnt` bits and stores
 * the result in `result`.
 *
 * @param[out] result
 *     Array receiving the shifted value (length = size).
 *
 * @param[in] op1
 *     Input number to shift.
 *
 * @param[in] size
 *     Number of segments in `op1`.
 *
 * @param[in] bit_cnt
 *     Number of bits to shift right.
 *
 * @return apn_seg_t
 *     Bits shifted out from the least significant end.
 */
APAC_API apn_seg_t apn_rshift(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t bit_cnt
);

/**
 * @brief Left-shift an arbitrary-precision number by a number of bits.
 *
 * Performs a carry-propagating left shift of `op1` by `bit_cnt` bits and stores
 * the result in `result`.
 *
 * @param[out] result
 *     Array receiving the shifted value (length = size).
 *
 * @param[in] op1
 *     Input number to shift.
 *
 * @param[in] size
 *     Number of segments in `op1`.
 *
 * @param[in] bit_cnt
 *     Number of bits to shift left.
 *
 * @return apn_seg_t
 *     Bits shifted out from the most significant end.
 */
APAC_API apn_seg_t apn_lshift(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t bit_cnt
);

/**
 * @brief Check if an arbitrary-precision number is zero.
 *
 * Checks whether all `size` segments of `op1` are zero.
 *
 * @param[in] op1
 *     Number to check.
 *
 * @param[in] size
 *     Number of segments in `op1`.
 *
 * @return int
 *     0 if `op1` is zero, 1 otherwise.
 */
APAC_API int apn_is_zero(
    const apn_seg_t* op1,
    apn_size_t size
);

APAC_API apn_size_t apn_clamp(
    const apn_seg_t* op1,
    apn_size_t size
);

#endif