#include "apac.h"

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

#define TEST_SIZE_MAX   (apn_size_t)512ULL

#if defined(_MSC_VER)

	#define ROTL64(x, k) (_rotl64((x), (k)))

#elif defined(__GNUC__) || defined(__clang__)

	#define ROTL64(x, k) (__builtin_rotateleft64((x), (k)))

#else

	#error "Unknown Compiler!"

#endif

/* PRNG */

static uint64_t prng_state[4] = { 0 };

static inline uint64_t random_sfc64(void)
{
	uint64_t out = prng_state[1] + prng_state[2] + prng_state[0];
    prng_state[0]++;
	prng_state[1] = prng_state[2] ^ (prng_state[2] >> 11);
	prng_state[2] = prng_state[3] + (prng_state[3] << 3);
	prng_state[3] = ROTL64(prng_state[3], 24) + out;
	return out;
}

static inline void random_sfc64_seed(uint64_t seed)
{
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
*  1)  apn_set          - done
*  2)  apn_cpy          - done
*  3)  apn_cmp          - done
*  4)  apn_is_zero      - done
*  5)  apn_add_one      - done
*  6)  apn_add_n        - done
*  7)  apn_add          - done
*  8)  apn_neg          - done
*  9)  apn_sub_one      - done
* 10)  apn_sub_n        - done
* 11)  apn_sub          - done
* 12)  apn_addmul_one   - done
* 13)  apn_lshift
* 14)  apn_rshift
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

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);

    printf("\nTEST-1: Comparison against memset\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        uint64_t val = 0;
        do
        {
            val = random_sfc64();
        }
        while (val == 0);

        uint8_t  val1 = (uint8_t)(val & 0xFF);
        uint64_t val2 = val1 * 0x0101010101010101ULL;

        memset(op1, val1, sizeof(apn_seg_t) * i);
        apn_set(op2, i, val2);

        int cmp_res = memcmp(op1, op2, sizeof(apn_seg_t) * i);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_set() memcmp test failed!\n"
            "\t Operand length tested    : %" PRI_APN_SIZE "\n"
            "\t Actual comparison result : %i\n",
            (apn_size_t)i,
            cmp_res
        );

        printf("PASSED\n");
    }

    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_set");
}

static void check_apn_cpy(void)
{
    TEST_START("apn_cpy");

    apn_seg_t* op1 = NULL, * op2 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);

    apn_set(op1, TEST_SIZE_MAX, 0ULL);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);
        
        set_to_random(op1, i);
        apn_cpy(op2, op1, i);

        int cmp_res = memcmp(op1, op2, i * sizeof(apn_seg_t));

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_cpy() equality test failed!\n"
            "\t Operand length tested    : %" PRI_APN_SIZE "\n"
            "\t Actual comparison result : %i\n",
            (apn_size_t)i,
            cmp_res
        );

        apn_set(op2, i, 0ULL);

        printf("PASSED\n");
    }

    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_cpy");
}

static void check_apn_cmp(void)
{
    TEST_START("apn_cmp");

    apn_seg_t* op1 = NULL, * op2 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);

    printf("\nTEST-1: Equality test\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        uint64_t val = 0;
        do
        {
            val = random_sfc64();

        } while (val == 0);

        apn_set(op1, i, val);
        apn_set(op2, i, val);

        int cmp_res = apn_cmp(op1, op2, i);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_cmp() equality test failed!\n"
            "\t Operand length tested    : %" PRI_APN_SIZE "\n"
            "\t Actual comparison result : %i\n",
            (apn_size_t)i,
            cmp_res
        );

        printf("PASSED\n");
    }

    printf("\nTEST-2: Single segment difference test\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        set_to_random(op1, i);
        apn_cpy(op2, op1, i);

        /* Pick a random position */
        apn_size_t pos = random_sfc64() % i;

        /* Base value at that position */
        uint64_t base = op1[pos];

        /* Controlled difference: base 1 */
        uint64_t diff;
        int plus = random_sfc64() & 1;

        if (plus)
            diff = base + 1;
        else
            diff = base - 1;

        /* Decide which operand gets modified */
        int diff_in_op1 = random_sfc64() & 1;

        if (diff_in_op1)
            op1[pos] = diff;
        else
            op2[pos] = diff;

        int compare_result = apn_cmp(op1, op2, i);

        /* Expected result */
        int expected_cmp;
        if (diff > base)
            expected_cmp = diff_in_op1 ? 1 : -1;
        else
            expected_cmp = diff_in_op1 ? -1 : 1;

        APAC_ALWAYS_ASSERT(
            compare_result == expected_cmp,
            "apn_cmp() single-segment difference test failed!\n"
            "\t Operand length tested        : %" PRI_APN_SIZE "\n"
            "\t Change position              : %" PRI_APN_SIZE "\n"
            "\t Base value                   : %" PRI_APN_SEG "\n"
            "\t Modified value               : %" PRI_APN_SEG "\n"
            "\t Modified operand             : %s\n"
            "\t Expected comparison result   : %i\n"
            "\t Actual comparison result     : %i\n",
            (apn_size_t)i,
            (apn_size_t)pos,
            (apn_seg_t)base,
            (apn_seg_t)diff,
            diff_in_op1 ? "op1" : "op2",
            expected_cmp,
            compare_result
        );

        printf("PASSED\n");
    }

    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_cmp");
}

static void check_apn_is_zero(void)
{
    TEST_START("apn_is_zero");

    apn_seg_t* op = NULL;

    MALLOC_AND_CHECK(op, TEST_SIZE_MAX);

    apn_set(op, TEST_SIZE_MAX, 0);

    printf("\n\tTEST-1: All-zero input\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        int res = apn_is_zero(op, i);

        APAC_ALWAYS_ASSERT(
            res == 0,
            "apn_is_zero() all-zero test failed!\n"
            "\t Operand length tested        : %" PRI_APN_SIZE "\n"
            "\t Actual result                : %i\n",
            (apn_size_t)i,
            res
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-2: Single non-zero segment\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        apn_size_t pos = random_sfc64() % i;

        uint64_t val = 0;
        do { val = random_sfc64(); } while (val == 0);

        op[pos] = val;

        int res = apn_is_zero(op, i);

        APAC_ALWAYS_ASSERT(
            res == 1,
            "apn_is_zero() non-zero test failed!\n"
            "\t Operand length tested        : %" PRI_APN_SIZE "\n"
            "\t Non-zero position            : %" PRI_APN_SIZE "\n"
            "\t Non-zero value               : %" PRI_APN_SEG "\n"
            "\t Actual result                : %i\n",
            (apn_size_t)i,
            (apn_size_t)pos,
            (apn_seg_t)val,
            res
        );

        op[pos] = 0;

        printf("PASSED\n");
    }

    apac_free(op);

    TEST_END("apn_is_zero");
}

static void check_apn_add_one(void)
{
    TEST_START("apn_add_one");

	apn_seg_t* op1 = NULL, *op2 = NULL;

	MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
	MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);

	/* TEST-1: Carry Propagation */
	printf("\n\tTEST-1: Full Carry propagation\n\n");

	apn_set(op1, TEST_SIZE_MAX, APN_SEG_MAX);
	apn_set(op2, TEST_SIZE_MAX, 0x00ULL);

	for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
	{
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

		apn_seg_t carry_out = apn_add_one(op2, op1, i, 1);

        APAC_ALWAYS_ASSERT(
            carry_out == 1,
            "apn_add_one() carry propagation test failed!\n"
            "\t Operand length tested      : %" PRI_APN_SIZE "\n"
            "\t Actual carry value         : %" PRI_APN_SEG "\n",
            (apn_size_t)i,
            (apn_seg_t)carry_out
        );

		int is_zero = apn_is_zero(op2, i);

		APAC_ALWAYS_ASSERT(
			is_zero == 0,
			"apn_add_one() carry propagation test failed!\n"
			"\t Operand length tested      : %" PRI_APN_SIZE "\n"
			"\t Actual result (is_zero)    : %d\n",
			i,
			is_zero
		);

		printf("PASSED\n");
	}

	/* TEST-2 Adding Identity Element */
	printf("\n\tTEST-2: Adding Identity Element\n\n");

	apn_set(op2, TEST_SIZE_MAX, 0x00ULL);

	for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
	{
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        uint64_t val = 0;
        do
        {
            val = random_sfc64();
        } 
        while (val == 0);

		set_to_random(op1, i);
		apn_seg_t carry_out = apn_add_one(op2, op1, i, 0);

        APAC_ALWAYS_ASSERT(
            carry_out == 0,
            "apn_add_one() identity test failed!\n"
            "\t Operand length tested      : %" PRI_APN_SIZE "\n"
            "\t Actual carry value         : %" PRI_APN_SEG "\n",
            (apn_size_t)i,
            (apn_seg_t)carry_out
        );

		int is_equal = apn_cmp(op1, op2, i);

		APAC_ALWAYS_ASSERT(
			is_equal == 0,
			"apn_add_one() identity test failed!\n"
			"\t Operand length tested      : %" PRI_APN_SIZE "\n"
			"\t Actual result (cmp)        : %d\n",
			i,
			is_equal
		);

		printf("PASSED\n");
	}

	/* TEST-3 No Carry Test */
	printf("\n\tTEST-3: Monotonicity Test\n\n");

    apn_set(op1, TEST_SIZE_MAX, 0ULL);
    apn_set(op2, TEST_SIZE_MAX, 0ULL);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        set_to_random(op1, i);

        uint64_t val = 0;
        do
        {
            val = random_sfc64();
        }
        while (val == 0);

        apn_seg_t carry_out = apn_add_one(op2, op1, i, val);

        int cmp_res = apn_cmp(op2, op1, i);

        APAC_ALWAYS_ASSERT(
            cmp_res == (carry_out ? -1 : 1),
            "apn_add_one() monotonicity test failed!\n"
            "\t Operand length tested      : %" PRI_APN_SIZE "\n"
            "\t Addend value               : 0x%016" PRI_APN_SEG "\n"
            "\t Carry out                  : %" PRI_APN_SEG "\n"
            "\t Expected cmp result        : %d\n"
            "\t Actual cmp result          : %d\n",
            (apn_size_t)i,
            (apn_seg_t)val,
            (apn_seg_t)carry_out,
            carry_out ? -1 : 1,
            cmp_res
        );

        printf("PASSED\n");
    }

	apac_free(op2);
	apac_free(op1);

    TEST_END("apn_add_one");
}

static void check_apn_add_n(void)
{
    TEST_START("apn_add_n");

    apn_seg_t* op1 = NULL, * op2 = NULL, * op3 = NULL;
    apn_seg_t* op4 = NULL, * op5 = NULL, * op6 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op4, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op5, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op6, TEST_SIZE_MAX);

    apn_set(op2, TEST_SIZE_MAX, 0);

    printf("\n\tTEST-1: Adding identity element\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        set_to_random(op1, i);

        apn_seg_t carry = apn_add_n(op3, op1, op2, i);
        int cmp_res = apn_cmp(op3, op1, i);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_add_n() identity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Actual cmp result     : %d\n",
            (apn_size_t)i,
            cmp_res
        );

        APAC_ALWAYS_ASSERT(
            carry == 0,
            "apn_add_n() identity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Actual carry          : %" PRI_APN_SEG "\n",
            (apn_size_t)i,
            (apn_seg_t)carry
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-2: Carry Out Test\n\n");

    apn_set(op1, TEST_SIZE_MAX, APN_SEG_MAX);
    apn_set(op2, TEST_SIZE_MAX, APN_SEG_MAX);
    op2[0] = APN_SEG_MAX - 1;

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        apn_seg_t carry = apn_add_n(op3, op1, op1, i);
        int cmp_res = apn_cmp(op3, op2, i);

        APAC_ALWAYS_ASSERT(
            carry == 1,
            "apn_add_n() carry-out test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Actual carry          : %" PRI_APN_SEG "\n",
            (apn_size_t)i,
            (apn_seg_t)carry
        );

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_add_n() carry-out test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Actual cmp result     : %d\n",
            (apn_size_t)i,
            cmp_res
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-3: Monotonicity Test\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        set_to_random(op1, i);
        set_to_random(op2, i);

        apn_seg_t carry = apn_add_n(op3, op1, op2, i);

        int cmp3_1 = apn_cmp(op3, op1, i);
        int cmp3_2 = apn_cmp(op3, op2, i);

        int monotonic_ok =
            (carry == 0) ? (cmp3_1 >= 0 && cmp3_2 >= 0)
            : (cmp3_1 < 0 && cmp3_2 < 0);

        APAC_ALWAYS_ASSERT(
            monotonic_ok == 1,
            "apn_add_n() monotonicity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Carry out             : %" PRI_APN_SEG "\n"
            "\t cmp(op3, op1)         : %d\n"
            "\t cmp(op3, op2)         : %d\n",
            (apn_size_t)i,
            (apn_seg_t)carry,
            cmp3_1,
            cmp3_2
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-4: Commutativity Test\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        set_to_random(op1, i);
        set_to_random(op2, i);

        apn_add_n(op3, op1, op2, i); /* a + b */
        apn_add_n(op4, op2, op1, i); /* b + a */

        int cmp = apn_cmp(op3, op4, i);

        APAC_ALWAYS_ASSERT(
            cmp == 0,
            "apn_add_n() commutativity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(a+b, b+a)         : %d\n",
            (apn_size_t)i,
            cmp
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-5: Associativity Test\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        set_to_random(op1, i);
        set_to_random(op2, i);
        set_to_random(op3, i);

        apn_seg_t carry0 = 0, carry1 = 0;

        /* (a + b) + c */
        carry0 += apn_add_n(op4, op1, op2, i);
        carry0 += apn_add_n(op5, op4, op3, i);

        /* a + (b + c) */
        carry1 += apn_add_n(op6, op2, op3, i);
        carry1 += apn_add_n(op4, op1, op6, i);

        int cmp = apn_cmp(op5, op4, i);

        APAC_ALWAYS_ASSERT(
            cmp == 0,
            "apn_add_n() associativity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp((a+b)+c, a+(b+c)) : %d\n",
            (apn_size_t)i,
            cmp
        );

        APAC_ALWAYS_ASSERT(
            carry0 == carry1,
            "apn_add_n() associativity carry mismatch!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t carry0 : %" PRI_APN_SEG "\n"
            "\t carry1 : %" PRI_APN_SEG "\n",
            (apn_size_t)i,
            (apn_seg_t)carry0,
            (apn_seg_t)carry1
        );

        printf("PASSED\n");
    }

    apac_free(op6);
    apac_free(op5);
    apac_free(op4);
    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_add_n");
}

static void check_apn_add(void)
{
    TEST_START("apn_add");

    apn_seg_t* a = NULL, * b = NULL;
    apn_seg_t* r1 = NULL, * t1 = NULL;

    MALLOC_AND_CHECK(a, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(b, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(r1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(t1, TEST_SIZE_MAX);

    printf("\n\tTEST-1: Identity (a + 0 = a)\n\n");

    /* b = 0 for entire test */
    apn_set(b, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (apn_size_t j = 1; j <= i; j++)
        {
            printf(
                "\tTesting size: a=%" PRI_APN_SIZE ", b=%" PRI_APN_SIZE " ... ",
                i,
                j
            );

            set_to_random(a, i);

            apn_seg_t carry = apn_add(r1, a, b, i, j);
            int cmp = apn_cmp(r1, a, i);

            APAC_ALWAYS_ASSERT(
                carry == 0,
                "apn_add() identity test failed: unexpected carry!\n"
                "\t size_a : %" PRI_APN_SIZE "\n"
                "\t size_b : %" PRI_APN_SIZE "\n"
                "\t carry  : %" PRI_APN_SEG "\n",
                i, j, carry
            );

            APAC_ALWAYS_ASSERT(
                cmp == 0,
                "apn_add() identity test failed: result != a!\n"
                "\t size_a : %" PRI_APN_SIZE "\n"
                "\t size_b : %" PRI_APN_SIZE "\n",
                i, j
            );

            printf("PASSED\n");
        }
    }

    printf("\n\tTEST-2: Correctness vs apn_add_n()\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: i=%" PRI_APN_SIZE " ... ", i);

        set_to_random(a, i);
        set_to_random(b, i);

        apn_seg_t carry1 = apn_add(r1, a, b, i, i);
        apn_seg_t carry2 = apn_add_n(t1, a, b, i);

        int cmp = apn_cmp(r1, t1, i);

        APAC_ALWAYS_ASSERT(
            carry1 == carry2,
            "apn_add() correctness test failed: carry mismatch!\n"
            "\t size  : %" PRI_APN_SIZE "\n"
            "\t carry1: %" PRI_APN_SEG "\n"
            "\t carry2: %" PRI_APN_SEG "\n",
            i, carry1, carry2
        );

        APAC_ALWAYS_ASSERT(
            cmp == 0,
            "apn_add() correctness test failed: result mismatch!\n"
            "\t size  : %" PRI_APN_SIZE "\n",
            i
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-3: Full carry-out (all-ones operands)\n\n");

    apn_set(a, TEST_SIZE_MAX, APN_SEG_MAX);
    apn_set(b, TEST_SIZE_MAX, APN_SEG_MAX);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (apn_size_t j = 1; j <= i; j++)
        {
            printf(
                "\tTesting size: a=%" PRI_APN_SIZE ", b=%" PRI_APN_SIZE " ... ",
                i, j
            );

            apn_set(t1, j, APN_SEG_MAX);
            t1[0] = APN_SEG_MAX - 1;
            apn_set(t1 + j, i - j, 0);

            apn_seg_t carry = apn_add(r1, a, b, i, j);
            int cmp_res = apn_cmp(r1, t1, i);

            APAC_ALWAYS_ASSERT(
                carry == 1,
                "apn_add() full-ones carry-out test failed: missing carry!\n"
                "\t size_a : %" PRI_APN_SIZE "\n"
                "\t size_b : %" PRI_APN_SIZE "\n"
                "\t carry  : %" PRI_APN_SEG "\n",
                i, j, carry
            );

            APAC_ALWAYS_ASSERT(
                cmp_res == 0,
                "apn_add() full-ones result mismatch!\n"
                "\t size_a : %" PRI_APN_SIZE "\n"
                "\t size_b : %" PRI_APN_SIZE "\n"
                "\t cmp(r, expected) : %d\n",
                i, j, cmp_res
            );

            printf("PASSED\n");
        }
    }

    apac_free(t1);
    apac_free(r1);
    apac_free(b);
    apac_free(a);

    TEST_END("apn_add");
}

static void check_apn_neg(void)
{
    TEST_START("apn_neg");

    apn_seg_t* op1 = NULL;
    apn_seg_t* op2 = NULL;
    apn_seg_t* op3 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX);

    printf("\n\tTEST-1: a + (-a) == 0 with carry\n\n");

    for (apn_size_t size = 1; size <= TEST_SIZE_MAX; size++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", size);

        /* Randomize operand */
        set_to_random(op1, size);

        /* op2 = -op1 */
        apn_neg(op2, op1, size);

        apn_seg_t carry = apn_add_n(op3, op1, op2, size);

        APAC_ALWAYS_ASSERT(
            carry == 1,
            "apn_neg() correctness test failed (carry)!\n"
            "\t size  : %" PRI_APN_SIZE "\n"
            "\t carry : %" PRI_APN_SEG "\n",
            (apn_size_t)size,
            (apn_seg_t)carry
        );

        int cmp_val = apn_is_zero(op3, size);

        APAC_ALWAYS_ASSERT(
            cmp_val == 0,
            "apn_neg() correctness test failed (result not zero)!\n"
            "\t size  : %" PRI_APN_SIZE "\n",
            (apn_size_t)size
        );

        printf("PASSED\n");
    }

    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_neg");
}

static void check_apn_sub_one(void)
{
    TEST_START("apn_sub_one");

    apn_seg_t* op1 = NULL;
    apn_seg_t* op2 = NULL;
    apn_seg_t* op3 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX);

    /* TEST-1: Full Borrow Propagation */
    printf("\n\tTEST-1: Full Borrow propagation\n\n");

    apn_set(op1, TEST_SIZE_MAX, 0x00ULL);
    apn_set(op2, TEST_SIZE_MAX, 0x00ULL);
    apn_set(op3, TEST_SIZE_MAX, APN_SEG_MAX);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", i);

        apn_seg_t borrow_out = apn_sub_one(op2, op1, i, 1ULL);

        APAC_ALWAYS_ASSERT(
            borrow_out == 1,
            "apn_sub_one() borrow propagation test failed!\n"
            "\t Operand length tested      : %" PRI_APN_SIZE "\n"
            "\t Actual borrow value        : %" PRI_APN_SEG "\n",
            i,
            borrow_out
        );

        int cmp_res = apn_cmp(op3, op2, i);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_sub_one() borrow propagation test failed!\n"
            "\t Operand length tested   : %" PRI_APN_SIZE "\n"
            "\t Actual cmp(op3, op2)    : %d\n",
            i,
            cmp_res
        );

        printf("PASSED\n");
    }

    /* TEST-2: Subtracting Identity Element */
    printf("\n\tTEST-2: Subtracting Identity Element\n\n");

    apn_set(op2, TEST_SIZE_MAX, 0x00ULL);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", i);

        set_to_random(op1, i);
        apn_seg_t borrow_out = apn_sub_one(op2, op1, i, 0ULL);

        APAC_ALWAYS_ASSERT(
            borrow_out == 0,
            "apn_sub_one() identity test failed!\n"
            "\t Operand length tested      : %" PRI_APN_SIZE "\n"
            "\t Actual borrow value        : %" PRI_APN_SEG "\n",
            i,
            borrow_out
        );

        int cmp = apn_cmp(op2, op1, i);

        APAC_ALWAYS_ASSERT(
            cmp == 0,
            "apn_sub_one() identity test failed!\n"
            "\t Operand length tested      : %" PRI_APN_SIZE "\n"
            "\t Expected result (cmp)      : 0 (identity property)\n"
            "\t Actual result (cmp)        : %d\n",
            i,
            cmp
        );

        printf("PASSED\n");
    }

    /* TEST-3: Monotonicity Test */
    printf("\n\tTEST-3: Monotonicity Test\n\n");

    apn_set(op1, TEST_SIZE_MAX, 0ULL);
    apn_set(op2, TEST_SIZE_MAX, 0ULL);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", i);

        set_to_random(op1, i);

        uint64_t val;
        do
        {
            val = random_sfc64();

        }
        while (val == 0);

        apn_seg_t borrow_out = apn_sub_one(op2, op1, i, val);
        int cmp_res = apn_cmp(op2, op1, i);

        int expected = borrow_out ? 1 : -1;

        APAC_ALWAYS_ASSERT(
            cmp_res == expected,
            "apn_sub_one() monotonicity test failed!\n"
            "\t Operand length tested      : %" PRI_APN_SIZE "\n"
            "\t Subtrahend value           : 0x%016" PRI_APN_SEG "\n"
            "\t Borrow out                 : %" PRI_APN_SEG "\n"
            "\t Expected cmp result        : %d\n"
            "\t Actual cmp result          : %d\n",
            i,
            (apn_seg_t)val,
            borrow_out,
            expected,
            cmp_res
        );

        printf("PASSED\n");
    }

    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_sub_one");
}

static void check_apn_sub_n(void)
{
    TEST_START("apn_sub_n");

    apn_seg_t* op1 = NULL, * op2 = NULL;
    apn_seg_t* op3 = NULL, * op4 = NULL;;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op4, TEST_SIZE_MAX);

    printf("\n\tTEST-1: Subtracting Identity Element:\n\n");
    
    apn_set(op2, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        set_to_random(op1, i);

        apn_seg_t carry = apn_sub_n(op3, op1, op2, i);

        int cmp_res = apn_cmp(op3, op1, i);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_sub_n() identity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Actual cmp result     : %d\n",
            (apn_size_t)i,
            cmp_res
        );

        APAC_ALWAYS_ASSERT(
            carry == 0,
            "apn_add_n() identity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Actual carry          : %" PRI_APN_SEG  "\n",
            (apn_size_t)i,
            (apn_seg_t)carry
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-2: Full borrow propagation\n\n");

    apn_set(op2, TEST_SIZE_MAX, 0);
    apn_set(op1, TEST_SIZE_MAX, 0);
    op1[0] = 1;

    apn_set(op4, TEST_SIZE_MAX, APN_SEG_MAX);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        apn_seg_t carry = apn_sub_n(op3, op2, op1, i);

        int is_max = apn_cmp(op4, op3, i);  // should be equal

        APAC_ALWAYS_ASSERT(
            carry == 1,
            "apn_sub_n() full borrow propagation test failed: missing borrow!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Actual borrow         : %" PRI_APN_SEG "\n",
            (apn_size_t)i,
            (apn_seg_t)carry
        );

        APAC_ALWAYS_ASSERT(
            is_max == 0,
            "apn_sub_n() full borrow propagation test failed: result not zero!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t apn_is_zero(result)   : %d\n",
            (apn_size_t)i,
            is_max
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-3: Self subtraction (a - a = 0)\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        set_to_random(op1, i);

        /* op3 = op1 - op1 */
        apn_seg_t borrow = apn_sub_n(op3, op1, op1, i);

        int is_zero = apn_is_zero(op3, i);

        APAC_ALWAYS_ASSERT(
            borrow == 0,
            "apn_sub_n() self subtraction failed: unexpected borrow!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Borrow                : %" PRI_APN_SEG "\n",
            (apn_size_t)i,
            (apn_seg_t)borrow
        );

        APAC_ALWAYS_ASSERT(
            is_zero == 0,
            "apn_sub_n() self subtraction failed: result not zero!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t apn_is_zero(result)   : %d\n",
            (apn_size_t)i,
            is_zero
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-4: Borrow invariant (borrow == a < b)\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", (apn_size_t)i);

        set_to_random(op1, i);
        set_to_random(op2, i);

        apn_seg_t borrow = apn_sub_n(op3, op1, op2, i);

        int cmp = apn_cmp(op1, op2, i);

        APAC_ALWAYS_ASSERT(
            borrow == (cmp < 0),
            "apn_sub_n() borrow invariant failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(a, b)             : %d\n"
            "\t borrow                : %" PRI_APN_SEG "\n",
            (apn_size_t)i,
            cmp,
            (apn_seg_t)borrow
        );

        printf("PASSED\n");
    }

    apac_free(op4);
    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_sub_n");
}

static void check_apn_sub(void)
{
    TEST_START("apn_sub");

    apn_seg_t* a = NULL, * b = NULL, * r = NULL, * t = NULL;

    MALLOC_AND_CHECK(a, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(b, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(r, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(t, TEST_SIZE_MAX);

    printf("\n\tTEST-1: Identity (a - 0 = a)\n\n");

    apn_set(b, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (apn_size_t j = 1; j <= i; j++)
        {
            printf("\tTesting size: %" PRI_APN_SIZE ", %" PRI_APN_SIZE " ... ",
                i, j);

            set_to_random(a, i);

            apn_seg_t borrow = apn_sub(r, a, b, i, j);
            int cmp = apn_cmp(r, a, i);

            APAC_ALWAYS_ASSERT(
                borrow == 0,
                "apn_sub() identity failed: unexpected borrow!\n"
                "\t size_a : %" PRI_APN_SIZE "\n"
                "\t size_b : %" PRI_APN_SIZE "\n",
                i, j
            );

            APAC_ALWAYS_ASSERT(
                cmp == 0,
                "apn_sub() identity failed: result mismatch!\n"
                "\t size_a : %" PRI_APN_SIZE "\n"
                "\t size_b : %" PRI_APN_SIZE "\n",
                i, j
            );

            printf("PASSED\n");
        }
    }

    printf("\n\tTEST-2: Borrow invariant (borrow == a < b)\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (apn_size_t j = 1; j <= i; j++)
        {
            printf(
                "\tTesting size: a=%" PRI_APN_SIZE ", b=%" PRI_APN_SIZE " ... ",
                i, j
            );

            set_to_random(a, i);

            /* IMPORTANT: zero b fully before setting lower j limbs */
            apn_set(b, i, 0);
            set_to_random(b, j);

            apn_seg_t borrow = apn_sub(r, a, b, i, j);

            int cmp = apn_cmp(a, b, i);

            APAC_ALWAYS_ASSERT(
                borrow == (cmp < 0),
                "apn_sub() borrow invariant failed!\n"
                "\t size_a : %" PRI_APN_SIZE "\n"
                "\t size_b : %" PRI_APN_SIZE "\n"
                "\t cmp    : %d\n"
                "\t borrow : %" PRI_APN_SEG "\n",
                i, j, cmp, borrow
            );

            printf("PASSED\n");
        }
    }

    printf("\n\tTEST-3: Full borrow propagation (0 - 1)\n\n");

    apn_set(t, TEST_SIZE_MAX, APN_SEG_MAX);
    apn_set(a, TEST_SIZE_MAX, 0);
    apn_set(b, TEST_SIZE_MAX, 0);
    b[0] = 1;

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", i);

        apn_seg_t borrow = apn_sub(r, a, b, i, 1);
        int is_max = apn_cmp(r, t, i);

        APAC_ALWAYS_ASSERT(
            borrow == 1,
            "apn_sub() full borrow propagation failed!\n"
            "\t size : %" PRI_APN_SIZE "\n",
            i
        );

        APAC_ALWAYS_ASSERT(
            is_max == 0,
            "apn_sub() full borrow propagation failed: result not zero!\n"
            "\t size : %" PRI_APN_SIZE "\n",
            i
        );

        printf("PASSED\n");
    }

    printf("\n\tTEST-4: Self subtraction (a - a = 0)\n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        printf("\tTesting size: %" PRI_APN_SIZE " ... ", i);

        set_to_random(a, i);

        apn_seg_t borrow = apn_sub(r, a, a, i, i);
        int is_zero = apn_is_zero(r, i);

        APAC_ALWAYS_ASSERT(
            borrow == 0,
            "apn_sub() self subtraction failed: borrow!\n"
            "\t size : %" PRI_APN_SIZE "\n",
            i
        );

        APAC_ALWAYS_ASSERT(
            is_zero == 0,
            "apn_sub() self subtraction failed: result not zero!\n"
            "\t size : %" PRI_APN_SIZE "\n",
            i
        );

        printf("PASSED\n");
    }

    apac_free(t);
    apac_free(r);
    apac_free(b);
    apac_free(a);

    TEST_END("apn_sub");
}

int main(void)
{
	apacInit();
	random_sfc64_seed(0xFULL);

    check_apn_set();
    check_apn_cpy();
    check_apn_cmp();
    check_apn_is_zero();
    check_apn_add_one();
    check_apn_add_n();
    check_apn_add();
    check_apn_neg();
    check_apn_sub_one();
    check_apn_sub_n();
    check_apn_sub();

    printf("\nALL FUNCTIONS TESTED!");
	
    return 0;
}

