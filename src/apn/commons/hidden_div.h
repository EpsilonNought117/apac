#ifndef APN_HIDDEN_DIV
#define APN_HIDDEN_DIV

#include "../../../include/apac.h"

/*
* 1) O(n * m) division
* 2) Dividend should have size_divd number of segments
* 3) Quotient should have size_quot number of segments
* 4) Divisor should have size_dvsr number of segments
* 5) No pointer arguments can be NULL
* 6) No size arguments can be zero
* 7) size_divd should always be greater than or equal to size_dvsr
* 8) Most significant segment of divisor should be non-zero
* 9) Returns MSD of quotient
*/
apn_seg_t apn_basecase_div(
    apn_seg_t* quotient,
    apn_seg_t* dividend,
    const apn_seg_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr
);

// dnc = divide-n-conquer
apn_seg_t apn_dnc_div_balanced(
    apn_seg_t* quotient,
    apn_seg_t* dividend,
    const apn_seg_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr,
    apn_seg_t* temp
);

// divide-n-conquer unbalanced
apn_seg_t apn_dnc_div_unbalanced(
    apn_seg_t* quotient,
    apn_seg_t* dividend,
    const apn_seg_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr,
    apn_seg_t* temp
);

#endif