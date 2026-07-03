#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((apn_size_t)512ULL)

static void
check_apn_rshift(uint64_t iterations)
{
    apn_dig_t* op1 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op2 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op3 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(op3 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX, 0);
    apn_set(op3, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        apn_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        /* TEST-1: shift-out correctness */

        apn_set_random(op1, size);

        for (uint32_t sh = 1; sh <= APN_DIG_BITS - 1; sh++)
        {
            apn_cpy(op3, op1, size);

            apn_set(op2, size, 0);

            apn_dig_t shift_out = apn_rshift(
                op2,
                op1,
                size,
                sh
            );

            apn_dig_t expected_shift_out =
                op3[0] << (APN_DIG_BITS - sh);

            APAC_ALWAYS_ASSERT(
                shift_out == expected_shift_out
            );
        }

        /* TEST-2: shifting zero */

        apn_set(op1, size, 0);

        for (uint32_t sh = 1; sh <= APN_DIG_BITS - 1; sh++)
        {
            apn_set(op2, size, 0);

            apn_dig_t shift_out = apn_rshift(
                op2,
                op1,
                size,
                sh
            );

            int cmp_res = apn_cmp(op2, op1, size);

            APAC_ALWAYS_ASSERT(shift_out == 0);

            APAC_ALWAYS_ASSERT(cmp_res == 0);
        }

        /* TEST-3: reconstruct via lshift */

        apn_set_random(op1, size);

        for (uint32_t sh = 1; sh <= APN_DIG_BITS - 1; sh++)
        {
            apn_cpy(op3, op1, size);

            apn_set(op2, size, 0);

            apn_dig_t shift_out = apn_rshift(
                op2,
                op1,
                size,
                sh
            );

            apn_lshift(
                op2,
                op2,
                size,
                sh
            );

            op2[0] |=
                (shift_out >> (APN_DIG_BITS - sh));

            int cmp_res = apn_cmp(op2, op3, size);

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

    uint64_t seed = 0x47B0D6F2E8351A9CULL;
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

    check_apn_rshift(iterations);

    return EXIT_SUCCESS;
}