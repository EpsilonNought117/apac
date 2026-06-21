#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((apn_size_t)512ULL)

static void
check_apn_mul_n(uint64_t iterations)
{
    apn_dig_t* op1 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op2 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op3 = apac_malloc(sizeof(apn_dig_t) * (TEST_SIZE_MAX * 2));
    apn_dig_t* op4 = apac_malloc(sizeof(apn_dig_t) * (TEST_SIZE_MAX * 2));
    apn_dig_t* op5 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(op3 != NULL);
    APAC_ALWAYS_ASSERT(op4 != NULL);
    APAC_ALWAYS_ASSERT(op5 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX, 0);
    apn_set(op3, TEST_SIZE_MAX * 2, 0);
    apn_set(op4, TEST_SIZE_MAX * 2, 0);
    apn_set(op5, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        apn_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        /* TEST-1: compare against schoolbook addmul_one */

        apn_set_random(op1, size);
        apn_set_random(op2, size);

        apn_set(op3, size * 2, 0);
        apn_set(op4, size * 2, 0);

        apac_err err_out = apn_mul_n(
            op3,
            op1,
            op2,
            size
        );

        APAC_ALWAYS_ASSERT(err_out != APAC_OOM);

        for (apn_size_t k = 0; k < size; k++)
        {
            apn_addmul_one(
                &op4[k],
                op1,
                size,
                op2[k]
            );
        }

        int cmp_res = apn_cmp(op3, op4, size * 2);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-2: max * max */

        apn_set(op1, size, APN_DIG_MAX);
        apn_set(op2, size, APN_DIG_MAX);

        apn_set(op4, size * 2, 0);

        apn_set(op3, size, 0);
        op3[0] = 1;

        apn_set(op3 + size, size, APN_DIG_MAX);
        op3[size] = APN_DIG_MAX - 1;

        err_out = apn_mul_n(
            op4,
            op1,
            op2,
            size
        );

        APAC_ALWAYS_ASSERT(err_out != APAC_OOM);

        cmp_res = apn_cmp(op3, op4, size * 2);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-3: multiply by one */

        apn_set(op2, size, 0);
        op2[0] = 1;

        apn_set_random(op1, size);
        apn_set(op3, size * 2, 0);

        err_out = apn_mul_n(
            op3,
            op1,
            op2,
            size
        );

        APAC_ALWAYS_ASSERT(err_out != APAC_OOM);

        cmp_res = apn_cmp(op3, op1, size);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-4: multiply by zero */

        apn_set(op2, size, 0);

        apn_set_random(op1, size);
        apn_set(op3, size * 2, 0);

        err_out = apn_mul_n(
            op3,
            op1,
            op2,
            size
        );

        APAC_ALWAYS_ASSERT(err_out != APAC_OOM);

        int is_zero = apn_is_zero(op3, size * 2);

        APAC_ALWAYS_ASSERT(is_zero == 1);

        /* TEST-5: commutativity */

        apn_set_random(op1, size);
        apn_set_random(op2, size);

        apn_set(op3, size * 2, 0);
        apn_set(op4, size * 2, 0);

        apac_err err_out1 = apn_mul_n(
            op3,
            op1,
            op2,
            size
        );

        APAC_ALWAYS_ASSERT(err_out1 != APAC_OOM);

        apac_err err_out2 = apn_mul_n(
            op4,
            op2,
            op1,
            size
        );

        APAC_ALWAYS_ASSERT(err_out2 != APAC_OOM);

        cmp_res = apn_cmp(op3, op4, size * 2);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-6: monotonicity */

        apn_set_random(op1, size);
        apn_set_random(op2, size);
        apn_set_random(op5, size);

        apn_set(op3, size * 2, 0);
        apn_set(op4, size * 2, 0);

        int cmp_res1 = apn_cmp(op5, op2, size);

        err_out1 = apn_mul_n(
            op3,
            op2,
            op1,
            size
        );

        APAC_ALWAYS_ASSERT(err_out1 != APAC_OOM);

        err_out2 = apn_mul_n(
            op4,
            op5,
            op1,
            size
        );

        APAC_ALWAYS_ASSERT(err_out2 != APAC_OOM);

        int cmp_res2 = apn_cmp(op4, op3, size * 2);

        APAC_ALWAYS_ASSERT(cmp_res1 == cmp_res2);
    }

    apac_free(op5);
    apac_free(op4);
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

    uint64_t seed = 0x89D5C3F6042B7E1AULL;
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

    check_apn_mul_n(iterations);

    return EXIT_SUCCESS;
}