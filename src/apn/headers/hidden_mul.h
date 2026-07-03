#ifndef APN_HIDDEN_MUL
#define APN_HIDDEN_MUL

#include "../../header/apac_internal.h"

/*
 * 1) Calls multiplication routines written in assembly
 * 2) Unbalanced & balanced multiplications can be done
 * 3) O(n^2) schoolbook multiplication
 * 4) size1 must be greater than or equal to size2
 * 5) In-place, O(1) space requirement
 */
void apn_basecase_mul(
    apn_dig_t* result,
    const apn_dig_t* op1,
    const apn_dig_t* op2,
    apn_size_t size1,
    apn_size_t size2
);

/*
 * 1) Subtractive Karatsuba variant
 * 2) Balanced multiplication
 * 3) Uses (2 * n + 64) limbs of space for size n operands
 * 4) O(n^1.585) time complexity
 * 5) size1 must be greater than or equal to size2
 */
void apn_karatsuba_mul(
    apn_dig_t* result,
    const apn_dig_t* op1,
    const apn_dig_t* op2,
    apn_size_t size1,
    apn_size_t size2,
    apn_dig_t* temp
);

#endif