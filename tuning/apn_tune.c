#include "../include/apac.h"
#include "../utilities/apac_utilities.h"

#define MAX_RUNTIME_1 (uint64_t)10 * 1000 * 1000
#define MAX_RUNTIME_2 (uint64_t)100 * 1000

#define APN_TUNE_ASSERT(expr)                   \
        do                                      \
        {                                       \
            if (!(expr))                        \
            {                                   \
                restore_turbo_boost();          \
            }                                   \
            APAC_ALWAYS_ASSERT((expr));         \
        } while (0)

static apn_size_t get_karatsuba_mul_balanced_threshold(void)
{
    const apn_size_t thresh_start = 10;
    const apn_size_t thresh_end = 50;
    const apn_size_t size_start = 1;
    const apn_size_t size_end = 256;

    apn_seg_t* op1 = apac_malloc(sizeof(apn_seg_t) * size_end);
    apn_seg_t* op2 = apac_malloc(sizeof(apn_seg_t) * size_end);
    apn_seg_t* res = apac_malloc(sizeof(apn_seg_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(op2 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    set_to_random(op1, size_end);
    set_to_random(op2, size_end);

    FILE* fp = fopen("karatsuba_mul_balanced_threshold.csv", "w");
    if (!fp)
    {
        restore_turbo_boost();
        APAC_LOG_ERR("Failed to open karatsuba_mul_balanced_threshold.csv");
        abort();
    }

    fprintf(fp, "threshold,size,min_cycles\n");

    double best_avg = 1e300;
    apn_size_t best_thresh = thresh_start;
    const apn_size_t size_count = size_end - size_start + 1;

    for (apn_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Karatsuba Balanced Mul Threshold %" PRI_APN_SIZE " ... ", thresh);
        fflush(stdout);

        curr_cpu.karatsuba_mul_balanced_threshold = thresh;

        double sum_mintimes = 0.0;

        for (apn_size_t size = size_start; size <= size_end; size++)
        {
            uint64_t mintime = UINT64_MAX;
            uint64_t start_ns = os_timer();

            while (os_timer() - start_ns < MAX_RUNTIME_1)
            {
                uint64_t t0 = cpu_timer();
                apn_mul_n(res, op1, op2, size);
                uint64_t t1 = cpu_timer();

                uint64_t dur = t1 - t0;
                if (dur < mintime)
                    mintime = dur;
            }

            fprintf(fp,
                "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n",
                thresh, size, mintime);

            sum_mintimes += (double)mintime;
        }

        double avg_mintime = sum_mintimes / size_count;

        if (avg_mintime < best_avg)
        {
            best_avg = avg_mintime;
            best_thresh = thresh;
        }

        printf("Done (avg min cycles = %.3f)\n", avg_mintime);
    }

    fclose(fp);

    curr_cpu.karatsuba_mul_balanced_threshold = best_thresh;

    apac_free(op1);
    apac_free(op2);
    apac_free(res);

    return best_thresh;
}

static apn_size_t get_karatsuba_sqr_threshold(void)
{
    const apn_size_t thresh_start = 10;
    const apn_size_t thresh_end = 70;
    const apn_size_t size_start = 1;
    const apn_size_t size_end = 256;

    apn_seg_t* op1 = apac_malloc(sizeof(apn_seg_t) * size_end);
    apn_seg_t* res = apac_malloc(sizeof(apn_seg_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    set_to_random(op1, size_end);

    FILE* fp = fopen("karatsuba_sqr_threshold.csv", "w");
    if (!fp)
    {
        restore_turbo_boost();
        APAC_LOG_ERR("Failed to open karatsuba_sqr_threshold.csv");
        abort();
    }

    fprintf(fp, "threshold,size,min_cycles\n");

    double best_avg = 1e300;
    apn_size_t best_thresh = thresh_start;
    const apn_size_t size_count = size_end - size_start + 1;

    for (apn_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Karatsuba Sqr Threshold %" PRI_APN_SIZE " ... ", thresh);
        fflush(stdout);

        curr_cpu.karatsuba_sqr_threshold = thresh;

        double sum_mintimes = 0.0;

        for (apn_size_t size = size_start; size <= size_end; size++)
        {
            uint64_t mintime = UINT64_MAX;
            uint64_t start_ns = os_timer();

            while (os_timer() - start_ns < MAX_RUNTIME_1)
            {
                uint64_t t0 = cpu_timer();
                apn_sqr(res, op1, size);
                uint64_t t1 = cpu_timer();

                uint64_t dur = t1 - t0;
                if (dur < mintime)
                    mintime = dur;
            }

            fprintf(fp,
                "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n",
                thresh, size, mintime);

            sum_mintimes += (double)mintime;
        }

        double avg_mintime = sum_mintimes / size_count;

        if (avg_mintime < best_avg)
        {
            best_avg = avg_mintime;
            best_thresh = thresh;
        }

        printf("Done (avg min cycles = %.3f)\n", avg_mintime);
    }

    fclose(fp);

    curr_cpu.karatsuba_sqr_threshold = best_thresh;

    apac_free(op1);
    apac_free(res);

    return best_thresh;
}

static apn_size_t get_karatsuba_mul_unbalanced_threshold(void)
{
    const apn_size_t thresh_start = 10;
    const apn_size_t thresh_end = 70;
    const apn_size_t size_start = 1;
    const apn_size_t size_end = 256;

    apn_seg_t* op1 = apac_malloc(sizeof(apn_seg_t) * size_end);
    apn_seg_t* op2 = apac_malloc(sizeof(apn_seg_t) * size_end);
    apn_seg_t* res = apac_malloc(sizeof(apn_seg_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(op2 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    set_to_random(op1, size_end);
    set_to_random(op2, size_end);

    FILE* fp = fopen("karatsuba_mul_unbalanced_threshold.csv", "w");
    if (!fp)
    {
        restore_turbo_boost();
        APAC_LOG_ERR("Failed to open karatsuba_mul_unbalanced_threshold.csv");
        abort();
    }

    fprintf(fp, "threshold,size1,size2,min_cycles\n");

    double best_avg = 1e300;
    apn_size_t best_thresh = thresh_start;

    const uint64_t range = size_end - size_start + 1;
    const uint64_t pair_count = (range * (range + 1)) / 2;

    for (apn_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Karatsuba Unbalanced Mul Threshold %" PRI_APN_SIZE " ... ", thresh);
        fflush(stdout);

        curr_cpu.karatsuba_mul_unbalanced_threshold = thresh;

        double sum_mintimes = 0.0;

        for (apn_size_t size1 = size_start; size1 <= size_end; size1++)
        {
            for (apn_size_t size2 = size_start; size2 <= size1; size2++)
            {
                uint64_t mintime = UINT64_MAX;
                uint64_t start_ns = os_timer();

                while (os_timer() - start_ns < MAX_RUNTIME_2)
                {
                    uint64_t t0 = cpu_timer();
                    apn_mul(res, op1, op2, size1, size2);
                    uint64_t t1 = cpu_timer();

                    uint64_t dur = t1 - t0;
                    if (dur < mintime)
                        mintime = dur;
                }

                fprintf(fp,
                    "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n",
                    thresh, size1, size2, mintime);

                sum_mintimes += (double)mintime;
            }
        }

        double avg_mintime = sum_mintimes / pair_count;

        if (avg_mintime < best_avg)
        {
            best_avg = avg_mintime;
            best_thresh = thresh;
        }

        printf("Done (avg min cycles = %.3f)\n", avg_mintime);
    }

    fclose(fp);

    curr_cpu.karatsuba_mul_unbalanced_threshold = best_thresh;

    apac_free(op1);
    apac_free(op2);
    apac_free(res);

    return best_thresh;
}

static apn_size_t get_dnc_div_threshold(void)
{
    const apn_size_t thresh_start = 10;
    const apn_size_t thresh_end = 70;
    const apn_size_t size_start = 1;     /* may be changed safely */
    const apn_size_t size_end = 256;

    apn_seg_t* dividend = apac_malloc(sizeof(apn_seg_t) * (size_end * 2));
    apn_seg_t* divisor = apac_malloc(sizeof(apn_seg_t) * size_end);
    apn_seg_t* quot = apac_malloc(sizeof(apn_seg_t) * (size_end * 2));
    apn_seg_t* rem = apac_malloc(sizeof(apn_seg_t) * size_end);

    APN_TUNE_ASSERT(dividend != NULL);
    APN_TUNE_ASSERT(divisor != NULL);
    APN_TUNE_ASSERT(quot != NULL);
    APN_TUNE_ASSERT(rem != NULL);

    /* dividend can be reused across all sizes */
    set_to_random(dividend, size_end * 2);

    FILE* fp = fopen("dnc_div_threshold.csv", "w");
    if (!fp)
    {
        restore_turbo_boost();
        APAC_LOG_ERR("Failed to open dnc_div_threshold.csv");
        abort();
    }

    fprintf(fp, "threshold,size_divd,size_dvsr,min_cycles\n");

    double best_avg = 1e300;
    apn_size_t best_thresh = thresh_start;

    /*
     * Count all (size_dvsr, size_divd) pairs:
     *
     * size_dvsr = size_start .. size_end
     * size_divd = size_dvsr .. 2*size_end
     */
    const uint64_t dvsr_count = size_end - size_start + 1;

    const uint64_t pair_count =
        dvsr_count * (2 * (uint64_t)size_end + 1)
        - (dvsr_count * (size_end + size_start)) / 2;

    for (apn_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Divide-&-Conquer Div threshold %" PRI_APN_SIZE " ... ", thresh);
        fflush(stdout);

        curr_cpu.dnc_div_threshold = thresh;
        double sum_mintimes = 0.0;

        /* divisor size loop */
        for (apn_size_t size_dvsr = size_start;
            size_dvsr <= size_end;
            size_dvsr++)
        {
            /* generate a normalized divisor */
            do {
                set_to_random(divisor, size_dvsr);
            } while (divisor[size_dvsr - 1] == 0);

            /* dividend size loop */
            for (apn_size_t size_divd = size_dvsr;
                size_divd <= size_end * 2;
                size_divd++)
            {
                apn_set(quot, size_divd - size_dvsr + 1, 0);
                apn_set(rem, size_dvsr, 0);

                uint64_t mintime = UINT64_MAX;
                uint64_t start_ns = os_timer();

                while (os_timer() - start_ns < MAX_RUNTIME_2)
                {
                    uint64_t t0 = cpu_timer();
                    apn_div(quot, rem,
                        dividend, divisor,
                        size_divd, size_dvsr);
                    uint64_t t1 = cpu_timer();

                    uint64_t dur = t1 - t0;
                    if (dur < mintime)
                        mintime = dur;
                }

                fprintf(fp,
                    "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n",
                    thresh, size_divd, size_dvsr, mintime);

                sum_mintimes += (double)mintime;
            }
        }

        const double avg_mintime = sum_mintimes / pair_count;

        if (avg_mintime < best_avg)
        {
            best_avg = avg_mintime;
            best_thresh = thresh;
        }

        printf("Done (avg min cycles = %.3f)\n", avg_mintime);
    }

    fclose(fp);

    curr_cpu.dnc_div_threshold = best_thresh;

    apac_free(dividend);
    apac_free(divisor);
    apac_free(quot);
    apac_free(rem);

    return best_thresh;
}

static void cpu_rest_ms(unsigned ms)
{
#if defined(_WIN32)
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

int main(int argc, char** argv)
{
    apacInit();

    /* ------------------------------------------------------------
       Parse command-line arguments
       ------------------------------------------------------------ */

    if (argc < 2 || argc > 3)
    {
        fprintf(stderr,
            "Usage: %s <core_id> [hex_seed]\n"
            "  core_id  : logical CPU core to pin to\n"
            "  hex_seed : optional PRNG seed (hexadecimal)\n",
            argv[0]);
        return EXIT_FAILURE;
    }

    char* endptr = NULL;

    /* Parse core ID (decimal) */
    unsigned long tmp = strtoul(argv[1], &endptr, 10);
    if (endptr == argv[1] || *endptr != '\0')
    {
        fprintf(stderr, "Invalid core id: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    uint32_t core_id = (uint32_t)tmp;

    /* Parse optional seed */
    uint64_t seed = 0xC0FFEE;   /* default */

    if (argc == 3)
    {
        endptr = NULL;
        seed = strtoull(argv[2], &endptr, 16);

        if (endptr == argv[2] || *endptr != '\0')
        {
            fprintf(stderr, "Invalid hexadecimal seed: %s\n", argv[2]);
            return EXIT_FAILURE;
        }
    }

    /* ------------------------------------------------------------
       Pin thread
       ------------------------------------------------------------ */

    if (pin_curr_thread_to_core(core_id) != 0)
    {
        fprintf(stderr,
            "Warning: failed to pin thread to core %u (continuing anyway)\n",
            core_id);
    }

    /* ------------------------------------------------------------
       Initialize PRNG
       ------------------------------------------------------------ */

    random_sfc64_seed(seed);

    printf("Pinned to core %u, using PRNG seed 0x%016llX\n",
        core_id,
        (unsigned long long)seed);

    /* Disable turbo boost (or prompt user on unsupported platforms) */
    disable_turbo_boost();

    /* ------------------------------------------------------------
       Run threshold sweeps (with CPU rest between them)
       ------------------------------------------------------------ */

    apn_size_t mul_balanced_thresh =
        get_karatsuba_mul_balanced_threshold();

    printf("Resting CPU...\n");
    cpu_rest_ms(2000);

    apn_size_t sqr_thresh =
        get_karatsuba_sqr_threshold();

    printf("Resting CPU...\n");
    cpu_rest_ms(2000);

    apn_size_t mul_unbalanced_thresh =
        get_karatsuba_mul_unbalanced_threshold();

    printf("Resting CPU...\n");
    cpu_rest_ms(2000);

    apn_size_t div_thresh =
        get_dnc_div_threshold();

    /* Restore original turbo boost state */
    restore_turbo_boost();

    /* ------------------------------------------------------------
       Print assignment-ready results
       ------------------------------------------------------------ */

    printf("\nRecommended threshold values:\n");

    printf("curr_cpu.karatsuba_mul_balanced_threshold   = "
        "(apn_size_t)(%" PRI_APN_SIZE ");\n",
        mul_balanced_thresh);

    printf("curr_cpu.karatsuba_sqr_threshold            = "
        "(apn_size_t)(%" PRI_APN_SIZE ");\n",
        sqr_thresh);

    printf("curr_cpu.karatsuba_mul_unbalanced_threshold = "
        "(apn_size_t)(%" PRI_APN_SIZE ");\n",
        mul_unbalanced_thresh);

    printf("curr_cpu.dnc_div_threshold                  = "
        "(apn_size_t)(%" PRI_APN_SIZE ");\n",
        div_thresh);

    return EXIT_SUCCESS;
}