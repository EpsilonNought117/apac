#include "../include/apac.h"
#include <memory.h>
#include <stdio.h>

#define LOG_2_PHI	(double)0.694241913
#define LOG_2_SQRT5	(double)1.160964047

static void fib_test(u64 fib_idx, uint64_t run_count)
{
	APAC_ASSERT(fib_idx >= 2);

	printf("\nFibonacci Number Index: %llu\nRun Count: %lli\n", fib_idx, run_count);

	u64* fib[2] = { NULL };

	u64 size = ((u64)((double)fib_idx * LOG_2_PHI - LOG_2_SQRT5) >> 6) + 1ULL;

	fib[0] = apac_malloc(sizeof(u64) * size);
	fib[1] = apac_malloc(sizeof(u64) * size);

	memset(fib[0], 0, sizeof(u64) * size);
	memset(fib[1], 0, sizeof(u64) * size);

	fib[0][0] = 1ULL, fib[1][0] = 1ULL; // f_0 = 1, f_1 = 1

	int64_t total_time = 0;

	for (uint64_t j = 0; j < run_count; j++)
	{
		int64_t before = getCPUTime();

		for (u64 i = 2; i <= fib_idx; i++)
		{
			u64 curr_size = ((u64)((double)i * LOG_2_PHI - LOG_2_SQRT5) >> 6) + 1ULL;
			apn_add_n(fib[i & 1], fib[1], fib[0], curr_size);
		}

		int64_t after = getCPUTime();

		double run_jth = (double)(after - before) / getCPUFreq();

		printf("\tRun Number: %llu\n\tRun Time: %0.3fs\n", j + 1, run_jth);

		total_time += (after - before);

		memset(fib[0], 0, sizeof(u64) * size);
		memset(fib[1], 0, sizeof(u64) * size);

		fib[0][0] = 1ULL, fib[1][0] = 1ULL;
	}

	apac_free(fib[0]);
	apac_free(fib[1]);

	double avg_runtime = (double)total_time / (run_count * getCPUFreq());

	printf("\tAverage Time per Run: %0.3fs\n", avg_runtime);

	return;
}

int main(void)
{
	pinThreadToCoreZero();
	apacInitDefault();

	DisableTurboBoost();

	fib_test(10ULL * 1000 * 1000, 1);

	RestoreTurboBoost();

	return 0;
}