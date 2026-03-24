#ifndef APN_HIDDEN_HELPERS
#define APN_HIDDEN_HELPERS

#include "../../../../include/apac.h"

#if defined(_MSC_VER)

	#define CLZ(value, count)																	\
			do																					\
			{																					\
				uint32_t idx = 0;																\
				(count) = _BitScanReverse64(&idx, (value)) ? (uint32_t)63 - idx : (uint32_t)64;	\
			} while (0)

	#define CTZ(value, count)														\
			do																		\
			{																		\
				uint32_t idx = 0;													\
				(count) = _BitScanForward64(&idx, (value)) ? idx : (uint32_t)64;	\
			} while (0)

#elif defined(__GNUC__) || defined(__clang__)

	#define CLZ(value, count)															\
			do																			\
			{																			\
				(count) = (value) ? (uint32_t)__builtin_clzll((value)) : (uint32_t)64;	\
			} while (0)

	#define CTZ(value, count)															\
			do																			\
			{																			\
				(count) = (value) ? (uint32_t)__builtin_ctzll((value)) : (uint32_t)64;	\
			} while(0)

#else
	#error "Unknown Compiler!"
#endif

#endif