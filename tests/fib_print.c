#include "../include/apac.h"
#include <memory.h>
#include <stdio.h>

#define LOG_2_PHI	(double)0.694241913
#define LOG_2_SQRT5	(double)1.160964047

static void fib_print(u64 fib_idx)
{
	printf("\nFibonacci Number Index: %llu\n\n", fib_idx);

	u64* fib[2] = { NULL };

	u64 size = ((u64)((double)fib_idx * LOG_2_PHI - LOG_2_SQRT5) >> 6) + 1ULL;

	fib[0] = apac_malloc(sizeof(u64) * size);
	fib[1] = apac_malloc(sizeof(u64) * size);

	memset(fib[0], 0, sizeof(u64) * size);
	memset(fib[1], 0, sizeof(u64) * size);

	fib[0][0] = 1ULL, fib[1][0] = 1ULL; // f_0 = 1, f_1 = 1

	for (u64 i = 2; i <= fib_idx; i++)
	{
		apn_add_n(fib[i & 1], fib[1], fib[0], size);
	}

	u64 to_print = (fib_idx - 1) & 1;

	printf("The Number (in Hex): ");
	for (u64 i = size - 1; i < size; i--)
	{
		printf("%llx", fib[to_print][i]);
	}

	printf("\n");

	apac_free(fib[0]);
	apac_free(fib[1]);
}

int main(void)
{
	pinThreadToCoreZero();
	apacInitDefault();

	u64 val = 0;

	printf("\n\nEnter the Index of the fibonacci Number: ");
	scanf("%llu", &val);

	fib_print(val);

	return 0;
}