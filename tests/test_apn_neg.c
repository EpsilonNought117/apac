#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)512ULL)

static void
check_apn_neg(uint64_t iterations)
{
    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op3 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(op3 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX, 0);
    apn_set(op3, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        ap_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        /* TEST-1: a + (-a) = 0 */

        apn_set_random(op1, size);

        apn_neg(op2, op1, size);

        ap_dig_t carry = apn_add_n(
            op3,
            op1,
            op2,
            size
        );

        APAC_ALWAYS_ASSERT(carry == 1);

        int is_zero = apn_is_zero(op3, size);

        APAC_ALWAYS_ASSERT(is_zero == 1);

        /* TEST-2: neg(neg(a)) = a */

        apn_neg(op2, op1, size);
        apn_neg(op3, op2, size);

        int cmp_res = apn_cmp(op1, op3, size);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-3: neg(0) = 0 */

        apn_set(op1, size, 0);

        apn_neg(op2, op1, size);

        is_zero = apn_is_zero(op2, size);

        APAC_ALWAYS_ASSERT(is_zero == 1);
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

    uint64_t seed = 0x8A72C41D5E9B37F6ULL;
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

    check_apn_neg(iterations);

    return EXIT_SUCCESS;
}