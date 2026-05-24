#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)512ULL)

static void
check_apn_div_one(uint64_t iterations)
{
    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX + 1));
    ap_dig_t* q1 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* q2 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(q1 != NULL);
    APAC_ALWAYS_ASSERT(q2 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX + 1, 0);
    apn_set(q1, TEST_SIZE_MAX, 0);
    apn_set(q2, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        ap_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        /* TEST-1: division by powers of two */

        apn_set_random(op1, size);

        for (uint32_t sh = 1; sh <= 63; sh++)
        {
            apn_set(q1, size, 0);
            apn_set(q2, size, 0);

            ap_dig_t shift_out = apn_rshift(q2, op1, size, sh);

            ap_dig_t divisor = ((ap_dig_t)1 << sh);

            ap_dig_t rem = apn_div_one(
                q1,
                op1,
                divisor,
                size,
                0
            );

            int cmp_q = apn_cmp(q1, q2, size);

            ap_dig_t expected_rem =
                shift_out >> (APN_DIG_BITS - sh);

            APAC_ALWAYS_ASSERT(cmp_q == 0);
            APAC_ALWAYS_ASSERT(rem == expected_rem);
        }

        /* TEST-2: reconstruction identity */

        apn_set_random(op1, size);

        ap_dig_t divisor = 0;

        do
        {
            apn_set_random(&divisor, 1);

        } while (divisor == 0);

        apn_set(q1, size, 0);
        apn_set(op2, size + 1, 0);

        ap_dig_t rem = apn_div_one(
            q1,
            op1,
            divisor,
            size,
            0
        );

        op2[0] = rem;

        ap_dig_t carry = apn_addmul_one(
                            op2,
                            q1,
                            size,
                            divisor
                        );

        int cmp_res = apn_cmp(op1, op2, size);

        APAC_ALWAYS_ASSERT(carry == 0);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-3: division by one */

        apn_set_random(op1, size);

        apn_set(q1, size, 0);

        rem = apn_div_one(
            q1,
            op1,
            1,
            size,
            0
        );

        int cmp_identity = apn_cmp(op1, q1, size);

        APAC_ALWAYS_ASSERT(rem == 0);
        APAC_ALWAYS_ASSERT(cmp_identity == 0);
    }

    apac_free(q2);
    apac_free(q1);
    apac_free(op2);
    apac_free(op1);
}

int
main(void)
{
    apac_init();

    char* prng_seed = getenv("PRNG_SEED");
    char* iter = getenv("ITERATIONS");

    uint64_t seed = 0xD81F7C5A91E3B647ULL;
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

    check_apn_div_one(iterations);

    return EXIT_SUCCESS;
}