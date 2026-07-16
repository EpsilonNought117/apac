#include "../src/header/apac_internal.h"

#define TEST_SIZE_MAX ((apn_size_t)512ULL)

static int 
cmp_ref(
    const apn_dig_t* op1, 
    const apn_dig_t* op2, 
    apn_size_t size
)
{
    apn_size_t i = size;

    while (i-- > 0)
    {
        if (!(op1[i] == op2[i]))
        {
            goto unequal;
        }
    }

    return 0;

unequal:

    return (op1[i] > op2[i]) ? 1 : -1;
}

static void
check_apn_cmp(uint64_t iterations)
{
    apn_dig_t* op1 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);
    apn_dig_t* op2 = apac_malloc(sizeof(apn_dig_t) * TEST_SIZE_MAX);

    APAC_ALWAYS_ASSERT(op1 != NULL);
    APAC_ALWAYS_ASSERT(op2 != NULL);

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op2, TEST_SIZE_MAX, 0);

    while (iterations--)
    {
        apn_size_t size = 0;

        do
        {
            apn_set_random(&size, 1);
            size %= (TEST_SIZE_MAX + 1);

        } while (size == 0);

        /* TEST-1: equality */

        apn_dig_t val = 0;

        do
        {
            apn_set_random(&val, 1);

        } while (val == 0);

        apn_set(op1, size, val);
        apn_set(op2, size, val);

        int cmp_res = apn_cmp(op1, op2, size);

        APAC_ALWAYS_ASSERT(cmp_res == 0);

        /* TEST-2: randomized differential */

        apn_set_random(op1, size);

        apn_cpy(op2, op1, size);

        apn_size_t pos = 0;

        do
        {
            apn_set_random(&pos, 1);

        } while (pos == 0);

        pos %= size;

        apn_dig_t bit;
        apn_set_random(&bit, 1);
        int mutate_op1 = bit & 1;

        if (mutate_op1)
        {
            if (op1[pos] == APN_DIG_MAX)
                op1[pos]--;
            else
                op1[pos]++;
        }
        else
        {
            if (op2[pos] == 0)
                op2[pos]++;
            else
                op2[pos]--;
        }

        int compare_result = apn_cmp(op1, op2, size);
        int expected_cmp = cmp_ref(op1, op2, size);

        APAC_ALWAYS_ASSERT(compare_result == expected_cmp);
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

    uint64_t seed = 0x91A7F23CB8D54E6FULL;
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

    check_apn_cmp(iterations);

    return EXIT_SUCCESS;
}