#ifndef BENCHMARK_TOOLS_H
#define BENCHMARK_TOOLS_H

#include <stdint.h>

void RestoreTurboBoost(void);

void DisableTurboBoost(void);

int64_t getCPUTime(void);

int64_t getCPUFreq(void);

void pinThreadToCoreZero(void);

#endif