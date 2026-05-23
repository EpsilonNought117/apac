#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)7000ULL)

static void
check_apn_add_one(uint64_t iterations)
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

        /* TEST-1: max + 1 = 0 + carry */

        apn_set(op1, size, APN_DIG_MAX);

        ap_dig_t carry_out = apn_add_one(op2, op1, size, 1);

        APAC_ALWAYS_ASSERT(carry_out == 1);

        int is_zero = apn_is_zero(op2, size);

        APAC_ALWAYS_ASSERT(is_zero == 0);

        /* TEST-2: add_one(..., 0) == identity */

        apn_set_random(op1, size);

        carry_out = apn_add_one(op2, op1, size, 0);

        APAC_ALWAYS_ASSERT(carry_out == 0);

        int is_equal = apn_cmp(op1, op2, size);

        APAC_ALWAYS_ASSERT(is_equal == 0);

        /* TEST-3: monotonicity */

        apn_set_random(op1, size);

        ap_dig_t val = 0;

        do
        {
            apn_set_random(&val, 1);

        } while (val == 0);

        carry_out = apn_add_one(op2, op1, size, val);

        int cmp_res = apn_cmp(op2, op1, size);

        APAC_ALWAYS_ASSERT(cmp_res == (carry_out ? -1 : 1));

        /* TEST-4: add_one(a, b) == add_n(a, b_as_vec) */

        apn_set_random(op1, size);

        do
        {
            apn_set_random(&val, 1);

        } while (val == 0);

        apn_set(op3, size, 0);
        op3[0] = val;

        ap_dig_t carry_ref = apn_add_n(op2, op1, op3, size);

        carry_out = apn_add_one(op3, op1, size, val);

        APAC_ALWAYS_ASSERT(carry_out == carry_ref);

        cmp_res = apn_cmp(op2, op3, size);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-5: associativity */

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

        ap_dig_t carry0 = apn_add_one(op2, op1, size, (val1 + val2));
        ap_dig_t carry1 = apn_add_one(op3, op1, size, val1);
        carry1 += apn_add_one(op3, op3, size, val2);

        APAC_ALWAYS_ASSERT(carry0 == carry1);

        cmp_res = apn_cmp(op2, op3, size);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-6: aliasing */

        apn_set_random(op1, size);

        do
        {
            apn_set_random(&val, 1);

        } while (val == 0);

        apn_cpy(op2, op1, size);

        ap_dig_t carry_alias = apn_add_one(op1, op1, size, val);

        apn_set(op3, size, 0);
        op3[0] = val;

        ap_dig_t carry_ref_alias = apn_add_n(op3, op2, op3, size);

        APAC_ALWAYS_ASSERT(carry_alias == carry_ref_alias);

        cmp_res = apn_cmp(op1, op3, size);

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

    uint64_t seed = 0x7B3E91A4C6D82F5EULL;
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

    check_apn_add_one(iterations);

    return EXIT_SUCCESS;
}