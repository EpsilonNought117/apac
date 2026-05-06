#include "../headers/apac_internal.h"

ap_dig_t apn_rshift(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size,
    ap_dig_t bit_cnt
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(bit_cnt > 0 && bit_cnt < (APN_DIG_BITS));
    APAC_PARTIAL_OVERLAP_BELOW(result, size, op1, size);
    APAC_ASSERT(curr_cpu.apn_rshift_ptr != NULL);

    ap_dig_t out_val = 0;

    out_val = curr_cpu.apn_rshift_ptr(result, op1, size, bit_cnt);

    return out_val;
}

ap_dig_t apn_lshift(
    ap_dig_t* result,
    const ap_dig_t* op1,
    ap_size_t size,
    ap_dig_t bit_cnt
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(bit_cnt > 0 && bit_cnt < (APN_DIG_BITS));
    APAC_PARTIAL_OVERLAP_ABOVE(result, size, op1, size);
    APAC_ASSERT(curr_cpu.apn_lshift_ptr != NULL);

    ap_dig_t out_val = 0;

    out_val = curr_cpu.apn_lshift_ptr(result, op1, size, bit_cnt);

    return out_val;
}