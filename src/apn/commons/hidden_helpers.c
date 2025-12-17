#include "hidden_helpers.h"

inline uint32_t clz64(uint64_t value)
{
	uint32_t out_val = 0;

#if defined(_MSC_VER)

	out_val = (_BitScanReverse64(&out_val, value) ? ((uint32_t)63 - out_val) : 64);

#elif defined(__GNUC__) || defined(__clang__)

	out_val = (value ? __builtin_clzll(value) : 64);

#else

	#error "Unknown Compiler"

#endif

	return out_val;
}