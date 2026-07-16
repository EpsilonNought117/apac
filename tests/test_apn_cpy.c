#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((apn_size_t)512ULL)

static void
cpy_ref(
    apn_dig_t* dst,
    const apn_dig_t* src,
    apn_size_t size
)
{
    for (apn_size_t i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}

static void
check_apn_cpy(uint64_t iterations)
{
    apn_dig_t* op1 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op2 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op3 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);
    APAC_ALWAYS_ASSERT(op3 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX, 0);
    apn_set(op3, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        apn_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= (TEST_SIZE_MAX + 1);

        } while (size == 0);

        apn_set_random(op1, size);

        cpy_ref(op2, op1, size);
        apn_cpy(op3, op1, size);

        int cmp_res = apn_cmp(op2, op3, size);

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

    uint64_t seed = 0x8F24D7B16CA35E91ULL;
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

    check_apn_cpy(iterations);

    return EXIT_SUCCESS;
}