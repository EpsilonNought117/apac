#include "../../../../include/apac.h"

extern apac_cpu_params curr_cpu;

ap_seg_t apn_rshift(
    ap_seg_t* result,
    const ap_seg_t* op1,
    ap_size_t size,
    ap_seg_t bit_cnt
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(bit_cnt > 0 && bit_cnt < (APN_SEG_BITS));
    APAC_PARTIAL_OVERLAP_ABOVE(result, size, op1, size);
    APAC_ASSERT(curr_cpu.apn_rshift_lt64_ptr != NULL);

    ap_seg_t out_val = 0;

    out_val = curr_cpu.apn_rshift_lt64_ptr(result, op1, size, bit_cnt);

    return out_val;
}

ap_seg_t apn_lshift(
    ap_seg_t* result,
    const ap_seg_t* op1,
    ap_size_t size,
    ap_seg_t bit_cnt
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(bit_cnt > 0 && bit_cnt < (APN_SEG_BITS));
    APAC_PARTIAL_OVERLAP_BELOW(result, size, op1, size);
    APAC_ASSERT(curr_cpu.apn_lshift_lt64_ptr != NULL);

    ap_seg_t out_val = 0;

    out_val = curr_cpu.apn_lshift_lt64_ptr(result, op1, size, bit_cnt);

    return out_val;
}