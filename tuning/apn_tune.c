#include "../src/header/apac_internal.h"

#define MAX_RUNTIME_1 ((uint64_t)10 * 1000 * 1000)
#define MAX_RUNTIME_2 ((uint64_t)100 * 1000)

/*
 * Small step size is important because recursive crossover
 * regions can shift sharply across only a few limbs.
 */
#define SIZE_STEP_SIZE ((ap_size_t)2)

/*
 * We use minimum observed timings because system noise
 * almost always increases runtime rather than decreases it.
 *
 * Small improve threshold avoids premature convergence.
 */
#define IMPROVE_PCT (0.002)

/*
 * First threshold within FLATLINE_PCT of global optimum
 * is selected.
 */
#define FLATLINE_PCT (0.003)

#define APN_TUNE_ASSERT(expr)           \
    do                                  \
    {                                   \
        if (!(expr))                    \
        {                               \
            apac_restore_dfs();         \
        }                               \
        APAC_ALWAYS_ASSERT((expr));     \
    } while (0)

static uint64_t benchmark_mul(
    ap_dig_t* res,
    ap_dig_t* op1,
    ap_dig_t* op2,
    ap_size_t size)
{
    uint64_t best = UINT64_MAX;
    uint64_t last_improve = apac_os_timer();

    for (;;)
    {
        uint64_t t0 = apac_cpu_timer();

        apn_mul_n(res, op1, op2, size);

        uint64_t t1 = apac_cpu_timer();

        uint64_t dur = t1 - t0;

        if (best == UINT64_MAX ||
            dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
        {
            best = dur;
            last_improve = apac_os_timer();
        }
        else if ((apac_os_timer() - last_improve) >= MAX_RUNTIME_1)
        {
            break;
        }
    }

    return best;
}

static uint64_t benchmark_sqr(
    ap_dig_t* res,
    ap_dig_t* op1,
    ap_size_t size)
{
    uint64_t best = UINT64_MAX;
    uint64_t last_improve = apac_os_timer();

    for (;;)
    {
        uint64_t t0 = apac_cpu_timer();

        apn_sqr(res, op1, size);

        uint64_t t1 = apac_cpu_timer();

        uint64_t dur = t1 - t0;

        if (best == UINT64_MAX ||
            dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
        {
            best = dur;
            last_improve = apac_os_timer();
        }
        else if ((apac_os_timer() - last_improve) >= MAX_RUNTIME_1)
        {
            break;
        }
    }

    return best;
}

static ap_size_t choose_flatline_threshold(
    double* avg_times,
    ap_size_t thresh_start,
    ap_size_t thresh_end)
{
    double best = avg_times[0];

    for (ap_size_t i = 1;
         i <= (thresh_end - thresh_start);
         i++)
    {
        if (avg_times[i] < best)
        {
            best = avg_times[i];
        }
    }

    for (ap_size_t i = 0;
         i <= (thresh_end - thresh_start);
         i++)
    {
        if (avg_times[i] <= best * (1.0 + FLATLINE_PCT))
        {
            return thresh_start + i;
        }
    }

    return thresh_end;
}

static ap_size_t get_karatsuba_mul_threshold(void)
{
    const ap_size_t thresh_start = 10;
    const ap_size_t thresh_end = 80;

    const ap_size_t size_start = 1;
    const ap_size_t size_end = 256;

    double avg_times[
        (thresh_end - thresh_start) + 1];

    ap_dig_t* op1 =
        apac_malloc(sizeof(ap_dig_t) * size_end);

    ap_dig_t* op2 =
        apac_malloc(sizeof(ap_dig_t) * size_end);

    ap_dig_t* res =
        apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(op2 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp =
        fopen("karatsuba_mul_threshold.csv", "w");

    APN_TUNE_ASSERT(fp != NULL);

    fprintf(fp,
        "threshold,size,min_cycles\n");

    TOOMCOOK3_MUL_THRESHOLD = APN_DIG_MAX;

    for (ap_size_t thresh = thresh_start;
         thresh <= thresh_end;
         thresh++)
    {
        printf(
            "Testing Karatsuba Mul Threshold %" PRI_APN_SIZE "... ",
            thresh);

        fflush(stdout);

        KARATSUBA_MUL_THRESHOLD = thresh;

        double total = 0.0;
        uint64_t count = 0;

        for (ap_size_t size = size_start;
             size <= size_end;
             size += SIZE_STEP_SIZE)
        {
            apn_set_random(op1, size);
            apn_set_random(op2, size);

            uint64_t best =
                benchmark_mul(res, op1, op2, size);

            fprintf(fp,
                "%" PRI_APN_SIZE
                ",%" PRI_APN_SIZE
                ",%" PRIu64 "\n",
                thresh,
                size,
                best);

            total += (double)best;
            count++;
        }

        avg_times[thresh - thresh_start] =
            total / (double)count;

        printf(
            "avg = %.3f\n",
            avg_times[thresh - thresh_start]);
    }

    fclose(fp);

    ap_size_t best_thresh =
        choose_flatline_threshold(
            avg_times,
            thresh_start,
            thresh_end);

    printf(
        "\nSelected Karatsuba Mul Threshold = %" PRI_APN_SIZE "\n",
        best_thresh);

    apac_free(op1);
    apac_free(op2);
    apac_free(res);

    KARATSUBA_MUL_THRESHOLD = best_thresh;

    return best_thresh;
}

static ap_size_t get_karatsuba_sqr_threshold(void)
{
    const ap_size_t thresh_start = 10;
    const ap_size_t thresh_end = 100;

    const ap_size_t size_start = 1;
    const ap_size_t size_end = 256;

    double avg_times[
        (thresh_end - thresh_start) + 1];

    ap_dig_t* op1 =
        apac_malloc(sizeof(ap_dig_t) * size_end);

    ap_dig_t* res =
        apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp =
        fopen("karatsuba_sqr_threshold.csv", "w");

    APN_TUNE_ASSERT(fp != NULL);

    fprintf(fp,
        "threshold,size,min_cycles\n");

    TOOMCOOK3_SQR_THRESHOLD = APN_DIG_MAX;

    for (ap_size_t thresh = thresh_start;
         thresh <= thresh_end;
         thresh++)
    {
        printf(
            "Testing Karatsuba Sqr Threshold %" PRI_APN_SIZE "... ",
            thresh);

        fflush(stdout);

        KARATSUBA_SQR_THRESHOLD = thresh;

        double total = 0.0;
        uint64_t count = 0;

        for (ap_size_t size = size_start;
             size <= size_end;
             size += SIZE_STEP_SIZE)
        {
            apn_set_random(op1, size);

            uint64_t best =
                benchmark_sqr(res, op1, size);

            fprintf(fp,
                "%" PRI_APN_SIZE
                ",%" PRI_APN_SIZE
                ",%" PRIu64 "\n",
                thresh,
                size,
                best);

            total += (double)best;
            count++;
        }

        avg_times[thresh - thresh_start] =
            total / (double)count;

        printf(
            "avg = %.3f\n",
            avg_times[thresh - thresh_start]);
    }

    fclose(fp);

    ap_size_t best_thresh =
        choose_flatline_threshold(
            avg_times,
            thresh_start,
            thresh_end);

    printf(
        "\nSelected Karatsuba Sqr Threshold = %" PRI_APN_SIZE "\n",
        best_thresh);

    apac_free(op1);
    apac_free(res);

    KARATSUBA_SQR_THRESHOLD = best_thresh;

    return best_thresh;
}

static ap_size_t get_toomcook3_mul_threshold(void)
{
    const ap_size_t thresh_start = 100;
    const ap_size_t thresh_end = 256;

    const ap_size_t size_start = 1;
    const ap_size_t size_end = 512;

    double avg_times[
        (thresh_end - thresh_start) + 1];

    ap_dig_t* op1 =
        apac_malloc(sizeof(ap_dig_t) * size_end);

    ap_dig_t* op2 =
        apac_malloc(sizeof(ap_dig_t) * size_end);

    ap_dig_t* res =
        apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(op2 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp =
        fopen("toomcook3_mul_threshold.csv", "w");

    APN_TUNE_ASSERT(fp != NULL);

    fprintf(fp,
        "threshold,size,min_cycles\n");

    for (ap_size_t thresh = thresh_start;
         thresh <= thresh_end;
         thresh++)
    {
        printf(
            "Testing ToomCook3 Mul Threshold %" PRI_APN_SIZE "... ",
            thresh);

        fflush(stdout);

        TOOMCOOK3_MUL_THRESHOLD = thresh;

        double total = 0.0;
        uint64_t count = 0;

        for (ap_size_t size = size_start;
             size <= size_end;
             size += SIZE_STEP_SIZE)
        {
            apn_set_random(op1, size);
            apn_set_random(op2, size);

            uint64_t best =
                benchmark_mul(res, op1, op2, size);

            fprintf(fp,
                "%" PRI_APN_SIZE
                ",%" PRI_APN_SIZE
                ",%" PRIu64 "\n",
                thresh,
                size,
                best);

            total += (double)best;
            count++;
        }

        avg_times[thresh - thresh_start] =
            total / (double)count;

        printf(
            "avg = %.3f\n",
            avg_times[thresh - thresh_start]);
    }

    fclose(fp);

    ap_size_t best_thresh =
        choose_flatline_threshold(
            avg_times,
            thresh_start,
            thresh_end);

    printf(
        "\nSelected ToomCook3 Mul Threshold = %" PRI_APN_SIZE "\n",
        best_thresh);

    apac_free(op1);
    apac_free(op2);
    apac_free(res);

    TOOMCOOK3_MUL_THRESHOLD = best_thresh;

    return best_thresh;
}

static ap_size_t get_toomcook3_sqr_threshold(void)
{
    const ap_size_t thresh_start = 120;
    const ap_size_t thresh_end = 300;

    const ap_size_t size_start = 1;
    const ap_size_t size_end = 512;

    double avg_times[
        (thresh_end - thresh_start) + 1];

    ap_dig_t* op1 =
        apac_malloc(sizeof(ap_dig_t) * size_end);

    ap_dig_t* res =
        apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp =
        fopen("toomcook3_sqr_threshold.csv", "w");

    APN_TUNE_ASSERT(fp != NULL);

    fprintf(fp,
        "threshold,size,min_cycles\n");

    for (ap_size_t thresh = thresh_start;
         thresh <= thresh_end;
         thresh++)
    {
        printf(
            "Testing ToomCook3 Sqr Threshold %" PRI_APN_SIZE "... ",
            thresh);

        fflush(stdout);

        TOOMCOOK3_SQR_THRESHOLD = thresh;

        double total = 0.0;
        uint64_t count = 0;

        for (ap_size_t size = size_start;
             size <= size_end;
             size += SIZE_STEP_SIZE)
        {
            apn_set_random(op1, size);

            uint64_t best =
                benchmark_sqr(res, op1, size);

            fprintf(fp,
                "%" PRI_APN_SIZE
                ",%" PRI_APN_SIZE
                ",%" PRIu64 "\n",
                thresh,
                size,
                best);

            total += (double)best;
            count++;
        }

        avg_times[thresh - thresh_start] =
            total / (double)count;

        printf(
            "avg = %.3f\n",
            avg_times[thresh - thresh_start]);
    }

    fclose(fp);

    ap_size_t best_thresh =
        choose_flatline_threshold(
            avg_times,
            thresh_start,
            thresh_end);

    printf(
        "\nSelected ToomCook3 Sqr Threshold = %" PRI_APN_SIZE "\n",
        best_thresh);

    apac_free(op1);
    apac_free(res);

    TOOMCOOK3_SQR_THRESHOLD = best_thresh;

    return best_thresh;
}

static void get_dnc_div_threshold(void)
{
    const ap_size_t thresh_start = 10;
    const ap_size_t thresh_end = 80;
    const ap_size_t size_start = 1;
    const ap_size_t size_end = 384;

    ap_dig_t* dividend = apac_malloc(sizeof(ap_dig_t) * (size_end * 2));
    ap_dig_t* divisor = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* quot = apac_malloc(sizeof(ap_dig_t) * (size_end * 2));
    ap_dig_t* rem = apac_malloc(sizeof(ap_dig_t) * size_end);

    APN_TUNE_ASSERT(dividend != NULL);
    APN_TUNE_ASSERT(divisor != NULL);
    APN_TUNE_ASSERT(quot != NULL);
    APN_TUNE_ASSERT(rem != NULL);

    apn_set_random(dividend, size_end * 2);

    FILE* fp = fopen("dnc_div_threshold.csv", "w");
    APN_TUNE_ASSERT(fp != NULL);

    fprintf(fp, "threshold,size_divd,size_dvsr,min_cycles\n");

    for (ap_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Divide-&-Conquer Div threshold %" PRI_APN_SIZE " ... \n", thresh);
        fflush(stdout);

        DNC_DIV_THRESHOLD = thresh;

        for (ap_size_t size_dvsr = size_start; size_dvsr <= size_end; size_dvsr += SIZE_STEP_SIZE)
        {
            do
            {
                apn_set_random(divisor, size_dvsr);
            } while (divisor[size_dvsr - 1] == 0);

            for (ap_size_t size_divd = size_dvsr; size_divd <= size_end * 2; size_divd += SIZE_STEP_SIZE)
            {
                apn_set(quot, size_divd - size_dvsr + 1, 0);
                apn_set(rem, size_dvsr, 0);

                uint64_t best = UINT64_MAX;
                uint64_t last_improve = apac_os_timer();

                for (;;)
                {
                    uint64_t t0 = apac_cpu_timer();
                    apn_div(quot, rem, dividend, divisor, size_divd, 0, size_dvsr);
                    uint64_t t1 = apac_cpu_timer();

                    uint64_t dur = t1 - t0;

                    if (best == UINT64_MAX || dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
                    {
                        best = dur;
                        last_improve = apac_os_timer();
                    }
                    else if ((apac_os_timer() - last_improve) >= MAX_RUNTIME_2)
                    {
                        break;
                    }
                }

                fprintf(fp, "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n",
                    thresh, size_divd, size_dvsr, best);
            }
        }
    }

    fclose(fp);

    apac_free(dividend);
    apac_free(divisor);
    apac_free(quot);
    apac_free(rem);
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

    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: %s <core_id> [hex_seed]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* endptr = NULL;
    unsigned long tmp = strtoul(argv[1], &endptr, 10);
    if (endptr == argv[1] || *endptr != '\0') return EXIT_FAILURE;
    uint32_t core_id = (uint32_t)tmp;

    uint64_t seed = 0xC0FFEE;
    if (argc == 3)
    {
        endptr = NULL;
        seed = strtoull(argv[2], &endptr, 16);
        if (endptr == argv[2] || *endptr != '\0') return EXIT_FAILURE;
    }

    apac_pin_thread_to_core(core_id);
    apn_seed_prng(seed);

    printf("Pinned to core %u, seed 0x%016llX\n", core_id, (unsigned long long)seed);
    apac_disable_dfs();

    get_karatsuba_mul_threshold();
    cpu_rest_ms(2000);

    get_karatsuba_sqr_threshold();
    cpu_rest_ms(2000);

    get_toomcook3_mul_threshold();
    cpu_rest_ms(2000);

    get_toomcook3_sqr_threshold();
    cpu_rest_ms(2000);

    get_dnc_div_threshold();

    apac_restore_dfs();
    printf("\nData collection complete. Process CSV files to select optimal thresholds.\n");

    return EXIT_SUCCESS;
}