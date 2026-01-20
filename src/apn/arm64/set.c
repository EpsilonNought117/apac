#include "../../../include/apac.h"

void set_asimd_4unroll(
    apn_seg_t* result,
    apn_size_t size,
    apn_seg_t val
)
{
    uint64x2_t my_val = vdupq_n_u64(val);

    apn_size_t blocks = size - (size % 8);
    apn_size_t counter = 0;

    while (counter < blocks)
    {
        // Store 8 values (4x uint64x2_t vectors)
        vst1q_u64(&result[counter], my_val);
        vst1q_u64(&result[counter + 2], my_val);
        vst1q_u64(&result[counter + 4], my_val);
        vst1q_u64(&result[counter + 6], my_val);

        counter += 8;
    }

    // Handle remaining elements
    while (counter < size)
    {
        result[counter] = val;
        counter++;
    }
}