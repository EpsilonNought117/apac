#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((apn_size_t)512ULL)

static void
check_apn_div(uint64_t iterations)
{
    apn_dig_t* op1 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op2 = apac_malloc(sizeof(apn_dig_t) * (TEST_SIZE_MAX * 2));
    apn_dig_t* quot = apac_malloc(sizeof(apn_dig_t) * (TEST_SIZE_MAX + 1));
    apn_dig_t* rmdr = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* temp = apac_malloc(sizeof(apn_dig_t) * ((TEST_SIZE_MAX * 2) + 1));

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
        apn_size_t i = 0;
        apn_size_t j = 0;

        do
        {
            apn_set_random(&i, 1);
            i %= (TEST_SIZE_MAX + 1);

        } while (i == 0);

        do
        {
            apn_set_random(&j, 1);
            j %= TEST_SIZE_MAX;

        } while (j == 0);

        /* divisor (size i), ensure top limb non-zero */
        do
        {
            apn_set_random(op1, i);

        } while (op1[i - 1] == 0);

        /* dividend (size i + j) */
        apn_set_random(op2, i + j);

        apn_set(quot, j + 1, 0);
        apn_set(rmdr, i, 0);
        apn_set(temp, i + j, 0);

        /* divide */
        apac_err ret = apn_div(
            quot,
            rmdr,
            op2,
            op1,
            i + j, /* dividend size */
            0,
            i      /* divisor size  */
        );

        APAC_ALWAYS_ASSERT(ret != APAC_OOM);
        
        /* decide multiplication order (quotient size is always j + 1) */
        apn_dig_t* mul_a;
        apn_dig_t* mul_b;
        apn_size_t mul_asz;
        apn_size_t mul_bsz;

        if ((j + 1) >= i)
        {
            mul_a = quot;
            mul_b = op1;
            mul_asz = j + 1;
            mul_bsz = i;
        }
        else
        {
            mul_a = op1;
            mul_b = quot;
            mul_asz = i;
            mul_bsz = j + 1;
        }

        /* temp = quot * op1 */
        ret = apn_mul(
            temp,
            mul_a,
            mul_b,
            mul_asz,
            mul_bsz
        );

        APAC_ALWAYS_ASSERT(ret != APAC_OOM);

        /* temp += rmdr */
        apn_dig_t carry = apn_add(
            temp,
            temp,
            rmdr,
            i + j,
            i
        );

        APAC_ALWAYS_ASSERT(carry == 0);

        /* reconstructed value must equal dividend */
        int cmp = apn_cmp(temp, op2, i + j);

        APAC_ALWAYS_ASSERT(cmp == 0);

        int cmp2 = apn_cmp(rmdr, op1, i);

        /* remainder invariant */
        APAC_ALWAYS_ASSERT(cmp2 == -1);

        /* reset */
        apn_set(op1, i, 0);
        apn_set(op2, i + j, 0);
        apn_set(quot, j + 1, 0);
        apn_set(rmdr, i, 0);
        apn_set(temp, i + j, 0);
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