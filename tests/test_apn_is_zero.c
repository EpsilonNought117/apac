#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((ap_size_t)512ULL)

static int
is_zero_ref(
    const ap_dig_t* op,
    ap_size_t size
)
{
    ap_size_t i = size;

    while (i-- > 0)
    {
        if (op[i] != 0)
        {
            return 1;
        }
    }

    return 0;
}

static void
check_apn_is_zero(uint64_t iterations)
{
    ap_dig_t* op = apac_malloc(sizeof(ap_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op != NULL);

    apn_set(op, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        ap_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= TEST_SIZE_MAX;

        } while (size == 0);

        /* TEST-1: zero */

        apn_set(op, size, 0);

        int res = apn_is_zero(op, size);
        int ref = is_zero_ref(op, size);

        APAC_ALWAYS_ASSERT(res == ref);

        /* TEST-2: randomized non-zero limb */

        apn_set(op, size, 0);

        ap_size_t pos = 0;

        apn_set_random(&pos, 1);

        pos %= size;

        ap_dig_t val = 0;

        do
        {
            apn_set_random(&val, 1);

        } while (val == 0);

        op[pos] = val;

        res = apn_is_zero(op, size);
        ref = is_zero_ref(op, size);

        APAC_ALWAYS_ASSERT(res == ref);
    }

    apac_free(op);
}

int
main(void)
{
    apac_init();

    char* prng_seed = getenv("PRNG_SEED");
    char* iter = getenv("ITERATIONS");

    uint64_t seed = 0xD84C29B17F63AE52ULL;
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

    check_apn_is_zero(iterations);

    return EXIT_SUCCESS;
}