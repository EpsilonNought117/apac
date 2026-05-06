#include "../headers/apac_internal.h"

/*
    result = op1 + (op2 * (2^bit_cnt))
*/
ap_dig_t apn_lshift_add(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size,
    ap_dig_t bit_cnt
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(op2 != NULL);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(bit_cnt > 0 && bit_cnt < 64);
    APAC_NO_OVERLAP(result, size, op2, size);
    APAC_NO_OVERLAP(op1, size, op2, size);
    APAC_PARTIAL_OVERLAP_ABOVE(result, size, op1, size);
    APAC_ASSERT(curr_cpu.apn_lshift_add_ptr != NULL);

    ap_dig_t out_val = 0;

    out_val = curr_cpu.apn_lshift_add_ptr(result, op1, op2, size, bit_cnt);
    return out_val;
}

/*
    result = op1 - (op2 * (2^bit_cnt))
*/
ap_dig_t apn_lshift_sub(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size,
    ap_dig_t bit_cnt
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(op2 != NULL);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(bit_cnt > 0 && bit_cnt < 64);
    APAC_NO_OVERLAP(result, size, op2, size);
    APAC_NO_OVERLAP(op1, size, op2, size);
    APAC_PARTIAL_OVERLAP_ABOVE(result, size, op1, size);
    APAC_ASSERT(curr_cpu.apn_lshift_add_ptr != NULL);

    ap_dig_t out_val = 0;

    out_val = curr_cpu.apn_lshift_sub_ptr(result, op1, op2, size, bit_cnt);
    return out_val;
}