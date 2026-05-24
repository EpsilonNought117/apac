#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)512ULL)

static void
check_apn_sqr(uint64_t iterations)
{
    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX * 2));
    ap_dig_t* op3 = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX * 2));

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(op3 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX * 2, 0);
    apn_set(op3, TEST_SIZE_MAX * 2, 0);

    while (iterations--)
    {
        ap_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        /* TEST-1: square of zero */

        apn_set(op1, size, 0);

        apn_set(op2, size * 2, 0);
        apn_set(op3, size * 2, 0);

        apac_err err_out = apn_sqr(
            op2,
            op1,
            size
        );

        APAC_ALWAYS_ASSERT(err_out != APAC_OOM);

        int cmp_res = apn_cmp(op2, op3, size * 2);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-2: compare against multiplication */

        apn_set_random(op1, size);

        apn_set(op2, size * 2, 0);
        apn_set(op3, size * 2, 0);

        apac_err err_out1 = apn_sqr(
            op2,
            op1,
            size
        );

        APAC_ALWAYS_ASSERT(err_out1 != APAC_OOM);

        apac_err err_out2 = apn_mul_n(
            op3,
            op1,
            op1,
            size
        );

        APAC_ALWAYS_ASSERT(err_out2 != APAC_OOM);

        cmp_res = apn_cmp(op2, op3, size * 2);

        APAC_ALWAYS_ASSERT(cmp_res == 0);
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

    uint64_t seed = 0xF1A4782DC6309B5EULL;
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

    check_apn_sqr(iterations);

    return EXIT_SUCCESS;
}