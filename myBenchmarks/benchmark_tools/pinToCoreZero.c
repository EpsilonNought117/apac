#include "benchmarkTools.h"
#include <Windows.h>
#include <stdio.h>

#define FIRST_CORE_MASK 0x01ULL

void pinThreadToCoreZero(void)
{
	HANDLE curr_thread = GetCurrentThread();

	if (!SetThreadAffinityMask(curr_thread, (DWORD_PTR)FIRST_CORE_MASK))
	{
		fprintf(stderr, "Failed to pin the thread to core zero!");
	}

	return;
}