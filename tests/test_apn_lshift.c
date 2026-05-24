#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)512ULL)

static void
check_apn_lshift(uint64_t iterations)
{
    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX + 1));
    ap_dig_t* op3 = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX + 1));

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(op3 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX + 1, 0);
    apn_set(op3, TEST_SIZE_MAX + 1, 0);

    while (iterations--)
    {
        ap_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        /* TEST-1: zero shift input */

        apn_set(op1, size, 0);

        for (uint32_t sh = 1; sh <= APN_DIG_BITS - 1; sh++)
        {
            apn_set(op2, size + 1, 0);

            ap_dig_t carry = apn_lshift(
                op2,
                op1,
                size,
                sh
            );

            int cmp_res = apn_cmp(op2, op1, size);

            APAC_ALWAYS_ASSERT(carry == 0);
            APAC_ALWAYS_ASSERT(cmp_res == 0);
        }

        /* TEST-2: carry extraction */

        for (uint32_t sh = 1; sh <= APN_DIG_BITS - 1; sh++)
        {
            apn_set_random(op1, size);

            apn_set(op2, size + 1, 0);

            ap_dig_t carry = apn_lshift(
                op2,
                op1,
                size,
                sh
            );

            ap_dig_t expected_carry = (op1[size - 1] >> (APN_DIG_BITS - sh));

            APAC_ALWAYS_ASSERT(carry == expected_carry);
        }

        /* TEST-3: compare against addmul_one */

        for (uint32_t sh = 1; sh <= APN_DIG_BITS - 1; sh++)
        {
            apn_set_random(op1, size);

            apn_set(op2, size + 1, 0);
            apn_set(op3, size + 1, 0);

            op2[size] = apn_lshift(
                op2,
                op1,
                size,
                sh
            );

            apn_addmul_one(
                op3,
                op1,
                size,
                ((ap_dig_t)1 << sh)
            );

            int cmp_res = apn_cmp(op2, op3, size + 1);
            APAC_ALWAYS_ASSERT(cmp_res == 0);
        }
    }

    apac_free(op3);
    apac_free(op2);
    apac_free(op1);
}

int
main(void)
{
    apac_init();

    char* prng_seed = getenv("PRNG_SEED");
    char* iter = getenv("ITERATIONS");

    uint64_t seed = 0xC3F82A7D510E4B96ULL;
    uint64_t iterations = 1024ULL;

    if (prng_seed != NULL)
    {
        seed = (uint64_t)strtoull(prng_seed, NULL, 0);
    }

    if (iter != NULL)
    {
        iterations = (uint64_t)strtoull(iter, NULL, 0);
    }

    apn_seed_prng(seed);

    check_apn_lshift(iterations);

    return EXIT_SUCCESS;
}