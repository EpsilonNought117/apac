#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)512ULL)

static void
set_ref(
    ap_dig_t* op,
    ap_size_t size,
    ap_dig_t val
)
{
    ap_size_t i = 0;

    while (i < size)
    {
        op[i] = val;
        i++;
    }
}

static void
check_apn_set(uint64_t iterations)
{
    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        ap_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        ap_dig_t val = 0;

        apn_set_random(&val, 1);

        set_ref(op1, size, val);

        apn_set(op2, size, val);

        int cmp_res = apn_cmp(op1, op2, size);

        APAC_ALWAYS_ASSERT(cmp_res == 0);
    }

    apac_free(op2);
    apac_free(op1);
}

int
main(void)
{
    apac_init();

    char* prng_seed = getenv("PRNG_SEED");
    char* iter = getenv("ITERATIONS");

    uint64_t seed = 0x8F31C7A5D29E4B61ULL;
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

    check_apn_set(iterations);

    return EXIT_SUCCESS;
}