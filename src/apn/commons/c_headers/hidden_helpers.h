#ifndef APN_HIDDEN_HELPERS
#define APN_HIDDEN_HELPERS

#include "../../../../include/apac.h"

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

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

	#define ROTL(value, count)	do { value = _rotl64((value), (count)); } while (0)

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

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

	#define ROTL(value, count)									\
	do															\
	{															\
		value = (												\
			(((uint64_t)(value)) << ((count) & 63)) 	   	| 	\
			(((uint64_t)(value)) >> (64 - ((count) & 63)))		\
		);														\
	} while (0)

#else
	#error "Unknown Compiler, OS Platform and CPU ISA!"
#endif

#endif