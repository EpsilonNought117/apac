#ifndef APN_HIDDEN_DIV
#define APN_HIDDEN_DIV

#include "../../header/apac_internal.h"

/* ---------------------------- DIVISION-HELPER FUNCTIONS -------------------------------- */

apn_dig_t apn_recip_word_2by1(
    apn_dig_t d
);

apn_dig_t apn_recip_word_3by2(
    apn_dig_t d1,
    apn_dig_t d0
);

apn_dig_t apn_udiv_2by1(
    apn_dig_t u1,
    apn_dig_t u0,
    apn_dig_t d,
    apn_dig_t v,
    apn_dig_t* r
);

apn_dig_t apn_udiv_3by2_quot(
    apn_dig_t u2,
    apn_dig_t u1,
    apn_dig_t u0,
    apn_dig_t d1,
    apn_dig_t d0,
    apn_dig_t v
);

/* ----------------------------- DIVISION-MAIN FUNCTIONS --------------------------------- */

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
apn_dig_t apn_basecase_div(
    apn_dig_t* quotient,
    apn_dig_t* dividend,
    const apn_dig_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr
);

// dnc = divide-n-conquer
apn_dig_t apn_dnc_div_balanced(
    apn_dig_t* quotient,
    apn_dig_t* dividend,
    const apn_dig_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr,
    apn_dig_t* temp
);

// divide-n-conquer unbalanced
apn_dig_t apn_dnc_div_unbalanced(
    apn_dig_t* quotient,
    apn_dig_t* dividend,
    const apn_dig_t* divisor,
    apn_size_t size_divd,
    apn_size_t size_dvsr,
    apn_dig_t* temp
);

#endif