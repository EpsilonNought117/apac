#ifndef APAC_H
#define APAC_H

/****************************************************************************************************/
/*********************************      REQUIRED STANDARD HEADERS     ********************************/
/****************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <intrin.h>
#include <immintrin.h>
#include <ammintrin.h>

/****************************************************************************************************/
/*********************************       SHARED AND STATIC DEFS      ********************************/
/****************************************************************************************************/

#if defined(BUILD_SHARED_LIBS)
// Export symbols when building the DLL
#define APAC_API __declspec(dllexport)
#elif defined(LIBAPAC_DLL)
// Import symbols when using the DLL
#define APAC_API __declspec(dllimport)
#else
// Static library, no import/export needed
#define APAC_API
#endif

/****************************************************************************************************/
/*********************************         MISCELLANEOUS          ***********************************/
/****************************************************************************************************/

// for error handling and assertion

typedef enum apac_err
{
    APAC_OK,
    APAC_OOM,
    APAC_MATH_ERR

}   apac_err;

#ifndef APAC_REPORT_ERR
#define APAC_REPORT_ERR(x) fprintf(stderr, "APAC ERROR [%s:%d]: %s\n", __FILE__, __LINE__, x);
#endif

#ifndef APAC_ASSERT
#define APAC_ASSERT(x)      \
if (!(x))                   \
{                           \
    APAC_REPORT_ERR(#x)     \
    abort();                \
}                   
#endif

// for setMemFuncs()

APAC_API extern void* (*apac_malloc)(size_t);
APAC_API extern void* (*apac_realloc)(void*, size_t);
APAC_API extern void (*apac_free)(void*);

APAC_API void setMemFuncs(
    void* (*ptr1)(size_t),
    void* (*ptr2)(void*, size_t),
    void (*ptr3)(void*)
);

APAC_API void apacInitDefault(void);

/****************************************************************************************************/
/*********************************        FETCH CPU FEATURES       **********************************/
/****************************************************************************************************/

// cpu feature flags

extern uint8_t adx_chk;
extern uint8_t bmi2_chk;
extern uint8_t avx2_chk;
extern uint8_t avx_chk;
extern uint8_t avx512f_chk;

APAC_API void getCPUSpec(void);

/****************************************************************************************************/
/*********************************          APN DEFINITIONS        **********************************/
/****************************************************************************************************/

typedef uint64_t u64;
typedef uint16_t u16;
typedef uint8_t   u8;
typedef int64_t  i64;

/****************************************************************************************************/
/*********************************          APN FUNCTIONS         ***********************************/
/****************************************************************************************************/


/**               
*                   IMPORTANT NOTES
* 
* 1) THESE FUNCTIONS DO NOT PERFORM ANY MEMORY ALLOCATIONS.
*
* 2) ASSERTS ARE USED FOR DEBUG BUILDS TO CATCH ANY ERRORS.
* 
* 3) THEY ASSUME THAT THE RESULT HAS REQUIRED NUMBER OF LIMBS.
* 
* 4) THEY DO NOT PERFORM ANY BOUNDS CHECKING IN RELEASE BUILDS.
* 
* 5) VERY LITTLE WORK IS DONE APART FROM THE ACTUAL COMPUTATION NEEDED.
* 
* 6) NO SIZE ARGUMENT SHOULD BE ZERO, ALWAYS PASS AT LEAST SIZE 1.
* 
*/

/*
    1) result must have "size" limbs
*/
APAC_API u8 apn_add_n(u64* result, const u64* op1, const u64* op2, u64 size);

/*
    1) size1 must be greater than or equal to size2.
    2) result must have size1 limbs
*/
APAC_API u8 apn_add(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/*
    1) result must have "size" limbs
*/
APAC_API u8 apn_add_one(u64* result, const u64* op1, u64 size, u64 val);

/*
    1) result must have "size" limbs
*/
APAC_API u8 apn_sub_n(u64* result, const u64* op1, const u64* op2, u64 size);

/*
    1) size1 must be greater than or equal to size2.
    2) result must have size1 limbs
*/
APAC_API u8 apn_sub(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/*
    1) result must have "size" limbs
*/
APAC_API u8 apn_sub_one(u64* result, const u64* op1, u64 size, u64 val);

/*
    1) result must have "size" number of limbs
*/
APAC_API void apn_cpy(u64* result, const u64* op1, u64 size);

/*
    1) result must have "size" number of limbs
*/
APAC_API void apn_negate(u64* result, const u64* op1, u64 size);

/*
    
*/
APAC_API void apn_mul_n(u64* result, const u64* op1, const u64* op2, u64 size);

#endif