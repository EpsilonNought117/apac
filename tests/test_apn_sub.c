#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((apn_size_t)512ULL)

static void
check_apn_sub(uint64_t iterations)
{
    apn_dig_t* a = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* b = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* r = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* t = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(a != NULL);
    APAC_ALWAYS_ASSERT(b != NULL);
    APAC_ALWAYS_ASSERT(r != NULL);
    APAC_ALWAYS_ASSERT(t != NULL);

    apn_set(a, TEST_SIZE_MAX, 0);
    apn_set(b, TEST_SIZE_MAX, 0);
    apn_set(r, TEST_SIZE_MAX, 0);
    apn_set(t, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        apn_size_t size1 = 0;
        apn_size_t size2 = 0;

        do
        {
            apn_set_random(&size1, 1);
            size1 %= (TEST_SIZE_MAX + 1);

        } while (size1 == 0);

        do
        {
            apn_set_random(&size2, 1);
            size2 %= (TEST_SIZE_MAX + 1);

        } while (size2 == 0);

        if (size2 > size1)
        {
            apn_size_t temp = size1;
            size1 = size2;
            size2 = temp;
        }

        /* TEST-1: a - 0 == a */

        apn_set_random(a, size1);
        apn_set(b, size2, 0);

        apn_dig_t borrow = apn_sub(r, a, b, size1, size2);

        int cmp = apn_cmp(r, a, size1);

        APAC_ALWAYS_ASSERT(borrow == 0);
        APAC_ALWAYS_ASSERT(cmp == 0);

        /* TEST-2: borrow matches comparison */

        apn_set_random(a, size1);

        apn_set(b, size1, 0);
        apn_set_random(b, size2);

        borrow = apn_sub(r, a, b, size1, size2);

        cmp = apn_cmp(a, b, size1);

        APAC_ALWAYS_ASSERT(borrow == (cmp < 0));

        /* TEST-3: 0 - 1 == max with borrow */

        apn_set(a, size1, 0);

        apn_set(b, size1, 0);
        b[0] = 1;

        apn_set(t, size1, APN_DIG_MAX);

        borrow = apn_sub(r, a, b, size1, 1);

        cmp = apn_cmp(r, t, size1);

        APAC_ALWAYS_ASSERT(borrow == 1);
        APAC_ALWAYS_ASSERT(cmp == 0);

        /* TEST-4: a - a == 0 */

        apn_set_random(a, size1);

        borrow = apn_sub(r, a, a, size1, size1);

        int is_zero = apn_is_zero(r, size1);

        APAC_ALWAYS_ASSERT(borrow == 0);
        APAC_ALWAYS_ASSERT(is_zero == 1);
    }

    apac_free(t);
    apac_free(r);
    apac_free(b);
    apac_free(a);
}

int
main(void)
{
    apac_init();

    char* prng_seed = getenv("PRNG_SEED");
    char* iter = getenv("ITERATIONS");

    uint64_t seed = 0x5C8E2A4F7D1B9063ULL;
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

    check_apn_sub(iterations);

    return EXIT_SUCCESS;
}