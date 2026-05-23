#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)7000ULL)

static void
check_apn_div(uint64_t iterations)
{
    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX * 2));
    ap_dig_t* quot = apac_malloc(sizeof(ap_dig_t) * (TEST_SIZE_MAX + 1));
    ap_dig_t* rmdr = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* temp = apac_malloc(sizeof(ap_dig_t) * ((TEST_SIZE_MAX * 2) + 1));

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(quot != NULL);
    APAC_ALWAYS_ASSERT(rmdr != NULL);
    APAC_ALWAYS_ASSERT(temp != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX * 2, 0);
    apn_set(quot, TEST_SIZE_MAX + 1, 0);
    apn_set(rmdr, TEST_SIZE_MAX, 0);
    apn_set(temp, TEST_SIZE_MAX * 2 + 1, 0);

    while (iterations--)
    {
        ap_size_t div_size = 0;
        ap_size_t quot_size = 0;

        do
        {
            apn_set_random(&div_size, 1);
            div_size %= TEST_SIZE_MAX;

        } while (div_size == 0);

        do
        {
            apn_set_random(&quot_size, 1);
            quot_size %= TEST_SIZE_MAX;

        } while (quot_size == 0);

        ap_size_t num_size = div_size + quot_size;

        if (num_size > (TEST_SIZE_MAX * 2))
            continue;

        do
        {
            apn_set_random(op1, div_size);

        } while (op1[div_size - 1] == 0);

        apn_set_random(op2, num_size);

        apn_set(quot, quot_size + 1, 0);
        apn_set(rmdr, div_size, 0);
        apn_set(temp, num_size, 0);

        apac_err ret = apn_div(
            quot,
            rmdr,
            op2,
            op1,
            num_size,
            0,
            div_size
        );

        APAC_ALWAYS_ASSERT(ret != APAC_OOM);

        ap_dig_t* mul_a;
        ap_dig_t* mul_b;

        ap_size_t mul_asz;
        ap_size_t mul_bsz;

        if ((quot_size + 1) >= div_size)
        {
            mul_a = quot;
            mul_b = op1;

            mul_asz = quot_size + 1;
            mul_bsz = div_size;
        }
        else
        {
            mul_a = op1;
            mul_b = quot;

            mul_asz = div_size;
            mul_bsz = quot_size + 1;
        }

        ret = apn_mul(
            temp,
            mul_a,
            mul_b,
            mul_asz,
            mul_bsz
        );

        APAC_ALWAYS_ASSERT(ret != APAC_OOM);

        ap_dig_t carry = apn_add(
            temp,
            temp,
            rmdr,
            num_size,
            div_size
        );

        APAC_ALWAYS_ASSERT(carry == 0);

        int cmp = apn_cmp(temp, op2, num_size);

        APAC_ALWAYS_ASSERT(cmp == 0);

        int cmp2 = apn_cmp(rmdr, op1, div_size);

        APAC_ALWAYS_ASSERT(cmp2 == -1);
    }

    apac_free(temp);
    apac_free(rmdr);
    apac_free(quot);
    apac_free(op2);
    apac_free(op1);
}

int
main(void)
{
    apac_init();

    char* prng_seed = getenv("PRNG_SEED");
    char* iter = getenv("ITERATIONS");

    uint64_t seed = 0x91AC73E54BD2086FULL;
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

    check_apn_div(iterations);

    return EXIT_SUCCESS;
}