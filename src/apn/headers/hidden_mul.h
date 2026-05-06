#ifndef APN_HIDDEN_MUL
#define APN_HIDDEN_MUL

#include "../../include/apac.h"
#include "apac_internal.h"

/*
 * 1) Calls multiplication routines written in assembly
 * 2) Unbalanced & balanced multiplications can be done
 * 3) O(n^2) schoolbook multiplication
 * 4) size1 must be greater than or equal to size2
 * 5) In-place, O(1) space requirement
 */
void apn_basecase_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2
);

/*
 * 1) Subtractive Karatsuba variant
 * 2) Balanced multiplication
 * 3) Uses (2 * n + 64) limbs of space for size n operands
 * 4) O(n^1.585) time complexity
 */
void apn_karatsuba_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size,
    ap_dig_t* temp
);

void apn_toomcook3_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size,
    ap_dig_t* temp
);

void apn_toomcook32_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2,
    ap_dig_t* temp
);

void apn_toomcook43_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2,
    ap_dig_t* temp
);

void apn_toomcook42_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2,
    ap_dig_t* temp
);

#endif