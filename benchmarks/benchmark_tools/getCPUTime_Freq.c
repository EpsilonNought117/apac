#include "benchmark_tools.h"
#include <Windows.h>

int64_t getCPUTime(void)
{
    int64_t cpu_time = 0;

    QueryPerformanceCounter((LARGE_INTEGER*)(void*)&cpu_time);
    
    return cpu_time;
}

int64_t getCPUFreq(void)
{
    static int64_t cpu_freq = 0;

    if (cpu_freq == 0)
    {
        QueryPerformanceFrequency((LARGE_INTEGER*)(void*)&cpu_freq);
    }
    
    return cpu_freq;
}