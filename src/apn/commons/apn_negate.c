#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

void apn_neg(
    apn_seg* result, 
    const apn_seg* op1, 
    apn_size size
)
{
    APAC_ASSERT(size != 0);
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);

    if (curr_cpu.apn_neg_ptr == NULL)
        apacGetCPUSpec();

    curr_cpu.apn_neg_ptr(result, op1, size);

    return;
}