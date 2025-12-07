#ifndef APN_THRESHOLDS
#define APN_THRESHOLDS

#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

#define KARATSUBA_MUL_BALANCED_THRESHOLD		(curr_cpu.karatsuba_mul_balanced_threshold)
#define KARATSUBA_MUL_UNBALANCED_THRESHOLD		(curr_cpu.karatsuba_mul_unbalanced_threshold)
#define KARATSUBA_SQR_THRESHOLD					(curr_cpu.karatsuba_sqr_threshold)
#define REC_DNC_DIV_REM_THRESHOLD				(curr_cpu.rec_dnc_div_rem_threshold)

#endif
