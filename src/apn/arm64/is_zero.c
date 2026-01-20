#include "../../../include/apac.h"

int is_zero_asimd_4unroll(
    const apn_seg_t* op1,
    apn_size_t size
)
{
    apn_size_t blocks = size - (size % 8);
    apn_size_t counter = 0;
    
    uint64x2_t zero = vdupq_n_u64(0);

    while (counter < blocks)
    {
        // Load 8 values
        uint64x2_t v0 = vld1q_u64(&op1[counter]);
        uint64x2_t v1 = vld1q_u64(&op1[counter + 2]);
        uint64x2_t v2 = vld1q_u64(&op1[counter + 4]);
        uint64x2_t v3 = vld1q_u64(&op1[counter + 6]);

        // OR all vectors together
        v0 = vorrq_u64(v0, v1);
        v2 = vorrq_u64(v2, v3);
        v0 = vorrq_u64(v0, v2);

        // Check if any non-zero: use horizontal OR reduction
        uint64x1_t reduced = vorr_u64(vget_low_u64(v0), vget_high_u64(v0));
        uint64_t result = vget_lane_u64(reduced, 0);
        
        if (result != 0)
            return 1;

        counter += 8;
    }

    // Handle remaining elements
    for (; counter < size; counter++)
    {
        if (op1[counter] != 0ULL)
            return 1;
    }

    return 0;
}