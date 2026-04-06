#include "../include/apac.h"
#include "../utilities/apac_utilities.h"

extern apac_cpu_params curr_cpu;

#define MAX_RUNTIME_1 (uint64_t)10 * 1000 * 1000
#define MAX_RUNTIME_2 (uint64_t)100 * 1000
#define TIME_TWOSIZE(thresh_idx, pair_idx) \
    (all_times[(thresh_idx) * (pair_count) + (pair_idx)])

#define APN_TUNE_ASSERT(expr)                   \
        do                                      \
        {                                       \
            if (!(expr))                        \
            {                                   \
                restore_turbo_boost();          \
            }                                   \
            APAC_ALWAYS_ASSERT((expr));         \
        } while (0)

static ap_size_t get_karatsuba_mul_threshold(void)
{
    const ap_size_t thresh_start = 10;
    const ap_size_t thresh_end = 50;
    const ap_size_t size_start = 1;
    const ap_size_t size_end = 256;

    const double IMPROVE_PCT = 0.05;   /* 5% improvement required */

    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* res = apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(op2 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp = fopen("karatsuba_mul_balanced_threshold.csv", "w");
    if (!fp)
    {
        restore_turbo_boost();
        APAC_LOG_ERR("Failed to open karatsuba_mul_balanced_threshold.csv");
        abort();
    }

    fprintf(fp, "threshold,size,min_cycles\n");

    double best_avg = 1e300;
    ap_size_t best_thresh = thresh_start;

    for (ap_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Karatsuba Balanced Mul Threshold %" PRI_APN_SIZE " ... ", thresh);
        fflush(stdout);

        curr_cpu.karatsuba_mul_threshold = thresh;

        double sum_mintimes = 0.0;
        ap_size_t size_count = 0;

        for (ap_size_t size = size_start; size <= size_end; size++)
        {
            set_to_random(op1, size);
            set_to_random(op2, size);

            uint64_t best = UINT64_MAX;
            uint64_t last_improve = os_timer();

            for (;;)
            {
                uint64_t t0 = cpu_timer();
                apn_mul_n(res, op1, op2, size);
                uint64_t t1 = cpu_timer();

                uint64_t dur = t1 - t0;

                if (best == UINT64_MAX || dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
                {
                    best = dur;
                    last_improve = os_timer();   /* reset only on >=5% improvement */
                }
                else
                {
                    uint64_t now = os_timer();
                    if ((now - last_improve) >= MAX_RUNTIME_1)
                    {
                        break;   /* no significant improvement for long enough */
                    }
                }
            }

            fprintf(fp,
                "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n",
                thresh, size, best);

            sum_mintimes += (double)best;
            size_count++;
        }

        double avg_mintime = sum_mintimes / (double)size_count;

        if (avg_mintime < best_avg)
        {
            best_avg = avg_mintime;
            best_thresh = thresh;
        }

        printf("Done (avg min cycles = %.3f)\n", avg_mintime);
    }

    fclose(fp);

    curr_cpu.karatsuba_mul_threshold = best_thresh;

    apac_free(op1);
    apac_free(op2);
    apac_free(res);

    return best_thresh;
}

static ap_size_t get_karatsuba_sqr_threshold(void)
{
    const ap_size_t thresh_start = 10;
    const ap_size_t thresh_end = 70;
    const ap_size_t size_start = 1;
    const ap_size_t size_end = 256;

    const double IMPROVE_PCT = 0.05;   /* 5% improvement required */

    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* res = apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp = fopen("karatsuba_sqr_threshold.csv", "w");
    if (!fp)
    {
        restore_turbo_boost();
        APAC_LOG_ERR("Failed to open karatsuba_sqr_threshold.csv");
        abort();
    }

    fprintf(fp, "threshold,size,min_cycles\n");

    double best_avg = 1e300;
    ap_size_t best_thresh = thresh_start;

    for (ap_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Karatsuba Sqr Threshold %" PRI_APN_SIZE " ... ", thresh);
        fflush(stdout);

        curr_cpu.karatsuba_sqr_threshold = thresh;

        double sum_mintimes = 0.0;
        ap_size_t size_count = 0;

        for (ap_size_t size = size_start; size <= size_end; size++)
        {
            set_to_random(op1, size);

            uint64_t best = UINT64_MAX;
            uint64_t last_improve = os_timer();

            for (;;)
            {
                uint64_t t0 = cpu_timer();
                apn_sqr(res, op1, size);
                uint64_t t1 = cpu_timer();

                uint64_t dur = t1 - t0;

                if (best == UINT64_MAX ||
                    dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
                {
                    best = dur;
                    last_improve = os_timer();   /* reset only on >=5% improvement */
                }
                else
                {
                    uint64_t now = os_timer();
                    if ((now - last_improve) >= MAX_RUNTIME_1)
                    {
                        break;   /* no significant improvement for long enough */
                    }
                }
            }

            fprintf(fp,
                "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n",
                thresh, size, best);

            sum_mintimes += (double)best;
            size_count++;
        }

        double avg_mintime = sum_mintimes / (double)size_count;

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

static ap_size_t get_dnc_div_threshold(void)
{
    const ap_size_t thresh_start = 10;
    const ap_size_t thresh_end = 70;
    const ap_size_t size_start = 1;
    const ap_size_t size_end = 256;

    const double IMPROVE_PCT = 0.05;
    const double TIE_PCT = 0.05;

    ap_dig_t* dividend = apac_malloc(sizeof(ap_dig_t) * (size_end * 2));
    ap_dig_t* divisor = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* quot = apac_malloc(sizeof(ap_dig_t) * (size_end * 2));
    ap_dig_t* rem = apac_malloc(sizeof(ap_dig_t) * size_end);

    APN_TUNE_ASSERT(dividend != NULL);
    APN_TUNE_ASSERT(divisor != NULL);
    APN_TUNE_ASSERT(quot != NULL);
    APN_TUNE_ASSERT(rem != NULL);

    set_to_random(dividend, size_end * 2);

    FILE* fp = fopen("dnc_div_threshold.csv", "w");
    if (!fp)
    {
        restore_turbo_boost();
        APAC_LOG_ERR("Failed to open dnc_div_threshold.csv");
        abort();
    }

    fprintf(fp, "threshold,size_divd,size_dvsr,min_cycles\n");

    const uint64_t dvsr_count = size_end - size_start + 1;

    const uint64_t pair_count =
        dvsr_count * (2 * (uint64_t)size_end + 1)
        - (dvsr_count * (size_end + size_start)) / 2;

    const ap_size_t thresh_count = thresh_end - thresh_start + 1;

    double* all_times = apac_malloc(sizeof(double) * thresh_count * pair_count);
    uint64_t* score = apac_malloc(sizeof(uint64_t) * thresh_count);

    APN_TUNE_ASSERT(all_times != NULL);
    APN_TUNE_ASSERT(score != NULL);

    for (ap_size_t ti = 0; ti < thresh_count; ti++)
        score[ti] = 0;

    /* ---------------- Collect timings ---------------- */

    for (ap_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        const ap_size_t ti = thresh - thresh_start;

        printf("Testing Divide-&-Conquer Div threshold %" PRI_APN_SIZE " ... ", thresh);
        fflush(stdout);

        curr_cpu.dnc_div_threshold = thresh;

        uint64_t pair_idx = 0;

        for (ap_size_t size_dvsr = size_start; size_dvsr <= size_end; size_dvsr++)
        {
            do {
                set_to_random(divisor, size_dvsr);
            } while (divisor[size_dvsr - 1] == 0);

            for (ap_size_t size_divd = size_dvsr; size_divd <= size_end * 2; size_divd++)
            {
                apn_set(quot, size_divd - size_dvsr + 1, 0);
                apn_set(rem, size_dvsr, 0);

                uint64_t best = UINT64_MAX;
                uint64_t last_improve = os_timer();

                for (;;)
                {
                    uint64_t t0 = cpu_timer();
                    apn_div(quot, rem,
                        dividend, divisor,
                        size_divd, size_dvsr);
                    uint64_t t1 = cpu_timer();

                    uint64_t dur = t1 - t0;

                    if (best == UINT64_MAX ||
                        dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
                    {
                        best = dur;
                        last_improve = os_timer();
                    }
                    else if ((os_timer() - last_improve) >= MAX_RUNTIME_2)
                    {
                        break;
                    }
                }

                fprintf(fp,
                    "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n",
                    thresh, size_divd, size_dvsr, best);

                TIME_TWOSIZE(ti, pair_idx) = (double)best;
                pair_idx++;
            }
        }

        printf("Done\n");
    }

    /* ---------------- Dominance scoring ---------------- */

    for (uint64_t pi = 0; pi < pair_count; pi++)
    {
        double best = TIME_TWOSIZE(0, pi);
        for (ap_size_t ti = 1; ti < thresh_count; ti++)
            if (TIME_TWOSIZE(ti, pi) < best)
                best = TIME_TWOSIZE(ti, pi);

        const double limit = best * (1.0 + TIE_PCT);

        for (ap_size_t ti = 0; ti < thresh_count; ti++)
            if (TIME_TWOSIZE(ti, pi) <= limit)
                score[ti]++;
    }

    uint64_t best_score = score[0];
    ap_size_t best_ti = 0;

    for (ap_size_t ti = 1; ti < thresh_count; ti++)
    {
        if (score[ti] > best_score)
        {
            best_score = score[ti];
            best_ti = ti;
        }
    }

    ap_size_t best_thresh = thresh_start + best_ti;

    printf("Best D&C div threshold = %" PRI_APN_SIZE
        " (wins %" PRIu64 " / %" PRIu64 " cases)\n",
        best_thresh, best_score, pair_count);

    fclose(fp);

    curr_cpu.dnc_div_threshold = best_thresh;

    apac_free(dividend);
    apac_free(divisor);
    apac_free(quot);
    apac_free(rem);
    apac_free(all_times);
    apac_free(score);

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
    apac_init();

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

    printf("\nAttempting to disable turbo boost ...\n");

    /* Disable turbo boost (or prompt user on unsupported platforms) */
    disable_turbo_boost();

    /* ------------------------------------------------------------
       Run threshold sweeps (with CPU rest between them)
       ------------------------------------------------------------ */

    ap_size_t mul_balanced_thresh = get_karatsuba_mul_threshold();

    printf("Resting CPU...\n");
    cpu_rest_ms(2000);

    ap_size_t sqr_thresh = get_karatsuba_sqr_threshold();

    printf("Resting CPU...\n");
    cpu_rest_ms(2000);

    ap_size_t div_thresh = get_dnc_div_threshold();

    printf("\nRestoring turbo boost (if disabled via apn_tune) ... \n");

    /* Restore original turbo boost state */
    restore_turbo_boost();

    /* ------------------------------------------------------------
       Print assignment-ready results
       ------------------------------------------------------------ */

    printf("\nRecommended threshold values:\n");

    printf("curr_cpu.karatsuba_mul_threshold   = "
        "(ap_size_t)(%" PRI_APN_SIZE ");\n",
        mul_balanced_thresh
    );

    printf("curr_cpu.karatsuba_sqr_threshold   = "
        "(ap_size_t)(%" PRI_APN_SIZE ");\n",
        sqr_thresh
    );

    printf("curr_cpu.dnc_div_threshold         = "
        "(ap_size_t)(%" PRI_APN_SIZE ");\n",
        div_thresh
    );

    return EXIT_SUCCESS;
}