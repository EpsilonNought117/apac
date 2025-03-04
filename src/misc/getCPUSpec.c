#include "misc.h"

uint8_t adx_chk = 0;
uint8_t bmi2_chk = 0;

void getCPUSpec(void)
{
    int specBuffer[4] = { 0 };

    __cpuid(specBuffer, 0x0);

    if (specBuffer[0] >= 7)
    {
        __cpuidex(specBuffer, 7, 0);

        adx_chk = (specBuffer[1] & (1 << 19)) >> 19;
        bmi2_chk = (specBuffer[1] & (1 << 8)) >> 8;
    }
    else
    {
        __cpuid(specBuffer, 0x1);


    }
    
    
    return;
}