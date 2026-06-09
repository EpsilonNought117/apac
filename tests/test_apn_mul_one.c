#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)512ULL)

static void
check_apn_mul_one(uint64_t iterations)
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

        /* TEST-1: max * APN_DIG_MAX */

        apn_set(op1, size, APN_DIG_MAX);

        apn_set(op2, size + 1, 0);
        apn_set(op3, size + 1, 0);

        apn_addmul_one(op3, op1, size, APN_DIG_MAX);

        apn_mul_one(op2, op1, size, APN_DIG_MAX);

        int cmp_res = apn_cmp(op2, op3, size + 1);
        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-2: mul_one(..., 1) == copy */

        apn_set_random(op1, size);

        apn_set(op2, size + 1, 0);
        apn_set(op3, size + 1, 0);

        apn_addmul_one(op3, op1, size, 1);

        apn_mul_one(op2, op1, size, 1);

        cmp_res = apn_cmp(op2, op3, size + 1);
        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-3: mul_one(..., 0) == zero */

        apn_set_random(op1, size);

        apn_set(op2, size + 1, 0);
        apn_set(op3, size + 1, 0);

        apn_addmul_one(op3, op1, size, 0);

        apn_mul_one(op2, op1, size, 0);

        cmp_res = apn_cmp(op2, op3, size + 1);
        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-4: random op1, random val */

        apn_set_random(op1, size);

        ap_dig_t val = 0;
        apn_set_random(&val, 1);

        apn_set(op2, size + 1, 0);
        apn_set(op3, size + 1, 0);

        apn_addmul_one(op3, op1, size, val);

        apn_mul_one(op2, op1, size, val);

        cmp_res = apn_cmp(op2, op3, size + 1);
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

    uint64_t seed = 0xA74E2F853D916C0BULL;
    uint64_t iterations = 1024ULL;

    if (prng_seed != NULL)
        seed = (uint64_t)strtoull(prng_seed, NULL, 0);

    if (iter != NULL)
        iterations = (uint64_t)strtoull(iter, NULL, 0);

    apn_seed_prng(seed);

    check_apn_mul_one(iterations);

    return EXIT_SUCCESS;
}