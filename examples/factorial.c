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

    apacInit();

    /* ------------------------------------------------------------
     * Stirling-based upper bound for N! limb count
     * ------------------------------------------------------------ */
    apn_size_t max_limbs = 0;
    
    double n = (double)N;
    
    double bits = (n * log2(n))
        - (n * LOG2E)
        + (0.5 * log2(2.0 * PI * n))
        + 1.0;   /* safety margin */

    max_limbs = (apn_size_t)(bits / (double)APN_SEG_BITS) + 2;

    apn_seg_t* src = apac_malloc((max_limbs + 2) * sizeof(apn_seg_t));
    apn_seg_t* dst = apac_malloc((max_limbs + 4) * sizeof(apn_seg_t));

    APAC_ALWAYS_ASSERT(src != NULL);
    APAC_ALWAYS_ASSERT(dst != NULL);

    apn_size_t nlimbs = 1;

    /* src = 1 */
    apn_set(dst, max_limbs + 4, 0);
    apn_set(src, max_limbs + 2, 0);
    src[0] = 1;

    /* ------------------------------------------------------------
     * Factorial loop
     * ------------------------------------------------------------ */
    for (apn_seg_t k = 2; k <= N; k++)
    {
        apn_set(dst, nlimbs + 2, 0);

        dst[nlimbs + 1] = apn_addmul_one(dst, src, nlimbs, k);

        nlimbs = apn_clamp(dst, nlimbs + 2);
        apn_cpy(src, dst, nlimbs);
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

    fprintf(fp, "%" PRI_APN_SEGU "! = 0x", N);
    for (apn_size_t i = nlimbs - 1; i < nlimbs; i--)
        fprintf(fp, "%" PRI_APN_SEGX, src[i]);
    fprintf(fp, "\n");

    fclose(fp);

    printf("Computed %" PRI_APN_SEGU "!\n", N);
    printf("Limbs used : %" PRI_APN_SIZE "\n", nlimbs);
    printf("Output     : %s\n", fname);

    apac_free(src);
    apac_free(dst);

    return 0;
}