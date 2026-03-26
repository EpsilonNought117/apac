#ifndef APN_HIDDEN_DIV
#define APN_HIDDEN_DIV

#include "../../../../include/apac.h"

/* ---------------------------- DIVISION-HELPER FUNCTIONS -------------------------------- */

ap_dig_t apn_recip_word_2by1(
    ap_dig_t d
);

ap_dig_t apn_recip_word_3by2(
    ap_dig_t d1,
    ap_dig_t d0
);

ap_dig_t apn_udiv_2by1(
    ap_dig_t u1,
    ap_dig_t u0,
    ap_dig_t d,
    ap_dig_t v,
    ap_dig_t* r
);

ap_dig_t apn_udiv_3by2_quot(
    ap_dig_t u2,
    ap_dig_t u1,
    ap_dig_t u0,
    ap_dig_t d1,
    ap_dig_t d0,
    ap_dig_t v
);

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
ap_dig_t apn_basecase_div(
    ap_dig_t* quotient,
    ap_dig_t* dividend,
    const ap_dig_t* divisor,
    ap_size_t size_divd,
    ap_size_t size_dvsr
);

// dnc = divide-n-conquer
ap_dig_t apn_dnc_div_balanced(
    ap_dig_t* quotient,
    ap_dig_t* dividend,
    const ap_dig_t* divisor,
    ap_size_t size_divd,
    ap_size_t size_dvsr,
    ap_dig_t* temp
);

// divide-n-conquer unbalanced
ap_dig_t apn_dnc_div_unbalanced(
    ap_dig_t* quotient,
    ap_dig_t* dividend,
    const ap_dig_t* divisor,
    ap_size_t size_divd,
    ap_size_t size_dvsr,
    ap_dig_t* temp
);

#endif