#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

int apn_is_equal(
    const apn_seg_t* op1,
    const apn_seg_t* op2,
    apn_size_t size
)
{
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(op2 != NULL);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(curr_cpu.apn_is_equal_ptr != NULL);

    int val = curr_cpu.apn_is_equal_ptr(op1, op2, size);
    return val;
}