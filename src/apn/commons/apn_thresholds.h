#ifndef APN_THRESHOLDS
#define APN_THRESHOLDS

#include "../../../include/apac.h"

extern __apac_cpu_params curr_cpu;

#define KARATSUBA_MUL_N_THRESHOLD	(curr_cpu.karatsuba_mul_n_threshold)
#define TOOM33_MUL_N_THRESHOLD		(curr_cpu.toom33_mul_n_threshold)

#endif
