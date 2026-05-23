#include "../include/apac.h"
#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)7000ULL)

static void
check_apn_submul_one(uint64_t iterations)
{
    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX + 1));
    ap_dig_t* op3 = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX + 1));
    ap_dig_t* op4 = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX + 1));

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(op3 != NULL);
    APAC_ALWAYS_ASSERT(op4 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX + 1, 0);
    apn_set(op3, TEST_SIZE_MAX + 1, 0);
    apn_set(op4, TEST_SIZE_MAX + 1, 0);

    while (iterations--)
    {
        ap_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        /* TEST-1: max * APN_DIG_MAX subtraction */

        apn_set(op1, size, APN_DIG_MAX);

        apn_set(op2, size + 1, 0);
        apn_set(op3, size + 1, 0);

        op3[0] = APN_DIG_MAX;
        op3[size] = 1;

        ap_dig_t borrow = apn_submul_one(
            op2,
            op1,
            size,
            APN_DIG_MAX
        );

        int cmp_res = apn_cmp(op2, op3, size + 1);

        APAC_ALWAYS_ASSERT(borrow == 1);
        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-2: addmul_one + submul_one cancel */

        apn_set_random(op1, size);
        apn_set_random(op3, size + 1);

        apn_cpy(op2, op3, size + 1);

        ap_dig_t val = 0;

        do
        {
            apn_set_random(&val, 1);

        } while (val == 0);

        ap_dig_t carry = apn_addmul_one(
            op2,
            op1,
            size,
            val
        );

        borrow = apn_submul_one(
            op2,
            op1,
            size,
            val
        );

        cmp_res = apn_cmp(op2, op3, size + 1);

        APAC_ALWAYS_ASSERT(carry == borrow);
        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-3: submul_one(..., 1) == subtraction */

        apn_set_random(op1, size);
        apn_set_random(op2, size + 1);

        apn_cpy(op3, op2, size + 1);

        ap_dig_t borrow_ref = apn_sub(
            op3,
            op3,
            op1,
            size + 1,
            size
        );

        borrow = apn_submul_one(
            op2,
            op1,
            size,
            1
        );

        cmp_res = apn_cmp(op2, op3, size + 1);

        APAC_ALWAYS_ASSERT(borrow == borrow_ref);
        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-4: submul_one(..., 0) == no-op */

        apn_set_random(op1, size);
        apn_set_random(op2, size + 1);

        apn_cpy(op3, op2, size + 1);

        borrow = apn_submul_one(
            op2,
            op1,
            size,
            0
        );

        cmp_res = apn_cmp(op2, op3, size + 1);

        APAC_ALWAYS_ASSERT(borrow == 0);
        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-5: monotonic decrease */

        apn_set_random(op1, size);

        do
        {
            apn_set_random(&val, 1);

        } while (val == 0);

        apn_set_random(op2, size + 1);

        apn_cpy(op3, op2, size + 1);

        borrow = apn_submul_one(
            op2,
            op1,
            size,
            val
        );

        int op1_is_zero = (apn_is_zero(op1, size) == 0);

        cmp_res = apn_cmp(op2, op3, size + 1);

        APAC_ALWAYS_ASSERT(
            op1_is_zero ?
            (cmp_res == 0) :
            (borrow ? (cmp_res >= 0) : (cmp_res <= 0))
        );

        /* TEST-6: distributive decomposition */

        ap_dig_t val1 = 0;
        ap_dig_t val2 = 0;

        do
        {
            apn_set_random(&val1, 1);

        } while (val1 == APN_DIG_MAX);

        do
        {
            apn_set_random(&val2, 1);

        } while ((ap_dig_t)(val1 + val2) < val1);

        apn_set_random(op1, size);
        apn_set_random(op2, size + 1);

        apn_cpy(op3, op2, size + 1);
        apn_cpy(op4, op2, size + 1);

        ap_dig_t borrow1 = apn_submul_one(
            op3,
            op1,
            size,
            (ap_dig_t)(val1 + val2)
        );

        ap_dig_t borrow2 = apn_submul_one(
            op4,
            op1,
            size,
            val1
        );

        borrow2 += apn_submul_one(
            op4,
            op1,
            size,
            val2
        );

        cmp_res = apn_cmp(op3, op4, size + 1);

        APAC_ALWAYS_ASSERT(borrow1 == borrow2);
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

    uint64_t seed = 0x6D0F4B2E91C7A538ULL;
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

    check_apn_submul_one(iterations);

    return EXIT_SUCCESS;
}