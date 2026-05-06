#include "../../headers/apac_internal.h"

#if defined(APAC_64BIT_PLATFORM)

APAC_THREAD_LOCAL uint64_t prng_state[4] = { 0 };
APAC_THREAD_LOCAL bool state_seed_init = false;

static inline uint64_t random_sfc64(void)
{
	uint64_t out = prng_state[1] + prng_state[2] + prng_state[0];

	prng_state[0]++;
	prng_state[1] = prng_state[2] ^ (prng_state[2] >> 11);
	prng_state[2] = prng_state[3] + (prng_state[3] << 3);
    
    uint64_t temp = prng_state[3];
    ROTL(temp, 24);
	
    prng_state[3] = temp + out;
	
    return out;
}

static inline void seed_random_sfc64(uint64_t seed)
{
	prng_state[0] = 1;
	prng_state[1] = seed;
	prng_state[2] = seed;
	prng_state[3] = seed;

	for (size_t i = 0; i < (size_t)16; i++){ random_sfc64(); }
}

#endif

void apn_set_to_random(
    ap_dig_t* op1, 
    ap_size_t size,
	ap_dig_t seed_val
)
{
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(size != 0);

    if (!state_seed_init) { seed_random_sfc64(seed_val); }

	for (ap_size_t i = 0; i < size; i++)
	{
		op1[i] = random_sfc64();
	}
}