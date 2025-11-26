#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

apn_seg_t apn_rshift(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t bit_cnt
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(bit_cnt != 0);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(size * 64 >= bit_cnt);
    APAC_ASSERT(curr_cpu.apn_rshift_lt64_ptr != NULL);

    // if op1 and result overlap, result ptr must point to 
    // a lower or same segment in op1 than op1 ptr itself
    // or result ptr must point to a totally different 
    // location beyond op1 bignum
    APAC_ASSERT((result <= op1) || (result >= (op1 + size * 8)));

    apn_size_t bit_cnt_full = bit_cnt >> 6; // bit_cnt /= 64
    apn_seg_t bit_cnt_frac = bit_cnt & 63;  // bit_cnt %= 64
    apn_seg_t out_val = 0;

    out_val = curr_cpu.apn_rshift_lt64_ptr(result, op1, size, bit_cnt_frac);
    
    apn_set(result + size - bit_cnt_full, bit_cnt_full, 0ULL);

    return out_val;
}

apn_seg_t apn_lshift(
    apn_seg_t* result,
    const apn_seg_t* op1,
    apn_size_t size,
    apn_seg_t bit_cnt
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(bit_cnt != 0);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(size * 64 >= bit_cnt);
    APAC_ASSERT(curr_cpu.apn_lshift_lt64_ptr != NULL);

    // if op1 and result overlap, result ptr must point to 
    // a higher or same segment in op1 than op1 ptr itself
    // or result ptr must point to a totally different
    // location below op1 bignum
    APAC_ASSERT((result >= op1) || (result <= (op1 - size * 8)));

    apn_size_t bit_cnt_full = bit_cnt >> 6; // bit_cnt /= 64
    apn_seg_t bit_cnt_frac = bit_cnt & 63;  // bit_cnt %= 64
    apn_seg_t out_val = 0;

    out_val = curr_cpu.apn_lshift_lt64_ptr(result + bit_cnt_full, op1, size, bit_cnt_frac);

    apn_set(result, bit_cnt_full, 0ULL);

    return out_val;
}