#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)4096ULL)

static void
check_apn_add(uint64_t iterations)
{
    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op3 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op4 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(op3 != NULL);
    APAC_ALWAYS_ASSERT(op4 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX, 0);
    apn_set(op3, TEST_SIZE_MAX, 0);
    apn_set(op4, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        ap_size_t size1 = 0, size2 = 0;

        do
        {
            apn_set_random(&size1, 1);
            size1 %= TEST_SIZE_MAX;

        } while (size1 == 0);

        do
        {
            apn_set_random(&size2, 1);
            size2 %= TEST_SIZE_MAX;

        } while (size2 == 0);

        ap_size_t large = size1 > size2 ? size1 : size2;
        ap_size_t small = size1 > size2 ? size2 : size1;
        size1 = large;
        size2 = small;

        apn_set_random(op1, size1);
        
        /* TEST-1: a = a + 0 */
        
        apn_set(op2, size2, 0);
        ap_dig_t carry = apn_add(op3, op1, op2, size1, size2);
        int res = apn_cmp(op1, op3, size1);

        APAC_ALWAYS_ASSERT(carry == 0);
        APAC_ALWAYS_ASSERT(res == 0);

        /* TEST-2: a + (-a) = 0 */

        apn_neg(op2, op1, size1);

        carry = apn_add(op3, op1, op2, size1, size1);

        APAC_ALWAYS_ASSERT(carry == 1);
        APAC_ALWAYS_ASSERT(apn_is_zero(op3, size1) == 0);

        /* TEST-3: c = a + b && d = c - b  => d == a */

        apn_set_random(op2, size2);

        carry = apn_add(op3, op1, op2, size1, size2);
        carry -= apn_sub(op4, op3, op2, size1, size2);

        APAC_ALWAYS_ASSERT(carry == 0);
        APAC_ALWAYS_ASSERT(apn_cmp(op1, op4, size1) == 0);

        /* TEST-4: a + a = (2 * a) */

        ap_dig_t carry1 = apn_add(op3, op1, op1, size1, size1);
        ap_dig_t carry2 = apn_lshift(op4, op1, size1, 1);

        APAC_ALWAYS_ASSERT(carry1 == carry2);
        APAC_ALWAYS_ASSERT(apn_cmp(op4, op3, size1) == 0);

        /* TEST-5: max_val + 1 = 0 + carry_out */

        apn_set(op1, size1, APN_DIG_MAX);
        apn_set(op2, size2, 0);
        op2[0] = (ap_dig_t)1;

        carry = apn_add(op3, op1, op2, size1, size2);

        APAC_ALWAYS_ASSERT(carry == 1);
        APAC_ALWAYS_ASSERT(apn_is_zero(op3, size1) == 0);

        /* TEST-6: (a + b) + c = (a + c) + b */

        // c = size2
        apn_set_random(op1, size1);
        apn_set_random(op2, size2);
        apn_set_random(op3, size2);
        apn_set(op4, size1, 0);

        carry1 = apn_add(op4, op1, op2, size1, size2);
        carry1 += apn_add(op4, op4, op3, size1, size2);

        carry2 = apn_add(op1, op1, op3, size1, size2);
        carry2 += apn_add(op1, op1, op2, size1, size2);

        APAC_ALWAYS_ASSERT(carry1 == carry2);
        APAC_ALWAYS_ASSERT(apn_cmp(op1, op4, size1) == 0);
    }

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

    uint64_t seed = 0xA74E2F853D916C3EULL;
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

    check_apn_add(iterations);

    return EXIT_SUCCESS;
}