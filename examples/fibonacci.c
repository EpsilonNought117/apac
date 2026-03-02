#include "../include/apac.h"
#include <math.h>
#include <errno.h>

#define LOG_2_PHI    0.694241913   /* log2(phi) */
#define LOG_2_SQRT5  1.160964047   /* log2(sqrt(5)) */

int main(int argc, char** argv)
{
    /* ------------------------------------------------------------
     * Argument parsing / validation (UNCHANGED)
     * ------------------------------------------------------------ */
    if (argc != 2)
    {
        fprintf(stderr,
            "Usage: %s <N>\n"
            "  <N> must be a non-negative integer (Fibonacci index)\n",
            argv[0]);
        return EXIT_FAILURE;
    }

    char* end = NULL;
    errno = 0;

    unsigned long long tmp = strtoull(argv[1], &end, 10);

    if (errno == ERANGE)
    {
        fprintf(stderr, "Error: input value out of range\n");
        return EXIT_FAILURE;
    }

    if (end == argv[1] || *end != '\0')
    {
        fprintf(stderr, "Error: invalid numeric input '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (tmp > (unsigned long long)APN_SEG_MAX)
    {
        fprintf(stderr,
            "Error: input too large for ap_dig_t (max %" PRI_APN_SEGU ")\n",
            (ap_dig_t)APN_SEG_MAX);
        return EXIT_FAILURE;
    }

    ap_dig_t N = (ap_dig_t)tmp;

    /* ------------------------------------------------------------
     * Fibonacci index must be >= 2 and < 2 ^ 20
     * ------------------------------------------------------------ */
    APAC_ALWAYS_ASSERT(N >= 2);
    APAC_ALWAYS_ASSERT(N <= (1ULL << 22));

    apac_init();

    /* ------------------------------------------------------------
     * Limb count estimate (original, correct formula)
     * ------------------------------------------------------------ */
    ap_size_t max_limbs = (ap_size_t)((((double)N * LOG_2_PHI - LOG_2_SQRT5) / APN_SEG_BITS)) + 2;

    /* ------------------------------------------------------------
     * Allocate rolling Fibonacci buffers
     * ------------------------------------------------------------ */
    ap_dig_t* fib[2] = { 0 };

    fib[0] = apac_malloc(max_limbs * sizeof(ap_dig_t));
    fib[1] = apac_malloc(max_limbs * sizeof(ap_dig_t));

    APAC_ALWAYS_ASSERT(fib[0] != NULL);
    APAC_ALWAYS_ASSERT(fib[1] != NULL);

    apn_set(fib[0], max_limbs, 0);
    apn_set(fib[1], max_limbs, 0);

    /* Base values: F(0) = 1, F(1) = 1 */
    fib[0][0] = 1;
    fib[1][0] = 1;

    ap_size_t size_prev = 1;
    ap_size_t size_curr = 1;

    /* ------------------------------------------------------------
     * Fibonacci loop: F(n) = F(n-1) + F(n-2)
     * ------------------------------------------------------------ */
    for (ap_dig_t i = 2; i <= N; i++)
    {
        ap_size_t curr_size = (ap_size_t)((((double)i * LOG_2_PHI - LOG_2_SQRT5) / APN_SEG_BITS)) + 2;
        apn_add_n(fib[i & 1], fib[0], fib[1], curr_size);
    }

    ap_size_t answer = N & 1;
    ap_size_t nlimbs = apn_clamp(fib[answer], max_limbs);

    /* ------------------------------------------------------------
     * Output file
     * ------------------------------------------------------------ */
    char fname[64];
    snprintf(fname, sizeof(fname),
        "fibonacci_%" PRI_APN_SEGU ".txt", N);

    FILE* fp = fopen(fname, "w");
    if (!fp)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fprintf(fp, "F(%" PRI_APN_SEGU ") = 0x", N);
    ap_size_t count = 0;

    for (ap_size_t i = nlimbs - 1; i < nlimbs; i--)
    {    
        fprintf(fp, "%016" PRI_APN_SEGX, fib[answer][i]);
        count++;

        if ((count & 7) == 0)
            fputc('\n', fp);
    }
    
    fprintf(fp, "\n");
    fclose(fp);

    printf("Computed F(%" PRI_APN_SEGU ")\n", N);
    printf("Segments used: %" PRI_APN_SIZE "\n", nlimbs);
    printf("Output       : %s\n", fname);

    apac_free(fib[0]);
    apac_free(fib[1]);

    return 0;
}