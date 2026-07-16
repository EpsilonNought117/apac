#include "../include/apac.h"
#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((apn_size_t)512ULL)

static void
check_apn_sub_n(uint64_t iterations)
{
    apn_dig_t* op1 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op2 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op3 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op4 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);

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
        apn_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= (TEST_SIZE_MAX + 1);

        } while (size == 0);

        /* TEST-1: a - 0 == a */

        apn_set_random(op1, size);
        apn_set(op2, size, 0);

        apn_dig_t borrow = apn_sub_n(op3, op1, op2, size);

        int cmp_res = apn_cmp(op3, op1, size);

        APAC_ALWAYS_ASSERT(cmp_res == 0);
        APAC_ALWAYS_ASSERT(borrow == 0);

        /* TEST-2: 0 - 1 == max with borrow */

        apn_set(op1, size, 0);

        apn_set(op2, size, 0);
        op2[0] = 1;

        apn_set(op4, size, APN_DIG_MAX);

        borrow = apn_sub_n(op3, op1, op2, size);

        int cmp_max = apn_cmp(op4, op3, size);

        APAC_ALWAYS_ASSERT(borrow == 1);
        APAC_ALWAYS_ASSERT(cmp_max == 0);

        /* TEST-3: a - a == 0 */

        apn_set_random(op1, size);

        borrow = apn_sub_n(op3, op1, op1, size);

        int is_zero = apn_is_zero(op3, size);

        APAC_ALWAYS_ASSERT(borrow == 0);
        APAC_ALWAYS_ASSERT(is_zero == 1);

        /* TEST-4: borrow matches comparison */

        apn_set_random(op1, size);
        apn_set_random(op2, size);

        borrow = apn_sub_n(op3, op1, op2, size);

        int cmp = apn_cmp(op1, op2, size);

        APAC_ALWAYS_ASSERT(borrow == (cmp < 0));

        /* TEST-5: anti-commutativity modulo borrow */

        apn_set_random(op1, size);
        apn_set_random(op2, size);

        apn_dig_t borrow1 = apn_sub_n(op3, op1, op2, size);
        apn_dig_t borrow2 = apn_sub_n(op4, op2, op1, size);

        int cmp_swap = apn_cmp(op3, op4, size);

        APAC_ALWAYS_ASSERT(
            (borrow1 == borrow2)
            ? (cmp_swap == 0)
            : (cmp_swap != 0)
        );

        /* TEST-6: (a - b) + b == a when a >= b */

        apn_set_random(op1, size);
        apn_set_random(op2, size);

        if (apn_cmp(op1, op2, size) < 0)
        {
            apn_cpy(op4, op1, size);
            apn_cpy(op1, op2, size);
            apn_cpy(op2, op4, size);
        }

        borrow = apn_sub_n(op3, op1, op2, size);

        APAC_ALWAYS_ASSERT(borrow == 0);

        apn_dig_t carry = apn_add_n(op4, op3, op2, size);

        cmp_res = apn_cmp(op4, op1, size);

        APAC_ALWAYS_ASSERT(carry == 0);
        APAC_ALWAYS_ASSERT(cmp_res == 0);
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

    uint64_t seed = 0xB2D47F1C6A093E85ULL;
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

    check_apn_sub_n(iterations);

    return EXIT_SUCCESS;
}