#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((apn_size_t)512ULL)

static void
check_apn_add_n(uint64_t iterations)
{
    apn_dig_t* op1 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op2 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op3 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op4 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op5 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(op3 != NULL);
    APAC_ALWAYS_ASSERT(op4 != NULL);
    APAC_ALWAYS_ASSERT(op5 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX, 0);
    apn_set(op3, TEST_SIZE_MAX, 0);
    apn_set(op4, TEST_SIZE_MAX, 0);
    apn_set(op5, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        apn_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        /* TEST-1: a + 0 = a */

        apn_set_random(op1, size);
        apn_set(op2, size, 0);

        apn_dig_t carry = apn_add_n(op3, op1, op2, size);

        int cmp_res = apn_cmp(op3, op1, size);

        APAC_ALWAYS_ASSERT(cmp_res == 0);
        APAC_ALWAYS_ASSERT(carry == 0);

        /* TEST-2: max + max */

        apn_set(op1, size, APN_DIG_MAX);
        apn_set(op2, size, APN_DIG_MAX);

        apn_set(op3, size, APN_DIG_MAX);

        op3[0] = APN_DIG_MAX - 1;

        carry = apn_add_n(op4, op1, op2, size);

        cmp_res = apn_cmp(op4, op3, size);

        APAC_ALWAYS_ASSERT(carry == 1);
        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-3: monotonicity */

        apn_set_random(op1, size);
        apn_set_random(op2, size);

        carry = apn_add_n(op3, op1, op2, size);

        int cmp3_1 = apn_cmp(op3, op1, size);
        int cmp3_2 = apn_cmp(op3, op2, size);

        int monotonic_ok =
            (carry == 0)
            ? (cmp3_1 >= 0 && cmp3_2 >= 0)
            : (cmp3_1 < 0 && cmp3_2 < 0);

        APAC_ALWAYS_ASSERT(monotonic_ok == 1);

        /* TEST-4: commutativity */

        apn_set_random(op1, size);
        apn_set_random(op2, size);

        apn_add_n(op3, op1, op2, size);
        apn_add_n(op4, op2, op1, size);

        int cmp = apn_cmp(op3, op4, size);

        APAC_ALWAYS_ASSERT(cmp == 0);

        /* TEST-5: associativity */

        apn_set_random(op1, size);
        apn_set_random(op2, size);
        apn_set_random(op3, size);

        apn_dig_t carry0 = 0;
        apn_dig_t carry1 = 0;

        carry0 += apn_add_n(op4, op1, op2, size);
        carry0 += apn_add_n(op4, op4, op3, size);

        carry1 += apn_add_n(op5, op2, op3, size);
        carry1 += apn_add_n(op5, op5, op1, size);

        cmp = apn_cmp(op4, op5, size);

        APAC_ALWAYS_ASSERT(cmp == 0);
        APAC_ALWAYS_ASSERT(carry0 == carry1);
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

    uint64_t seed = 0xC17A5E29D4B86F3CULL;
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

    check_apn_add_n(iterations);

    return EXIT_SUCCESS;
}