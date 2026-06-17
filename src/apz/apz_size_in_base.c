#include "../header/apac_internal.h"

ap_size_t
apz_size_in_base(
    const apz_t* op1,
    apac_str_base base
)
{
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(base == BASE10 || base == BASE16);
    APAC_ASSERT(op1->curr_size <= APN_SIZE_MAX);

    if (op1->curr_size == 0)
    {
        return 0;
    }

    ap_size_t size = 0;
    ap_size_t bitcnt = 0;

    CLZ(op1->num[op1->curr_size - 1], bitcnt);
    
    bitcnt = (ap_size_t)64 - bitcnt;
    bitcnt += (op1->curr_size - 1) * APN_DIG_BITS;

    // now we know op1->num <= 2^bitcnt - 1

    if (base == BASE10)
    {
        // Let L = log10(2).
        //
        // B = floor(L * 2^64)
        // C = floor((L * 2^64 - B) * 2^64)
        //
        // Together, B and C form a 128-bit fixed-point approximation of L:
        //
        //     L ≈ B/2^64 + C/2^128
        //
        // The high product bitcnt * B provides the primary estimate, while
        // bitcnt * C supplies enough additional precision to decide whether
        // the final ceil() result requires an extra increment.

        static const ap_size_t B = 5553023288523357132ULL;
        static const ap_size_t C = 5171448307347507388ULL;

#if defined(APAC_X64_WIN) || defined(APAC_ARM64_WIN)

        ap_size_t hi = __umulh(B, bitcnt);
        ap_size_t lo = B * bitcnt;
        ap_size_t D = __umulh(C, bitcnt);

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)
        
        __uint128_t prod = (__uint128_t)B * bitcnt;
        ap_size_t hi = (prod >> 64);
        ap_size_t lo = prod;

        ap_size_t D = ((__uint128_t)C * bitcnt) >> 64;

#else
    #error "Unsupported Platform!"
#endif

        size = hi + 1 + (D > ~lo);
    }
    else
    {
        size = (bitcnt + 3) / 4;
    }

    return size;
}