#ifndef APAC_INTERNAL_H
#define APAC_INTERNAL_H

#include "../../include/apac.h"

/****************************************************************************************************/
/****************************         USEFUL FUNCTION-LIKE MACROS        ****************************/
/****************************************************************************************************/

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

	#define CLZ(value, count)																	\
			do																					\
			{																					\
				unsigned long idx = 0;															\
				(count) = _BitScanReverse64(&idx, (value)) ? (ap_dig_t)63 - idx : (ap_dig_t)64;	\
			} while (0)

	#define CTZ(value, count)														\
			do																		\
			{																		\
				unsigned long idx = 0;												\
				(count) = _BitScanForward64(&idx, (value)) ? idx : (ap_dig_t)64;	\
			} while (0)

	#define ROTL(value, count)	do { value = _rotl64((value), (count)); } while (0)

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

	#define CLZ(value, count)															\
			do																			\
			{																			\
				(count) = (value) ? (ap_dig_t)__builtin_clzll((value)) : (ap_dig_t)64;	\
			} while (0)

	#define CTZ(value, count)															\
			do																			\
			{																			\
				(count) = (value) ? (ap_dig_t)__builtin_ctzll((value)) : (ap_dig_t)64;	\
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