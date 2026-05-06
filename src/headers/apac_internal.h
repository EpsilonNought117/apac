#ifndef APAC_INTERNAL_H
#define APAC_INTERNAL_H

#include "../../include/apac.h"

/****************************************************************************************************/
/*********************************      ERROR HANDLING MACROS      **********************************/
/****************************************************************************************************/

/* ==========================================================================
 * Always-Enabled Assertion
 * ========================================================================== */

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


/* ==========================================================================
 * Debug-Only Assertion
 * ========================================================================== */

#ifndef APAC_DISABLE_ASSERT
    #define APAC_ASSERT(expr) APAC_ALWAYS_ASSERT(expr)
#else
    #define APAC_ASSERT(expr)   do { /* nothing */ } while (0)
#endif

/* ==========================================================================
 * Debug-Only Overlap Checks
 * ========================================================================== */

/*
    These checks are only useful on platforms with a flat address space per process.
*/
#if !defined(APAC_DISABLE_ASSERT)   &&  \
    (defined(APAC_X64_WIN)          ||  \
     defined(APAC_X64_UNIX)         ||  \
     defined(APAC_ARM64_WIN)        ||  \
     defined(APAC_ARM64_UNIX)           \
    )

    #define APAC_NO_OVERLAP(op1, size1, op2, size2)                         \
        APAC_ALWAYS_ASSERT(                                                 \
            ((uintptr_t)(op1) + (size1)) <= (uintptr_t)(op2) ||             \
            ((uintptr_t)(op2) + (size2)) <= (uintptr_t)(op1)                \
        )

    #define APAC_FULL_ALIAS_ONLY(op1, size1, op2, size2)                    \
        APAC_ALWAYS_ASSERT(                                                 \
            (                                                               \
                (uintptr_t)(op1) == (uintptr_t)(op2) &&                     \
                (uintptr_t)(op1) + (size1) == (uintptr_t)(op2) + (size2)    \
            ) ||                                                            \
            (                                                               \
                ((uintptr_t)(op1) + (size1)) <= (uintptr_t)(op2) ||         \
                ((uintptr_t)(op2) + (size2)) <= (uintptr_t)(op1)            \
            )                                                               \
        )

    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)              \
        APAC_ALWAYS_ASSERT(                                                 \
            ((uintptr_t)(op1) + (size1)) <= ((uintptr_t)(op2) + (size2)) || \
            ((uintptr_t)(op2) + (size2)) <= (uintptr_t)(op1)                \
        )

    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)              \
        APAC_ALWAYS_ASSERT(                                                 \
            ((uintptr_t)(op2) + (size2)) <= ((uintptr_t)(op1) + (size1)) || \
            ((uintptr_t)(op1) + (size1)) <= (uintptr_t)(op2)                \
        )

#else

    #define APAC_NO_OVERLAP(op1, size1, op2, size2)             do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_ABOVE(op1, size1, op2, size2)  do { /* nothing */ } while (0)
    #define APAC_PARTIAL_OVERLAP_BELOW(op1, size1, op2, size2)  do { /* nothing */ } while (0)

#endif

/* ==========================================================================
 * Error Logging
 * ========================================================================== */

#define APAC_LOG_ERR(msg) fprintf(stderr, "APAC ERROR: %s\n", msg)

/****************************************************************************************************/
/************************** CPU DYNAMIC DISPATCH AND CUSTOM MEM ALLOCATOR ***************************/
/****************************************************************************************************/

/* ==========================================================================
 * Global Custom Memory Allocator Struct
 * ========================================================================== */

apac_alloc_t apac_allocator = { 0 };

/* ==========================================================================
 * Runtime CPU Detection and Low-Level Function Dispatch Struct
 * ========================================================================== */

typedef struct apac_cpu_params
{
    ap_size_t karatsuba_mul_threshold;
    ap_size_t toomcook3_mul_threshold;
    
    ap_size_t karatsuba_sqr_threshold;
    ap_size_t toomcook3_sqr_threshold;

    ap_size_t dnc_div_threshold;

    ap_dig_t (*apn_add_n_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t);
    ap_dig_t (*apn_sub_n_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t);
    ap_dig_t (*apn_add_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t (*apn_sub_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    void (*apn_neg_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t);

    ap_dig_t (*apn_addmul_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t (*apn_submul_one_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);

    ap_dig_t (*apn_lshift_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t (*apn_rshift_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);

    ap_dig_t (*apn_lshift_add_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);
    ap_dig_t (*apn_lshift_sub_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t, ap_dig_t);

    void (*apn_mul_bc_ptr)(ap_dig_t*, const ap_dig_t*, const ap_dig_t*, ap_size_t, ap_size_t);
    void (*apn_sqr_bc_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t);

    void (*apn_cpy_ptr)(ap_dig_t*, const ap_dig_t*, ap_size_t);
    void (*apn_set_ptr)(ap_dig_t*, ap_size_t, ap_dig_t);

    int (*apn_cmp_ptr)(const ap_dig_t*, const ap_dig_t*, ap_size_t);
    int (*apn_is_zero_ptr)(const ap_dig_t*, ap_size_t);

} apac_cpu_params;

apac_cpu_params curr_cpu = { 0 };

/* ==========================================================================
 * Algorithm Switching Threshold Macros
 * ========================================================================== */

#define KARATSUBA_MUL_THRESHOLD (curr_cpu.karatsuba_mul_threshold)
#define TOOMCOOK3_MUL_THRESHOLD (curr_cpu.toomcook3_mul_threshold)
#define KARATSUBA_SQR_THRESHOLD	(curr_cpu.karatsuba_sqr_threshold)
#define TOOMCOOK3_SQR_THRESHOLD (curr_cpu.toomcook3_sqr_threshold)
#define DNC_DIV_THRESHOLD	    (curr_cpu.dnc_div_threshold)

/****************************************************************************************************/
/****************************           USEFUL FUNCTION MACROS           ****************************/
/****************************************************************************************************/

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

	#define CLZ(value, count)																	\
			do																					\
			{																					\
				uint32_t idx = 0;																\
				(count) = _BitScanReverse64(&idx, (value)) ? (uint32_t)63 - idx : (uint32_t)64;	\
			} while (0)

	#define CTZ(value, count)														\
			do																		\
			{																		\
				uint32_t idx = 0;													\
				(count) = _BitScanForward64(&idx, (value)) ? idx : (uint32_t)64;	\
			} while (0)

	#define ROTL(value, count)	do { value = _rotl64((value), (count)); } while (0)

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

	#define CLZ(value, count)															\
			do																			\
			{																			\
				(count) = (value) ? (uint32_t)__builtin_clzll((value)) : (uint32_t)64;	\
			} while (0)

	#define CTZ(value, count)															\
			do																			\
			{																			\
				(count) = (value) ? (uint32_t)__builtin_ctzll((value)) : (uint32_t)64;	\
			} while(0)

	#define ROTL(value, count)									\
	do															\
	{															\
		value = (												\
			(((uint64_t)(value)) << ((count) & 63)) 	   	| 	\
			(((uint64_t)(value)) >> (64 - ((count) & 63)))		\
		);														\
	} while (0)

#else

	#error "Unknown Compiler, OS Platform and CPU ISA!"

#endif

#endif