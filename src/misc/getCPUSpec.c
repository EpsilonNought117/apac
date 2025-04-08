#include "../../include/apac.h"

uint8_t adx_chk = 0;
uint8_t bmi2_chk = 0;
uint8_t avx2_chk = 0;
uint8_t avx_chk = 0;
uint8_t avx512f_chk = 0;

void getCPUSpec(void)
{
    int specBuffer[4] = { 0 };

    __cpuid(specBuffer, 0x0);

    if (specBuffer[0] >= 7)
    {
        __cpuidex(specBuffer, 7, 0);

        adx_chk = (uint8_t)((specBuffer[1] & (1 << 19)) >> 19);
        bmi2_chk = (uint8_t)((specBuffer[1] & (1 << 8)) >> 8);
        avx2_chk = (uint8_t)((specBuffer[1] & (1 << 5)) >> 5);
        avx512f_chk = (uint8_t)((specBuffer[1] & (1 << 16)) >> 16);
    }
    
    __cpuid(specBuffer, 0x1);
    avx_chk = (uint8_t)((specBuffer[2] & (1 << 28)) >> 28);
    
    return;
}