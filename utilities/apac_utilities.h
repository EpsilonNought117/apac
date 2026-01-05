#ifndef APAC_UTILITIES_H
#define APAC_UTILITIES_H

#include "../include/apac.h"

#if defined(_WIN32) && defined(_MSC_VER)

	#include <Windows.h>
	#include <intrin.h>
    #include <powrprof.h>
    #pragma comment(lib, "powrprof")

	#define ROTL64(x, k) (_rotl64((x), (k)))

#elif defined(__GNUC__) || defined(__clang__)

	#define ROTL64(x, k) (((uint64_t)(x) << ((k) & 63)) | ((uint64_t)(x) >> (64 - ((k) & 63))))

#else

	#error "Unknown Compiler!"

#endif

#define TEST_START(name) printf("\nTEST START: %s()\n", name)

#define TEST_END(name) printf("TEST END: %s()\n", name);

uint64_t cpu_timer(void);

uint64_t os_timer(void);

uint64_t random_sfc64(void);

void random_sfc64_seed(uint64_t seed);

void set_to_random(apn_seg_t* op1, apn_size_t size);

int pin_curr_thread_to_core(uint32_t core_id);

void disable_turbo_boost(void);

void restore_turbo_boost(void);

#endif