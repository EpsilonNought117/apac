#ifndef APN_THRESHOLDS
#define APN_THRESHOLDS

#include "../../../include/apac.h"

extern __apac_cpu_params curr_cpu;

#define KARATSUBA_MUL_N_THRESHOLD	(curr_cpu.karatsuba_mul_n_threshold)
#define KARATSUBA_MUL_THRESHOLD		(curr_cpu.karatsuba_mul_threshold)		

#endif
