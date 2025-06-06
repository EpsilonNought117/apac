#include "../../../../include/apac.h"

extern __apac_cpu_params curr_cpu;

void apn_neg(u64* result, const u64* op1, u64 size)
{
    APAC_ASSERT(size != 0);
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);

    if (curr_cpu.__apn_neg_ptr == NULL)
        apacGetCPUSpec();

    curr_cpu.__apn_neg_ptr(result, op1, size);

    return;
}