#ifndef APN_THRESHOLDS
#define APN_THRESHOLDS

#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

#define KARATSUBA_MUL_BALANCED_THRESHOLD	(curr_cpu.karatsuba_mul_balanced_threshold)
#define KARATSUBA_MUL_UNBALANCED_THRESHOLD	(curr_cpu.karatsuba_mul_unbalanced_threshold)
#define KARATSUBA_SQR_THRESHOLD				(curr_cpu.karatsuba_sqr_threshold)
#define DNC_DIV_THRESHOLD					(curr_cpu.dnc_div_threshold)

#endif
