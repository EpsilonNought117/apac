#include "../include/apac.h"
#include <math.h>
#include <errno.h>

#define LOG2E 1.44269504088896340736f
#define PI    3.14159265358979323846f

int main(int argc, char** argv)
{
    /* ------------------------------------------------------------
     * Argument parsing / validation (UNCHANGED)
     * ------------------------------------------------------------ */
    if (argc != 2)
    {
        fprintf(stderr,
            "Usage: %s <N>\n"
            "  <N> must be a non-negative integer (0 allowed)\n",
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

    apn_seg_t N = (apn_seg_t)tmp;

    /* ------------------------------------------------------------
     * Factorial invariants
     * ------------------------------------------------------------ */
    APAC_ALWAYS_ASSERT(N >= 1);
    APAC_ALWAYS_ASSERT(N <= 1000 * 1000);

    apac_init();

    /* ------------------------------------------------------------
    * Stirling-based upper bound for N! limb count
    * ------------------------------------------------------------ */
    apn_size_t max_limbs = 0;

    double n = (double)N;

    double bits = n * log2(n)- (n * LOG2E) + (0.5 * log2(2.0 * PI * n));

    /* Round up bits → limbs, then pad */
    max_limbs = (apn_size_t)ceil(bits / APN_SEG_BITS);

    apn_seg_t* src = apac_malloc((max_limbs + 2) * sizeof(apn_seg_t));
    apn_seg_t* dst = apac_malloc((max_limbs + 2) * sizeof(apn_seg_t));

    APAC_ALWAYS_ASSERT(src != NULL);
    APAC_ALWAYS_ASSERT(dst != NULL);

    apn_size_t curr_limbs = 1;

    /* src = 1 */
    apn_set(dst, max_limbs + 2, 0);
    apn_set(src, max_limbs + 2, 0);
    src[0] = 1;

    /* ------------------------------------------------------------
    * Factorial loop (FIXED)
    * ------------------------------------------------------------ */
    for (apn_seg_t k = 2; k <= N; k++)
    {
        apn_addmul_one(dst, src, curr_limbs, k);
        curr_limbs = apn_clamp(dst, curr_limbs + 1);
        apn_cpy(src, dst, curr_limbs);
        apn_set(dst, curr_limbs, 0);
    }

    /* ------------------------------------------------------------
     * Output file
     * ------------------------------------------------------------ */
    char fname[64];
    snprintf(fname, sizeof(fname), "factorial_%" PRI_APN_SEGU ".txt", N);

    FILE* fp = fopen(fname, "w");
    if (!fp)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fprintf(fp, "%" PRI_APN_SEGU "! (in hexadecimal) = \n", N);

    apn_size_t count = 0;

    for (apn_size_t i = curr_limbs - 1; i < curr_limbs; i--)
    {
        fprintf(fp, "%016" PRI_APN_SEGX, src[i]);
        count++;

        if ((count & 7) == 0)
            fputc('\n', fp);
    }

    printf("Computed %" PRI_APN_SEGU "!\n", N);
    printf("Limbs used : %" PRI_APN_SIZE "\n", curr_limbs);
    printf("Output     : %s\n", fname);

    apac_free(src);
    apac_free(dst);

    return 0;
}