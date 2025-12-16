#include "apac.h"
#include <memory.h>

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

	#define GETRAND(value)																		\
			{																					\
				value = ((apn_seg_t)rand() << 45) | (rand() << 30) | (rand() << 15) | rand();	\
				value <<= 4;																	\
				value |= (apn_seg_t)rand() & 0xF;												\
			}

#elif defined(__GNUC__) || defined(__clang__)

	#define GETRAND(value)									\
			{												\
				value = ((apn_seg_t)rand() << 32) | rand();	\			
			}

#else
	#error "Unknown Compiler!"
#endif

/*
    || ------------------------------------- INTEGRATION-TESTS ------------------------------------- ||
*/

/*
* TESTING ORDER:
*
*  1)  apn_cmp - done
*  2)  apn_set - done
*  3)  apn_cpy - done
*  4)  apn_is_zero
*  5)  apn_add_one
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
	printf(
		"\n"
		"============================================================\n"
		"						TEST: apn_set                        \n"
		"============================================================\n"
	);

	apn_seg_t* op1 = NULL, * op2 = NULL;
	apn_size_t test_size = 256ULL;

	MALLOC_AND_CHECK(op1, test_size);
	MALLOC_AND_CHECK(op2, test_size);
	
	for (apn_size_t i = 1; i <= test_size; i++)
	{
		printf("Testing size: %llu ... ", i);

		uint8_t val1 = i & 255;
		uint64_t val2 = val1 * 0x0101010101010101ULL;

		memset(op1, val1, sizeof(apn_seg_t) * i);
		apn_set(op2, i, val2);
		
		int cmp_res = memcmp(op1, op2, sizeof(apn_seg_t) * i);

		APAC_ALWAYS_ASSERT(
			cmp_res == 0,
			"apn_set() memcmp test failed!\n"
			"\t Operand length tested		: %llu\n"
			"\t Expected comparison result	: 0\n"
			"\t Actual comparison result	: %i\n",
			i,
			cmp_res
		);

		printf("PASSED\n");
	}

	printf(
		"============================================================\n"
		"						ALL TESTS PASSED!                    \n"
		"============================================================\n\n"
	);

	apac_free(op2);
	apac_free(op1);
	return;
}

static void check_apn_cmp(void)
{
	printf(
		"\n"
		"============================================================\n"
		"						TESTS: apn_cmp                       \n"
		"============================================================\n"
	);

	apn_seg_t* op1 = NULL, * op2 = NULL;
	apn_size_t test_size = 512ULL;

	MALLOC_AND_CHECK(op1, test_size);
	MALLOC_AND_CHECK(op2, test_size);

	printf("\n\tTEST-1: Equality test\n\n");

	for (apn_size_t i = 1; i <= test_size; i++)
	{
		printf("\tTesting size: %llu ... ", i);

		uint64_t val = 0;
		GETRAND(val);

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

	printf("\n\tTEST-2: Inequality test\n\n");

	for (apn_size_t i = 1; i <= test_size; i++)
	{
		printf("\tTesting size: %llu ... ", i);

		uint64_t val = 0;
		GETRAND(val);

		/* Start equal */
		apn_set(op1, i, val);
		apn_set(op2, i, val);

		/* Pick where to introduce a difference */
		uint64_t pick = 0;
		GETRAND(pick);

		apn_size_t pos = 0;
		switch (pick % 8)
		{
		case 0:  pos = 0;               break;  /* LSD */
		case 1:  pos = i / 8;           break;
		case 2:  pos = i / 4;           break;
		case 3:  pos = (3 * i) / 8;     break;
		case 4:  pos = i / 2;           break;
		case 5:  pos = (5 * i) / 8;     break;
		case 6:  pos = (3 * i) / 4;     break;
		default: pos = i - 1;           break;  /* MSD */
		}

		/* Generate a different value */
		uint64_t diff;
		do { GETRAND(diff); } while (diff == val);

		/* Randomly choose which operand differs */
		uint64_t dir = 0;
		GETRAND(dir);

		if (dir & 1)
			op1[pos] = diff;
		else
			op2[pos] = diff;

		int compare_result = apn_cmp(op1, op2, i);

		/* Determine where the difference actually is */
		int diff_in_op1 = (op1[pos] == diff);

		/* Compute expected result from facts */
		int expected_cmp;
		if (diff > val)
			expected_cmp = diff_in_op1 ? 1 : -1;
		else /* diff < val */
			expected_cmp = diff_in_op1 ? -1 : 1;

		APAC_ALWAYS_ASSERT(
			compare_result == expected_cmp,
			"apn_cmp() ordering test failed!\n"
			"\t Operand length tested        : %llu\n"
			"\t Difference position          : %llu\n"
			"\t diff value                  : 0x%016llx\n"
			"\t base value                  : 0x%016llx\n"
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

	printf(
		"============================================================\n"
		"						ALL TESTS PASSED!                    \n"
		"============================================================\n\n"
	);

	apac_free(op2);
	apac_free(op1);
	return;
}

static void check_apn_cpy(void)
{
	printf(
		"\n"
		"============================================================\n"
		"                      TEST: apn_cpy                         \n"
		"============================================================\n"
	);

	apn_seg_t* op1 = NULL, * op2 = NULL;
	apn_size_t test_size = 512ULL;

	MALLOC_AND_CHECK(op1, test_size);
	MALLOC_AND_CHECK(op2, test_size);

	for (apn_size_t i = 1; i <= test_size; i++)
	{
		printf("\tTesting size: %llu ... ", i);

		uint64_t val = 0;
		GETRAND(val);

		/* Initialize source */
		apn_set(op1, i, val);

		/* Copy */
		apn_cpy(op2, op1, i);

		/* Compare */
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

		printf("PASSED\n");
	}

	printf(
		"============================================================\n"
		"                    ALL TESTS PASSED!                       \n"
		"============================================================\n\n"
	);

	apac_free(op2);
	apac_free(op1);
}

//static void check_apn_add_one(void)
//{
//	apn_seg_t* op1 = NULL, * op2 = NULL, * op3 = NULL;
//	apn_size_t test_size = 256ULL;
//
//	MALLOC_AND_CHECK(op1, test_size);
//	MALLOC_AND_CHECK(op2, test_size);
//	MALLOC_AND_CHECK(op3, test_size);
//
//	/* TEST 1 - Carry propagation test */
//
//	apn_set(op1, test_size, 0xFFFFFFFFFFFFFFFFULL);
//
//	for (apn_size_t i = 1; i <= test_size; i++)
//	{
//		apn_seg_t carry_out = apn_add_one(op2, op1, i, 1ULL);
//
//		APAC_ALWAYS_ASSERT(
//			carry_out == 1,
//			"apn_add_one() carry propagation test failed!\n"
//			"\t Operand length tested      : %llu\n"
//			"\t Expected carry value       : 1\n"
//			"\t Actual carry value         : %llu\n",
//			i,
//			carry_out
//		);
//
//		int cmp_val = apn_is_zero(op2, i);
//
//		APAC_ALWAYS_ASSERT(
//			cmp_val == 1,
//			"apn_add_one() carry propagation test failed!\n"
//			"\t Operand length tested      : %llu\n"
//			"\t Expected result (is_zero)  : 0 (result array should be all zeros)\n"
//			"\t Actual result (is_zero)    : %d\n",
//			i,
//			test_size,
//			cmp_val
//		);
//	}
//
//	/* Test 2 */
//
//	apn_set(op2, test_size, 0x00ULL);
//
//	for (apn_size_t i = 1; i <= test_size; i++)
//	{
//
//	}
//
//	apac_free(op3);
//	apac_free(op2);
//	apac_free(op1);
//
//	return;
//}

int main(void)
{
	apacInit();
	srand(0x117);


	return 0;
}

