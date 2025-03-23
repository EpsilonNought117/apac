#ifndef APAC_GETCPUSPEC_H
#define APAC_GETCPUSPEC_H

#include <intrin.h>
#include <stdint.h>
#include "../../apacExport.h"

// for getCPUSpec()

extern uint8_t adx_chk;
extern uint8_t bmi2_chk;
extern uint8_t avx2_chk;
extern uint8_t avx_chk;
extern uint8_t sse3_chk;

APAC_API void getCPUSpec(void);

#endif