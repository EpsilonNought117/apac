#include "apac.h"
#include "apac_internal.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MIN_SIZE (ap_size_t)100
#define MAX_SIZE (ap_size_t)250

ap_dig_t op1[MAX_SIZE];
ap_dig_t result[MAX_SIZE * 2];
ap_dig_t result_cpy[MAX_SIZE * 2];

int main(void)
{
    apac_init();

    apn_set(op1, MAX_SIZE, APN_DIG_MAX);
    apn_set(result_cpy, MAX_SIZE * 2, 0);

    for (ap_size_t i = MIN_SIZE; i <= MAX_SIZE; i++)
    {
        apn_sqr(result, op1, i);

        result_cpy[0] = (ap_dig_t)1;
        apn_set(&result_cpy[1], i - 1, 0);
        
        result_cpy[i] = (ap_dig_t)APN_DIG_MAX - 1;
        apn_set(&result[i + 1], i - 1, APN_DIG_MAX);

        APAC_ALWAYS_ASSERT(apn_cmp(result, result_cpy, i * 2) == 0);
    }

    return EXIT_SUCCESS;
}