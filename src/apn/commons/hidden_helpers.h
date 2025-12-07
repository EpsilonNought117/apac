#ifndef APN_HIDDEN_HELPERS
#define APN_HIDDEN_HELPERS

#include "../../../include/apac.h"

#if defined(_WIN32)
		
	#if defined(_MSC_VER)

		#if defined(_M_X64) || defined(_M_AMD64)

			#define CLZ64(value)	\
					({ uint32_t _idx; _BitScanReverse64(&_idx, (value)) ? (int)_idx : -1; })

		#elif defined(_M_ARM64) || defined(_M_ARM64EC)
		
		#else
			#error "Unsupported Architecture on Windows and MSVC!"	
		#endif
	#else

		#error "Unsupported Compiler on Windows!"

	#endif
	
#elif defined(__linux__) || defined(__linux)    ||  \
      defined(__unix__)  || defined(__unix)     ||  \
      defined(__APPLE__) || defined(__MACH__)

	#if defined(__GNUC__) || defined(__clang__)

		#define CLZ64(value)	\
				((value) ? (63 - __builtin_clzll(value)) : -1)

        #if defined(__x86_64)   || defined(__amd64)   || \
            defined(__x86_64__) || defined(__amd64__)

		
		#elif defined(__aarch64__) || defined(__arm64__)

		#else
			#error "Unsupported Architecture on Linux/Unix/MacOS and GCC/Clang!"
        #endif

    #else
        #error "Unknown Compiler on Linux/Unix/MacOS!"
    #endif

#else

	#error "Unsupported Platform!"

#endif

#endif