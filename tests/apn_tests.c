#include "../include/apac.h"

#define TEST_START(name) printf("\nTEST START: %s()\n", name)

#define TEST_END(name) printf("TEST END: %s()\n", name);

#define MALLOC_AND_CHECK(op, size)														\
		{																				\
			op = apac_malloc(sizeof(apn_seg_t) * (size));								\
			if (!(op))																	\
			{																			\
				APAC_LOG_ERR("Memory allocation failure in unit test! Aborting ...");	\
				abort();																\
			}																			\
		}	

#define TEST_SIZE_MAX   (apn_size_t)1024ULL

#if defined(_MSC_VER)

	#define ROTL64(x, k) (_rotl64((x), (k)))

#elif defined(__GNUC__) || defined(__clang__)

    #define ROTL64(x, k) \
            (((uint64_t)(x) << ((k) & 63)) | ((uint64_t)(x) >> (64 - ((k) & 63))))

#else

	#error "Unknown Compiler!"

#endif

/* PRNG - SFC64 */

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
* ---- TESTING ORDER ----
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
* 13)  apn_submul_one   - done
* 14)  apn_lshift       - done
* 15)  apn_rshift       - done
* 16)  apn_mul_n        - done
* 17)  apn_sqr          - done
* 18)  apn_mul          - done
* 19)  apn_div_one      - done
* 20)  apn_div
*
*/

static void check_apn_set(void)
{
    TEST_START("apn_set");

    apn_seg_t* op1 = NULL, * op2 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);

    printf("TEST-1: Comparison against memset\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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

    printf("TEST-1: Comparison using memcmp\n");

    apn_set(op1, TEST_SIZE_MAX, 0ULL);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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

    printf("TEST-1: Equality test\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-2: Single segment difference test\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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

    printf("TEST-1: All-zero input\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        int res = apn_is_zero(op, i);

        APAC_ALWAYS_ASSERT(
            res == 0,
            "apn_is_zero() all-zero test failed!\n"
            "\t Operand length tested        : %" PRI_APN_SIZE "\n"
            "\t Actual result                : %i\n",
            (apn_size_t)i,
            res
        );
    }

    printf("TEST-2: Single non-zero segment\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
	printf("TEST-1: Full Carry propagation\n");

	apn_set(op1, TEST_SIZE_MAX, APN_SEG_MAX);
	apn_set(op2, TEST_SIZE_MAX, 0x00ULL);

	for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
	{
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
	}

	/* TEST-2 Adding Identity Element */
	printf("TEST-2: Adding Identity Element\n");

	apn_set(op2, TEST_SIZE_MAX, 0x00ULL);

	for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
	{
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
	}

	/* TEST-3 No Carry Test */
	printf("TEST-3: Monotonicity Test\n");

    apn_set(op1, TEST_SIZE_MAX, 0ULL);
    apn_set(op2, TEST_SIZE_MAX, 0ULL);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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

    printf("TEST-1: Adding identity element\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-2: Carry Out Test\n");

    apn_set(op1, TEST_SIZE_MAX, APN_SEG_MAX);
    apn_set(op2, TEST_SIZE_MAX, APN_SEG_MAX);
    op2[0] = APN_SEG_MAX - 1;

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-3: Monotonicity Test\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-4: Commutativity Test\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-5: Associativity Test\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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

    printf("TEST-1: Identity (a + 0 = a)\n");

    /* b = 0 for entire test */
    apn_set(b, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (apn_size_t j = 1; j <= i; j++)
        {
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
        }
    }

    printf("TEST-2: Correctness vs apn_add_n()\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-3: Full carry-out (all-ones operands)\n");

    apn_set(a, TEST_SIZE_MAX, APN_SEG_MAX);
    apn_set(b, TEST_SIZE_MAX, APN_SEG_MAX);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (apn_size_t j = 1; j <= i; j++)
        {
            apn_set(t1, j, APN_SEG_MAX);
            t1[0] = APN_SEG_MAX - 1;

            if (i > j)
            {
                apn_set(t1 + j, i - j, 0);
            }
            
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

    printf("TEST-1: a + (-a) == 0 with carry\n");

    for (apn_size_t size = 1; size <= TEST_SIZE_MAX; size++)
    {
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
    printf("TEST-1: Full Borrow propagation\n");

    apn_set(op1, TEST_SIZE_MAX, 0x00ULL);
    apn_set(op2, TEST_SIZE_MAX, 0x00ULL);
    apn_set(op3, TEST_SIZE_MAX, APN_SEG_MAX);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    /* TEST-2: Subtracting Identity Element */
    printf("TEST-2: Subtracting Identity Element\n");

    apn_set(op2, TEST_SIZE_MAX, 0x00ULL);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    /* TEST-3: Monotonicity Test */
    printf("TEST-3: Monotonicity Test\n");

    apn_set(op1, TEST_SIZE_MAX, 0ULL);
    apn_set(op2, TEST_SIZE_MAX, 0ULL);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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

    printf("TEST-1: Subtracting Identity Element:\n");
    
    apn_set(op2, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-2: Full borrow propagation\n");

    apn_set(op2, TEST_SIZE_MAX, 0);
    apn_set(op1, TEST_SIZE_MAX, 0);
    op1[0] = 1;

    apn_set(op4, TEST_SIZE_MAX, APN_SEG_MAX);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-3: Self subtraction (a - a = 0)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-4: Borrow invariant (borrow == a < b)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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

    printf("TEST-1: Identity (a - 0 = a)\n");

    apn_set(b, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (apn_size_t j = 1; j <= i; j++)
        {
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
        }
    }

    printf("TEST-2: Borrow invariant (borrow == a < b)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (apn_size_t j = 1; j <= i; j++)
        {
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
        }
    }

    printf("TEST-3: Full borrow propagation (0 - 1)\n");

    apn_set(t, TEST_SIZE_MAX, APN_SEG_MAX);
    apn_set(a, TEST_SIZE_MAX, 0);
    apn_set(b, TEST_SIZE_MAX, 0);
    b[0] = 1;

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    printf("TEST-4: Self subtraction (a - a = 0)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
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
    }

    apac_free(t);
    apac_free(r);
    apac_free(b);
    apac_free(a);

    TEST_END("apn_sub");
}

static void check_apn_addmul_one(void)
{
    TEST_START("apn_addmul_one");

    apn_seg_t* op1 = NULL, * op2 = NULL, * op3 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX + 1);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX + 1);

    printf("TEST-1: Max Value * APN_SEG_MAX (full carry chain)\n");

    apn_set(op1, TEST_SIZE_MAX, APN_SEG_MAX);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        apn_set(op2, i + 1, 0);
        apn_set(op3, i + 1, APN_SEG_MAX);
        op3[i] = APN_SEG_MAX - 1;
        op3[0] = 1;

        apn_seg_t carry = apn_addmul_one(op2, op1, i, APN_SEG_MAX);

        APAC_ALWAYS_ASSERT(
            carry == 0,
            "apn_addmul_one() max value test failed!\n"
            "\t size  : %" PRI_APN_SIZE "\n"
            "\t carry : %" PRI_APN_SEG "\n",
            i,
            (apn_seg_t)carry
        );

        int cmp_res = apn_cmp(op3, op2, i + 1);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_addmul_one() max value test failed!\n"
            "\t size                : %" PRI_APN_SIZE "\n"
            "\t comparision result  : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-2: Identity Test (k = 1)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        set_to_random(op2, i + 1);
        apn_cpy(op3, op2, i + 1);

        apn_seg_t carry_ref = apn_add(op3, op3, op1, i + 1, i);

        apn_seg_t carry = apn_addmul_one(op2, op1, i, 1);

        APAC_ALWAYS_ASSERT(
            carry == carry_ref,
            "apn_addmul_one() identity test failed: carry mismatch!\n"
            "\t size  : %" PRI_APN_SIZE "\n"
            "\t carry : %" PRI_APN_SEG "\n"
            "\t ref   : %" PRI_APN_SEG "\n",
            i,
            (apn_seg_t)carry,
            (apn_seg_t)carry_ref
        );

        int cmp_res = apn_cmp(op3, op2, i + 1);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_addmul_one() identity test failed: result mismatch!\n"
            "\t size               : %" PRI_APN_SIZE "\n"
            "\t comparison result  : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-3: Multiplication with zero (k = 0)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        set_to_random(op2, i + 1);
        apn_cpy(op3, op2, i + 1);

        apn_seg_t carry = apn_addmul_one(op2, op1, i, 0);

        int cmp_res = apn_cmp(op2, op3, i + 1);

        APAC_ALWAYS_ASSERT(
            carry == 0,
            "apn_addmul_one() zero-multiply test failed: unexpected carry!\n"
            "\t size  : %" PRI_APN_SIZE "\n"
            "\t carry : %" PRI_APN_SEG "\n",
            i,
            (apn_seg_t)carry
        );

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_addmul_one() zero-multiply test failed: destination modified!\n"
            "\t size               : %" PRI_APN_SIZE "\n"
            "\t comparison result  : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-4: Max Value * APN_SEG_MAX with non-zero destination\n");

    apn_set(op1, TEST_SIZE_MAX, APN_SEG_MAX);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        apn_set(op2, i + 1, APN_SEG_MAX);
        apn_set(op3, i + 1, APN_SEG_MAX);

        op3[0] = 0;
        op3[i] = APN_SEG_MAX - 1;

        apn_seg_t carry = apn_addmul_one(op2, op1, i, APN_SEG_MAX);

        APAC_ALWAYS_ASSERT(
            carry == 1,
            "apn_addmul_one() non-zero destination test failed: missing carry!\n"
            "\t size  : %" PRI_APN_SIZE "\n"
            "\t carry : %" PRI_APN_SEG "\n",
            i,
            (apn_seg_t)carry
        );

        int cmp_res = apn_cmp(op3, op2, i + 1);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_addmul_one() non-zero destination test failed!\n"
            "\t size               : %" PRI_APN_SIZE "\n"
            "\t comparison result  : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-5: Monotonicity (op2 + op1 * k >= op2)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);

        uint64_t val = 0;
        do
        {
            val = random_sfc64();

        } while (val == 0);

        /* op2 initial */
        set_to_random(op2, i + 1);
        apn_cpy(op3, op2, i + 1);

        apn_seg_t carry = apn_addmul_one(op2, op1, i, val);

        int is_op1_zero = apn_is_zero(op1, i);
        int cmp_res = apn_cmp(op2, op3, i + 1);

        APAC_ALWAYS_ASSERT(
            is_op1_zero == 0 ? (cmp_res == 0)
            : (carry ? (cmp_res < 0) : (cmp_res > 0)),
            "apn_addmul_one() monotonicity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t op1_is_zero           : %d\n"
            "\t carry                 : %" PRI_APN_SEG "\n"
            "\t cmp(new, old)         : %d\n",
            i,
            is_op1_zero,
            (apn_seg_t)carry,
            cmp_res
        );
    }

    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_addmul_one");
}

static void check_apn_submul_one(void)
{
    TEST_START("apn_submul_one");

    apn_seg_t* op1 = NULL, * op2 = NULL;
    apn_seg_t* op3 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX + 1);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX + 1);

    printf("TEST-1: Max Value * APN_SEG_MAX subtracted from zero (full carry chain)\n");

    apn_set(op1, TEST_SIZE_MAX, APN_SEG_MAX);
    apn_set(op3, TEST_SIZE_MAX + 1, 0);
    op3[0] = APN_SEG_MAX;

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        op3[i] = 1;
        apn_set(op2, i + 1, 0);

        apn_seg_t borrow = apn_submul_one(op2, op1, i, APN_SEG_MAX);
        int cmp_res = apn_cmp(op3, op2, i + 1);

        APAC_ALWAYS_ASSERT(
            borrow == 1,
            "apn_submul_one() test failed: unexpected carry!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t borrow                : %" PRI_APN_SEG "\n",
            i,
            (apn_seg_t)borrow
        );

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_submul_one() test failed: result mismatch!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(expected, result) : %d\n",
            i,
            cmp_res
        );

        op3[i] = 0;
    }

    printf("TEST-2: Test against apn_addmul_one()\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        set_to_random(op3, i + 1);
        apn_cpy(op2, op3, i + 1);

        uint64_t val = 0;
        do
        {
            val = random_sfc64();

        } while (val == 0);

        apn_seg_t carry1 = apn_addmul_one(op2, op1, i, val);
        apn_seg_t carry2 = apn_submul_one(op2, op1, i, val);

        int cmp_res = apn_cmp(op2, op3, i + 1);
    
        APAC_ALWAYS_ASSERT(
            carry1 == carry2,
            "apn_submul_one() vs apn_addmul_one() test failed: carry mismatch!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t carry_addmul          : %" PRI_APN_SEG "\n"
            "\t carry_submul          : %" PRI_APN_SEG "\n",
            i,
            (apn_seg_t)carry1,
            (apn_seg_t)carry2
        );

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_submul_one() vs apn_addmul_one() test failed: result mismatch!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(op2, original)    : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-3: Multiply with 1 (op2 -= op1)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        set_to_random(op2, i + 1);
        apn_cpy(op3, op2, i + 1);

        /* Reference: op3 = op3 - op1 */
        apn_seg_t borrow_ref = apn_sub(op3, op3, op1, i + 1, i);

        /* Under test */
        apn_seg_t borrow = apn_submul_one(op2, op1, i, 1);

        APAC_ALWAYS_ASSERT(
            borrow == borrow_ref,
            "apn_submul_one() k=1 test failed: borrow mismatch!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t borrow_submul         : %" PRI_APN_SEG "\n"
            "\t borrow_sub            : %" PRI_APN_SEG "\n",
            i,
            (apn_seg_t)borrow,
            (apn_seg_t)borrow_ref
        );

        int cmp_res = apn_cmp(op2, op3, i + 1);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_submul_one() k=1 test failed: result mismatch!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(op2, expected)    : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-4: Multiply with 0 (no-op)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        set_to_random(op2, i + 1);
        apn_cpy(op3, op2, i + 1);

        apn_seg_t borrow = apn_submul_one(op2, op1, i, 0);

        APAC_ALWAYS_ASSERT(
            borrow == 0,
            "apn_submul_one() k=0 test failed: unexpected borrow!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t borrow                : %" PRI_APN_SEG "\n",
            i,
            (apn_seg_t)borrow
        );

        int cmp_res = apn_cmp(op2, op3, i + 1);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_submul_one() k=0 test failed: destination modified!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(op2, original)    : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-5: Monotonicity (op2 - op1 * k <= op2)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);

        uint64_t val = 0;
        do
        {
            val = random_sfc64();

        } while (val == 0);

        set_to_random(op2, i + 1);
        apn_cpy(op3, op2, i + 1);

        apn_seg_t borrow = apn_submul_one(op2, op1, i, val);

        int is_op1_zero = apn_is_zero(op1, i);
        int cmp = apn_cmp(op2, op3, i + 1);

        APAC_ALWAYS_ASSERT(
            is_op1_zero == 0 ? (cmp == 0)
            : (borrow ? (cmp >= 0) : (cmp <= 0)),
            "apn_submul_one() monotonicity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t op1_is_zero           : %d\n"
            "\t borrow                : %" PRI_APN_SEG "\n"
            "\t cmp(new, old)         : %d\n",
            i,
            is_op1_zero,
            (apn_seg_t)borrow,
            cmp
        );
    }

    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_submul_one");
}

static void check_apn_lshift(void)
{
    TEST_START("apn_lshift");

    apn_seg_t* op1 = NULL, * op2 = NULL, * op3 = NULL;
    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);

    // (TEST_SIZE_MAX + 1) for test against apn_addmul_one
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX + 1);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX + 1);

    printf("TEST-1: Shift zero value (1..63 bits)\n");

    apn_set(op1, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (uint32_t sh = 1; sh <= APN_SEG_BITS - 1; sh++)
        {
            apn_set(op2, i + 1, 0);

            apn_seg_t carry = apn_lshift(op2, op1, i, sh);
            int cmp_res = apn_cmp(op2, op1, i);

            APAC_ALWAYS_ASSERT(
                carry == 0,
                "apn_lshift() zero-input test failed: non-zero carry!\n"
                "\t Operand length tested : %" PRI_APN_SIZE "\n"
                "\t Shift amount          : %u\n"
                "\t Carry                 : %" PRI_APN_SEG "\n",
                i,
                sh,
                carry
            );

            APAC_ALWAYS_ASSERT(
                cmp_res == 0,
                "apn_lshift() zero-input test failed: result mismatch!\n"
                "\t Operand length tested : %" PRI_APN_SIZE "\n"
                "\t Shift amount          : %u\n"
                "\t cmp(result, zero)     : %d\n",
                i,
                sh,
                cmp_res
            );
        }
    }

    printf("TEST-2: Shift random values\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (uint32_t sh = 1; sh <= APN_SEG_BITS - 1; sh++)
        {
            set_to_random(op1, i);
            apn_set(op2, i + 1, 0);

            apn_seg_t carry = apn_lshift(op2, op1, i, sh);

            apn_seg_t expected_carry = (op1[i - 1] >> (APN_SEG_BITS - sh));

            APAC_ALWAYS_ASSERT(
                carry == expected_carry,
                "apn_lshift() all-ones test failed!\n"
                "\t Operand length tested : %" PRI_APN_SIZE "\n"
                "\t Shift amount          : %u\n"
                "\t Expected carry        : %" PRI_APN_SEG "\n"
                "\t Actual carry          : %" PRI_APN_SEG "\n",
                i,
                sh,
                expected_carry,
                carry
            );
        }
    }

    printf("TEST-3: Test against apn_addmul_one()\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (uint32_t sh = 1; sh <= APN_SEG_BITS - 1; sh++)
        {
            set_to_random(op1, i);
            apn_set(op2, i + 1, 0);
            apn_set(op3, i + 1, 0);

            op2[i] = apn_lshift(op2, op1, i, sh);

            apn_addmul_one(op3, op1, i, 1ULL << sh);

            int cmp_res = apn_cmp(op2, op3, i + 1);

            APAC_ALWAYS_ASSERT(
                cmp_res == 0,
                "apn_lshift() vs apn_addmul_one() test failed!\n"
                "\t Operand length tested : %" PRI_APN_SIZE "\n"
                "\t Shift amount          : %u\n"
                "\t cmp(op2, op3)         : %d\n",
                i,
                sh,
                cmp_res
            );
        }
    }

    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_lshift");
}

static void check_apn_rshift(void)
{
    TEST_START("apn_rshift");

    apn_seg_t* op1 = NULL, * op2 = NULL, * op3 = NULL;
    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX);

    printf("TEST-1: Shift out random values (1..63 bits)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);

        for (uint32_t sh = 1; sh <= APN_SEG_BITS - 1; sh++)
        {
            apn_cpy(op3, op1, i);
            apn_set(op2, i, 0);

            apn_seg_t shift_out_right = apn_rshift(op2, op1, i, sh);

            apn_seg_t expected_shift_out = op3[0] << (APN_SEG_BITS - sh);

            APAC_ALWAYS_ASSERT(
                shift_out_right == expected_shift_out,
                "apn_rshift() random-input test failed: shift-out mismatch!\n"
                "\t Operand length tested     : %" PRI_APN_SIZE "\n"
                "\t Shift amount              : %u\n"
                "\t Expected shift-out-right  : %" PRI_APN_SEG "\n"
                "\t Actual shift-out-right    : %" PRI_APN_SEG "\n",
                i,
                sh,
                expected_shift_out,
                shift_out_right
            );
        }
    }

    printf("TEST-2: Shift out all zeros (1..63 bits)\n");

    apn_set(op1, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        for (uint32_t sh = 1; sh <= APN_SEG_BITS - 1; sh++)
        {
            apn_set(op2, i, 0);

            apn_seg_t shift_out_right = apn_rshift(op2, op1, i, sh);
            int cmp_res = apn_cmp(op2, op1, i);

            APAC_ALWAYS_ASSERT(
                shift_out_right == 0,
                "apn_rshift() zero-input test failed: non-zero shift-out-right!\n"
                "\t Operand length tested   : %" PRI_APN_SIZE "\n"
                "\t Shift amount            : %u\n"
                "\t Shift-out-right         : %" PRI_APN_SEG "\n",
                i,
                sh,
                shift_out_right
            );

            APAC_ALWAYS_ASSERT(
                cmp_res == 0,
                "apn_rshift() zero-input test failed: result mismatch!\n"
                "\t Operand length tested : %" PRI_APN_SIZE "\n"
                "\t Shift amount          : %u\n"
                "\t cmp(result, zero)     : %d\n",
                i,
                sh,
                cmp_res
            );
        }
    }

    printf("TEST-3: Round-trip rshift + lshift + restore\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);

        for (uint32_t sh = 1; sh <= APN_SEG_BITS - 1; sh++)
        {
            /* Preserve original */
            apn_cpy(op3, op1, i);

            /* Step 1: right shift */
            apn_set(op2, i, 0);
            apn_seg_t shift_out_right = apn_rshift(op2, op1, i, sh);

            /* Step 2: left shift back */
            apn_lshift(op2, op2, i, sh);

            /* Step 3: restore shifted-out bits into LSW */
            op2[0] |= shift_out_right >> (APN_SEG_BITS - sh);

            int cmp_res = apn_cmp(op2, op3, i);

            APAC_ALWAYS_ASSERT(
                cmp_res == 0,
                "apn_rshift() round-trip test failed!\n"
                "\t Operand length tested : %" PRI_APN_SIZE "\n"
                "\t Shift amount          : %u\n"
                "\t cmp(reconstructed, original) : %d\n",
                i,
                sh,
                cmp_res
            );
        }
    }
    
    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_rshift");
}

static void check_apn_mul_n(void)
{
    TEST_START("apn_mul_n");

    apn_seg_t* op1 = NULL, * op2 = NULL;
    apn_seg_t* op3 = NULL, * op4 = NULL;
    apn_seg_t* op5 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX * 2);
    MALLOC_AND_CHECK(op4, TEST_SIZE_MAX * 2);
    MALLOC_AND_CHECK(op5, TEST_SIZE_MAX);

    printf("TEST-1: Compare against apn_addmul_one()\n");

    for (apn_size_t i = 1; i <= (apn_size_t)64; i++)
    {
        set_to_random(op1, i);
        set_to_random(op2, i);

        apn_set(op3, i * 2, 0);
        apn_set(op4, i * 2, 0);

        /* op3 = op1 * op2 */
        apac_err err_out = apn_mul_n(op3, op1, op2, i);

        if (err_out == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        for (apn_size_t k = 0; k < i; k++)
        {
            apn_seg_t carry = apn_addmul_one(&op4[k], op1, i, op2[k]);
        }

        int cmp_res = apn_cmp(op3, op4, i * 2);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_mul_n() vs apn_addmul_one() balanced test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(mul_n, addmul_one): %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-2: Max value multiplied by itself\n");

    apn_set(op1, TEST_SIZE_MAX, APN_SEG_MAX);
    apn_set(op2, TEST_SIZE_MAX, APN_SEG_MAX);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        apn_set(op4, i * 2, 0);

        // set value to which to compare
        apn_set(op3, i, 0);
        op3[0] = 1;
        apn_set(op3 + i, i, APN_SEG_MAX);
        op3[i] = APN_SEG_MAX - 1;

        apac_err err_out = apn_mul_n(op4, op1, op2, i);

        if (err_out == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        int cmp_res = apn_cmp(op3, op4, i * 2);
    
        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_mul_n() max-value self-multiply test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(expected, result) : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-3: Identity element (a * 1 == a)\n");

    apn_set(op2, TEST_SIZE_MAX, 0);
    op2[0] = 1;

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        apn_set(op3, i * 2, 0);

        apac_err err_out = apn_mul_n(op3, op1, op2, i);

        if (err_out == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        int cmp_res = apn_cmp(op3, op1, i);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_mul_n() identity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(a*1, a)           : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-4: Absorbing element (a * 0 == 0)\n");

    apn_set(op2, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        apn_set(op3, i * 2, 0);

        apac_err err_out = apn_mul_n(op3, op1, op2, i);

        if (err_out == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        int is_zero = apn_is_zero(op3, i * 2);

        APAC_ALWAYS_ASSERT(
            is_zero == 0,
            "apn_mul_n() absorbing test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t apn_is_zero(result)   : %d\n",
            i,
            is_zero
        );
    }

    printf("TEST-5: Commutativity (a * b == b * a)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        set_to_random(op2, i);

        apn_set(op3, i * 2, 0);
        apn_set(op4, i * 2, 0);

        apac_err err_out1 = apn_mul_n(op3, op1, op2, i);
        if (err_out1 == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        apac_err err_out2 = apn_mul_n(op4, op2, op1, i);
        if (err_out2 == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        int cmp_res = apn_cmp(op3, op4, i * 2);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_mul_n() commutativity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(a*b, b*a)         : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-6: Monotonicity\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        set_to_random(op2, i);
        set_to_random(op5, i);

        apn_set(op3, i * 2, 0);
        apn_set(op4, i * 2, 0);

        int cmp_res1 = apn_cmp(op5, op2, i);

        apac_err err_out1 = apn_mul_n(op3, op2, op1, i);
        if (err_out1 == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        apac_err err_out2 = apn_mul_n(op4, op5, op1, i);
        if (err_out2 == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        int cmp_res2 = apn_cmp(op4, op3, i * 2);

        APAC_ALWAYS_ASSERT(
            cmp_res1 == cmp_res2,
            "apn_mul_n() monotonicity test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(op5, op2)         : %d\n"
            "\t cmp(op5*op1, op2*op1) : %d\n",
            i,
            cmp_res1,
            cmp_res2
        );
    }

    apac_free(op5);
    apac_free(op4);
    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_mul_n");
}

static void check_apn_sqr(void)
{
    TEST_START("apn_sqr");

    apn_seg_t* op1 = NULL;
    apn_seg_t* op2 = NULL;
    apn_seg_t* op3 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX * 2);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX * 2);

    printf("TEST-1: Zero squared (0 * 0 = 0)\n");

    apn_set(op1, TEST_SIZE_MAX, 0);
    apn_set(op3, TEST_SIZE_MAX * 2, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        apn_set(op2, i * 2, 0);

        apac_err err_out = apn_sqr(op2, op1, i);

        if (err_out == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        int cmp_res = apn_cmp(op2, op3, i * 2);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_sqr() zero test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(0 * 0, 0)         : %d\n",
            i,
            cmp_res
        );
    }

    printf("TEST-2: Random values vs apn_mul_n()\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);

        apn_set(op2, i * 2, 0);
        apn_set(op3, i * 2, 0);

        apac_err err_out1 = apn_sqr(op2, op1, i);
        if (err_out1 == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        apac_err err_out2 = apn_mul_n(op3, op1, op1, i);
        if (err_out2 == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        int cmp_res = apn_cmp(op2, op3, i * 2);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_sqr() vs apn_mul_n() test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(sqr, mul_n)       : %d\n",
            i,
            cmp_res
        );
    }

    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_sqr");
}

static void check_apn_mul(void)
{
    TEST_START("apn_mul");

    apn_seg_t* op1 = NULL, * op2 = NULL;
    apn_seg_t* op3 = NULL, * op4 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op3, TEST_SIZE_MAX * 2);
    MALLOC_AND_CHECK(op4, TEST_SIZE_MAX * 2);

    printf("TEST-1: Compare against apn_addmul_one() (size2 <= 64)\n");

    for (apn_size_t size1 = 1; size1 <= (TEST_SIZE_MAX / 2); size1++)
    {
        for (apn_size_t size2 = 1; size2 <= size1; size2++)
        {
            set_to_random(op1, size1);
            set_to_random(op2, size2);

            apn_set(op3, size1 + size2, 0);
            apn_set(op4, size1 + size2, 0);

            /* op3 = op1 * op2 */
            apac_err err_out = apn_mul(op3, op1, op2, size1, size2);

            if (err_out == APAC_OOM)
            {
                APAC_LOG_ERR("Aborting test due to malloc failure!");
                abort();
            }

            // O(n * n) code, might be slow
            for (apn_size_t k = 0; k < size2; k++)
            {
                apn_seg_t carry = apn_addmul_one(&op4[k], op1, size1, op2[k]);
            }

            int cmp_res = apn_cmp(op3, op4, size1 + size2);

            APAC_ALWAYS_ASSERT(
                cmp_res == 0,
                "apn_mul() vs apn_addmul_one() test failed!\n"
                "\t Operand A length tested : %" PRI_APN_SIZE "\n"
                "\t Operand B length tested : %" PRI_APN_SIZE "\n"
                "\t cmp(mul, addmul_one)    : %d\n",
                size1,
                size2,
                cmp_res
            );
        }
    }

    printf("TEST-2: Multiplication with zero\n");

    apn_set(op2, TEST_SIZE_MAX, 0);

    for (apn_size_t size1 = 1; size1 <= (TEST_SIZE_MAX / 4); size1++)
    {
        for (apn_size_t size2 = 1; size2 <= size1; size2++)
        {
            set_to_random(op1, size1);
            apn_set(op3, size1 + size2, 0);

            apac_err err_out = apn_mul(op3, op1, op2, size1, size2);

            if (err_out == APAC_OOM)
            {
                APAC_LOG_ERR("Aborting test due to malloc failure!");
                abort();
            }

            int is_zero = apn_is_zero(op3, size1 + size2);

            APAC_ALWAYS_ASSERT(
                is_zero == 0,
                "apn_mul() absorbing test failed!\n"
                "\t Operand A length tested : %" PRI_APN_SIZE "\n"
                "\t Operand B length tested : %" PRI_APN_SIZE "\n"
                "\t apn_is_zero(result)     : %d\n",
                size1,
                size2,
                is_zero
            );
        }
    }

    printf("TEST-3: Identity element (multiply by 1)\n");

    apn_set(op2, TEST_SIZE_MAX, 0);
    op2[0] = 1;

    for (apn_size_t size1 = 1; size1 <= (TEST_SIZE_MAX / 4); size1++)
    {
        for (apn_size_t size2 = 1; size2 <= size1; size2++)
        {
            set_to_random(op1, size1);
            apn_set(op3, size1 + size2, 0);

            apac_err err_out = apn_mul(op3, op1, op2, size1, size2);
            
            if (err_out == APAC_OOM)
            {
                APAC_LOG_ERR("Aborting test due to malloc failure!");
                abort();
            }

            int cmp_res = apn_cmp(op3, op1, size1);

            APAC_ALWAYS_ASSERT(
                cmp_res == 0,
                "apn_mul() identity test failed!\n"
                "\t Operand length tested : %" PRI_APN_SIZE "\n"
                "\t cmp(a*1, a)           : %d\n",
                size1,
                cmp_res
            );
        }
    }

    printf("TEST-4: Random balanced multiplication vs apn_mul_n\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);
        set_to_random(op2, i);

        apn_set(op3, i * 2, 0);
        apn_set(op4, i * 2, 0);

        apac_err err_out1 = apn_mul(op3, op1, op2, i, i);
        if (err_out1 == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        apac_err err_out2 = apn_mul_n(op4, op1, op2, i);
        if (err_out2 == APAC_OOM)
        {
            APAC_LOG_ERR("Aborting test due to malloc failure!");
            abort();
        }

        int cmp_res = apn_cmp(op3, op4, i * 2);

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_mul() vs apn_mul_n() balanced random test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t cmp(mul, mul_n)       : %d\n",
            i,
            cmp_res
        );
    }

    apac_free(op4);
    apac_free(op3);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_mul");
}

static void check_apn_div_one(void)
{
    TEST_START("apn_div_one");

    apn_seg_t* op1 = NULL, * op2 = NULL;
    apn_seg_t* q1 = NULL;
    apn_seg_t* q2 = NULL;

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX + 1);
    MALLOC_AND_CHECK(q1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(q2, TEST_SIZE_MAX);

    printf("TEST-1: Division by powers of two vs apn_rshift (2^1 .. 2^63)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);

        for (uint32_t sh = 1; sh <= 63; sh++)
        {
            apn_set(q1, i, 0);
            apn_set(q2, i, 0);

            /* Reference: right shift */
            apn_seg_t shift_out_right = apn_rshift(q2, op1, i, sh);

            /* Test: divide by 2^sh */
            apn_seg_t divisor = ((apn_seg_t)1 << sh);
            apn_seg_t rem = apn_div_one(q1, op1, divisor, i);

            int cmp_q = apn_cmp(q1, q2, i);

            apn_seg_t expected_rem =
                shift_out_right >> (APN_SEG_BITS - sh);

            APAC_ALWAYS_ASSERT(
                cmp_q == 0,
                "apn_div_one() quotient mismatch vs apn_rshift!\n"
                "\t Operand length tested : %" PRI_APN_SIZE "\n"
                "\t Divisor power         : %u\n"
                "\t cmp(div, rshift)      : %d\n",
                i,
                sh,
                cmp_q
            );

            APAC_ALWAYS_ASSERT(
                rem == expected_rem,
                "apn_div_one() remainder mismatch vs apn_rshift!\n"
                "\t Operand length tested : %" PRI_APN_SIZE "\n"
                "\t Divisor power         : %u\n"
                "\t Expected remainder    : %" PRI_APN_SEG "\n"
                "\t Actual remainder      : %" PRI_APN_SEG "\n",
                i,
                sh,
                expected_rem,
                rem
            );
        }
    }

    printf("TEST-2: apn_div_one() inverse of apn_addmul_one()\n");

    apn_set(op2, TEST_SIZE_MAX + 1, 0);
    apn_set(op1, TEST_SIZE_MAX, 0);

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX; i++)
    {
        set_to_random(op1, i);

        apn_seg_t divisor = 0;
        do
        {
            divisor = random_sfc64();
        } while (divisor == 0);

        apn_set(q1, i, 0);
        apn_set(op2, i + 1, 0);

        apn_seg_t rem = apn_div_one(q1, op1, divisor, i);

        op2[0] += rem;
        apn_seg_t carry1 = apn_addmul_one(op2, q1, i, divisor);

        int cmp_res = apn_cmp(op1, op2, i);

        APAC_ALWAYS_ASSERT(
            carry1 == 0,
            "apn_div_one() inverse test failed: non-zero carry!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Divisor               : %" PRI_APN_SEG "\n"
            "\t Carry addmul_one      : %" PRI_APN_SEG "\n",
            i,
            divisor,
            carry1
        );

        APAC_ALWAYS_ASSERT(
            cmp_res == 0,
            "apn_div_one() inverse test failed!\n"
            "\t Operand length tested : %" PRI_APN_SIZE "\n"
            "\t Divisor               : %" PRI_APN_SEG "\n"
            "\t Remainder             : %" PRI_APN_SEG "\n"
            "\t cmp(q*d+r, original)  : %d\n",
            i,
            divisor,
            rem,
            cmp_res
        );

        apn_set(op1, i, 0);
    }

    apac_free(q2);
    apac_free(q1);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_div_one");
}

static void check_apn_div(void)
{
    TEST_START("apn_div");

    apn_seg_t* op1 = NULL; /* divisor: i */
    apn_seg_t* op2 = NULL; /* dividend: i + j */
    apn_seg_t* quot = NULL; /* quotient: j + 1 */
    apn_seg_t* rmdr = NULL; /* remainder: i */
    apn_seg_t* temp = NULL; /* temp: i + j */

    MALLOC_AND_CHECK(op1, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(op2, TEST_SIZE_MAX * 2);
    MALLOC_AND_CHECK(quot, TEST_SIZE_MAX + 1);
    MALLOC_AND_CHECK(rmdr, TEST_SIZE_MAX);
    MALLOC_AND_CHECK(temp, TEST_SIZE_MAX * 2);

    printf("TEST-1: apn_div identity (dividend = quot * divisor + remainder)\n");

    for (apn_size_t i = 1; i <= TEST_SIZE_MAX / 2; i++)
    {
        for (apn_size_t j = 1; j <= TEST_SIZE_MAX / 2; j++)
        {
            /* divisor (size i), ensure top limb non-zero */
            do {
                set_to_random(op1, i);
            } while (op1[i - 1] == 0);

            /* dividend (size i + j) */
            set_to_random(op2, i + j);

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
                i      /* divisor size  */
            );

            if (ret == APAC_OOM)
            {
                APAC_LOG_ERR("Aborting test due to malloc failure in apn_div!");
                abort();
            }

            /* decide multiplication order (quotient size is always j + 1) */
            apn_seg_t* mul_a;
            apn_seg_t* mul_b;
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

            if (ret == APAC_OOM)
            {
                APAC_LOG_ERR("Aborting test due to malloc failure in apn_mul!");
                abort();
            }

            /* temp += rmdr */
            apn_seg_t carry = apn_add(
                temp,
                temp,
                rmdr,
                i + j,
                i
            );

            APAC_ALWAYS_ASSERT(
                carry == 0,
                "apn_div test failed: carry after adding remainder\n"
                "  divisor size  = %" PRI_APN_SIZE "\n"
                "  dividend size = %" PRI_APN_SIZE "\n",
                i, (i + j)
            );

            /* reconstructed value must equal dividend */
            int cmp = apn_cmp(temp, op2, i + j);

            APAC_ALWAYS_ASSERT(
                cmp == 0,
                "apn_div failed: reconstruction mismatch\n"
                "  divisor size  = %" PRI_APN_SIZE "\n"
                "  dividend size = %" PRI_APN_SIZE "\n",
                i, (i + j)
            );

            /* remainder invariant */
            APAC_ALWAYS_ASSERT(
                apn_cmp(rmdr, op1, i) < 0,
                "apn_div failed: remainder >= divisor\n"
                "  divisor size  = %" PRI_APN_SIZE "\n"
                "  dividend size = %" PRI_APN_SIZE "\n",
                i, (i + j)
            );

            /* reset */
            apn_set(op1, i, 0);
            apn_set(op2, i + j, 0);
            apn_set(quot, j + 1, 0);
            apn_set(rmdr, i, 0);
            apn_set(temp, i + j, 0);
        }
    }

    apac_free(temp);
    apac_free(rmdr);
    apac_free(quot);
    apac_free(op2);
    apac_free(op1);

    TEST_END("apn_div");
}

int main(int argc, char** argv)
{
    apacInit();

    uint64_t seed = 0xC0FFEE; /* default seed */

    if (argc >= 2)
    {
        seed = strtoull(argv[1], NULL, 16);
    }

    printf("Using seed: 0x%" PRIX64 "\n", seed);
    random_sfc64_seed(seed);

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
    check_apn_addmul_one();
    check_apn_submul_one();
    check_apn_lshift();
    check_apn_rshift();
    check_apn_mul_n();
    check_apn_sqr();
    check_apn_mul();
    check_apn_div_one();
    check_apn_div();

    printf("\nALL TESTS PASSED!\n");
    return EXIT_SUCCESS;
}