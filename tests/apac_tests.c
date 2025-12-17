#include "apac.h"
#include <memory.h>

#define TEST_START(name)														\
		printf(																	\
			"\n"																\
			"0==========================================================0\n"	\
			"|                    START TESTS: %-24s |\n"						\
			"0==========================================================0\n",	\
			name																\
		)

#define TEST_END(name)															\
		printf(																	\
			"\n"																\
			"0==========================================================0\n"	\
			"|                      END TESTS: %-24s |\n"						\
			"0==========================================================0\n",	\
			name																\
		)

#define MALLOC_AND_CHECK(op, size)														\
		{																				\
			op = apac_malloc(sizeof(apn_seg_t) * (size));								\
			if (!(op))																	\
			{																			\
				APAC_LOG_ERR("Memory allocation failure in unit test! Aborting ...");	\
				abort();																\
			}																			\
		}	

#if defined(_MSC_VER)

	#define ROTL64(x, k) (_rotl64((x), (k)))

#elif defined(__GNUC__) || defined(__clang__)

	#define ROTL64(x, k) (__builtin_rotateleft64((x), (k)))

#else

	#error "Unknown Compiler!"

#endif

/* PRNG */

static uint64_t prng_state[4] = { 0 };

static inline uint64_t random_sfc64(void) {
	uint64_t out = prng_state[1] + prng_state[2] + prng_state[0]++;
	prng_state[1] = prng_state[2] ^ (prng_state[2] >> 11);
	prng_state[2] = prng_state[3] + (prng_state[3] << 3);
	prng_state[3] = ROTL64(prng_state[3], 24) + out;
	return out;
}

static inline void random_sfc64_seed(uint64_t seed) {
	prng_state[0] = 1;
	prng_state[1] = seed;
	prng_state[2] = seed;
	prng_state[3] = seed;
	for (int i = 0; i < 16; i++)
		random_sfc64();
}

static void set_to_random(apn_seg_t* op1, apn_size_t size)
{
	for (apn_size_t i = 0; i < size; i++)
	{
		op1[i] = random_sfc64();
	}
}

/*
    || ------------------------------------- INTEGRATION-TESTS ------------------------------------- ||
*/

// ---------------- TEST EDGE CASES AND MATH PROPERTIES (WHERE APPLICABLE) ----------------

/*
* TESTING ORDER:
*
*  1)  apn_cmp - done
*  2)  apn_set - done
*  3)  apn_cpy - done
*  4)  apn_is_zero - done 
*  5)  apn_add_one - done
*  6)  apn_add_n
*  7)  apn_add
*  8)  apn_sub_one
*  9)  apn_sub_n
* 10)  apn_sub
* 11)  apn_neg
* 12)  apn_lshift
* 13)  apn_rshift
* 14)  apn_addmul_one
* 15)  apn_submul_one
* 16)  apn_mul_n
* 17)  apn_mul
* 18)  apn_sqr
* 19)  apn_div_one
* 20)  apn_div
* 
*/
static void check_apn_set(void)
{
    TEST_START("apn_set");

    apn_seg_t* op1 = NULL, * op2 = NULL;
    apn_size_t test_size = 512ULL;

    MALLOC_AND_CHECK(op1, test_size);
    MALLOC_AND_CHECK(op2, test_size);

    printf("\nTEST-1: Comparison against memset\n\n");

    for (apn_size_t i = 1; i <= test_size; i++)
    {
        printf("\tTesting size: %llu ... ", i);

        uint64_t val = random_sfc64();
        uint8_t  val1 = (uint8_t)(val & 0xFF);
        uint64_t val2 = val1 * 0x0101010101010101ULL;

        memset(op1, val1, sizeof(apn_seg_t) * i);
        apn_set(op2, i, val2);

        int cmp_res = memcmp(op1, op2, sizeof(apn_seg_t) * i);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_set() memcmp test failed!\n"
            "\t Operand length tested        : %llu\n"
            "\t Expected comparison result  : 0\n"
            "\t Actual comparison result    : %i\n",
            i,
            cmp_res
        );

        printf("PASSED\n");
    }

    TEST_END("apn_set");

    apac_free(op2);
    apac_free(op1);
}

static void check_apn_cmp(void)
{
    TEST_START("apn_cmp");

    apn_seg_t* op1 = NULL, * op2 = NULL;
    apn_size_t test_size = 512ULL;

    MALLOC_AND_CHECK(op1, test_size);
    MALLOC_AND_CHECK(op2, test_size);

    printf("\nTEST-1: Equality test\n\n");

    for (apn_size_t i = 1; i <= test_size; i++)
    {
        printf("\tTesting size: %llu ... ", i);

        uint64_t val = random_sfc64();

        apn_set(op1, i, val);
        apn_set(op2, i, val);

        int cmp_res = apn_cmp(op1, op2, i);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_cmp() equality test failed!\n"
            "\t Operand length tested        : %llu\n"
            "\t Expected comparison result  : 0\n"
            "\t Actual comparison result    : %i\n",
            i,
            cmp_res
        );

        printf("PASSED\n");
    }

    printf("\nTEST-2: Controlled inequality test\n\n");

    for (apn_size_t i = 1; i <= test_size; i++)
    {
        printf("\tTesting size: %llu ... ", i);

        uint64_t val = random_sfc64();

        apn_set(op1, i, val);
        apn_set(op2, i, val);

        apn_size_t pos = random_sfc64() % i;

        uint64_t diff;
        do { diff = random_sfc64(); } while (diff == val);

        if (random_sfc64() & 1)
            op1[pos] = diff;
        else
            op2[pos] = diff;

        int compare_result = apn_cmp(op1, op2, i);

        int diff_in_op1 = (op1[pos] == diff);

        int expected_cmp;
        if (diff > val)
            expected_cmp = diff_in_op1 ? 1 : -1;
        else
            expected_cmp = diff_in_op1 ? -1 : 1;

        APAC_ALWAYS_ASSERT(
            compare_result == expected_cmp,
            "apn_cmp() inequality test failed!\n"
            "\t Operand length tested        : %llu\n"
            "\t Point of first difference   : %llu\n"
            "\t Difference value            : 0x%016llx\n"
            "\t Base value                  : 0x%016llx\n"
            "\t Expected comparison result  : %i\n"
            "\t Actual comparison result    : %i\n",
            i,
            pos,
            (unsigned long long)diff,
            (unsigned long long)val,
            expected_cmp,
            compare_result
        );

        printf("PASSED\n");
    }

    TEST_END("apn_cmp");

    apac_free(op2);
    apac_free(op1);
}

static void check_apn_cpy(void)
{
    TEST_START("apn_cpy");

    apn_seg_t* op1 = NULL, * op2 = NULL;
    apn_size_t test_size = 512ULL;

    MALLOC_AND_CHECK(op1, test_size);
    MALLOC_AND_CHECK(op2, test_size);

    set_to_random(op1, test_size);

    for (apn_size_t i = 1; i <= test_size; i++)
    {
        printf("\tTesting size: %llu ... ", i);

        apn_cpy(op2, op1, i);

        int cmp_res = apn_cmp(op1, op2, i);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_cpy() equality test failed!\n"
            "\t Operand length tested        : %llu\n"
            "\t Expected comparison result  : 0\n"
            "\t Actual comparison result    : %i\n",
            i,
            cmp_res
        );

        apn_set(op2, i, 0ULL);

        printf("PASSED\n");
    }

    TEST_END("apn_cpy");

    apac_free(op2);
    apac_free(op1);
}

static void check_apn_is_zero(void)
{
    TEST_START("apn_is_zero");

    apn_seg_t* op = NULL;
    apn_size_t test_size = 512ULL;

    MALLOC_AND_CHECK(op, test_size);

    apn_set(op, test_size, 0);

    printf("\n\tTEST-1: All-zero input\n\n");

    for (apn_size_t i = 1; i <= test_size; i++)
    {
        printf("\tTesting size: %llu ... ", i);

        int res = apn_is_zero(op, i);

        APAC_ALWAYS_ASSERT(
            res == 0,
            "apn_is_zero() all-zero test failed!\n"
            "\t Operand length tested        : %llu\n"
            "\t Expected result              : 0\n"
            "\t Actual result                : %i\n",
            i,
            res
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-2: Single non-zero segment\n\n");

    for (apn_size_t i = 1; i <= test_size; i++)
    {
        printf("\tTesting size: %llu ... ", i);

        apn_size_t pos = random_sfc64() % i;

        uint64_t val;
        do { val = random_sfc64(); } while (val == 0);

        op[pos] = val;

        int res = apn_is_zero(op, i);

        APAC_ALWAYS_ASSERT(
            res == 1,
            "apn_is_zero() non-zero test failed!\n"
            "\t Operand length tested        : %llu\n"
            "\t Non-zero position            : %llu\n"
            "\t Non-zero value               : 0x%016llx\n"
            "\t Expected result              : 1\n"
            "\t Actual result                : %i\n",
            i,
            pos,
            (unsigned long long)val,
            res
        );

        op[pos] = 0;

        printf("PASSED\n");
    }

    TEST_END("apn_is_zero");

    apac_free(op);
}

static void check_apn_add_one(void)
{
    TEST_START("apn_add_one");

	apn_seg_t* op1 = NULL, *op2 = NULL;
	apn_size_t test_size = 256ULL;

	MALLOC_AND_CHECK(op1, test_size);
	MALLOC_AND_CHECK(op2, test_size);

	/* TEST-1: Carry Propagation */
	printf("\n\tTEST-1: Full Carry propagation\n\n");

	apn_set(op1, test_size, 0xFFFFFFFFFFFFFFFFULL);
	apn_set(op2, test_size, 0x00ULL);

	for (apn_size_t i = 1; i <= test_size; i++)
	{
		printf("\tTesting size: %llu ... ", i);

		apn_seg_t carry_out = apn_add_one(op2, op1, i, 1ULL);

		APAC_ALWAYS_ASSERT(
			carry_out == 1,
			"apn_add_one() carry propagation test failed!\n"
			"\t Operand length tested      : %llu\n"
			"\t Expected carry value       : 1\n"
			"\t Actual carry value         : %llu\n",
			i,
			carry_out
		);

		int is_zero = apn_is_zero(op2, i);

		APAC_ALWAYS_ASSERT(
			is_zero == 0,
			"apn_add_one() carry propagation test failed!\n"
			"\t Operand length tested      : %llu\n"
			"\t Expected result (is_zero)  : 0\n"
			"\t Actual result (is_zero)    : %d\n",
			i,
			is_zero
		);

		printf("PASSED\n");
	}

	/* TEST-2 Adding Identity Element */
	printf("\n\tTEST-2: Adding Identity Element\n\n");

	apn_set(op2, test_size, 0x00ULL);

	for (apn_size_t i = 1; i <= test_size; i++)
	{
		printf("\tTesting size: %llu ... ", i);

        uint64_t val = random_sfc64();

		apn_set(op1, i, val);
		apn_set(op2, i, 0);

		apn_seg_t carry_out = apn_add_one(op2, op1, i, 0);

		APAC_ALWAYS_ASSERT(
			carry_out == 0,
			"apn_add_one() identity test failed!\n"
			"\t Operand length tested      : %llu\n"
			"\t Expected carry value       : 0\n"
			"\t Actual carry value         : %llu\n",
			i,
			carry_out
		);

		int is_equal = apn_cmp(op1, op2, i);

		APAC_ALWAYS_ASSERT(
			is_equal == 0,
			"apn_add_one() identity test failed!\n"
			"\t Operand length tested      : %llu\n"
			"\t Expected result (cmp)      : 0 (identity property)\n"
			"\t Actual result (cmp)        : %d\n",
			i,
			is_equal
		);

		printf("PASSED\n");
	}

	/* TEST-3 No Carry Test */
	printf("\n\tTEST-3: Monotonicity Test\n\n");

    apn_set(op1, test_size, 0ULL);
    apn_set(op2, test_size, 0ULL);

    for (apn_size_t i = 1; i <= test_size; i++)
    {
        printf("\tTesting size: %llu ... ", i);

        set_to_random(op1, i);

        uint64_t val = random_sfc64();

        apn_seg_t carry_out = apn_add_one(op2, op1, i, val);

        if (carry_out == 0)
        {
            int cmp_res = apn_cmp(op2, op1, i);

            APAC_ALWAYS_ASSERT(
                cmp_res > 0,
                "apn_add_one() monotonicity test failed!\n"
                "\t Operand length tested      : %llu\n"
                "\t Addend value               : 0x%016llx\n"
                "\t Expected result            : op2 > op1 (no carry)\n"
                "\t Actual cmp result          : %d\n",
                i,
                (unsigned long long)val,
                cmp_res
            );
        }

        apn_set(op1, i, 0ULL);
        apn_set(op2, i, 0ULL);

        printf("PASSED\n");
    }

    TEST_END("apn_add_one");

	apac_free(op2);
	apac_free(op1);
	return;
}

int main(void)
{
	apacInit();
	random_sfc64_seed(0x117ULL);


	return 0;
}

