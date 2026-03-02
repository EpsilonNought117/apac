#include "../../../../include/apac.h"

extern apac_cpu_params curr_cpu;

void apn_neg(
    ap_dig_t* result, 
    const ap_dig_t* op1, 
    ap_size_t size
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(size != 0);
    APAC_ASSERT(curr_cpu.apn_neg_ptr != NULL);

    curr_cpu.apn_neg_ptr(result, op1, size);

    return;
}