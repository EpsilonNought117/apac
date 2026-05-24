#include "../src/header/apac_internal.h"

extern apac_cpu_params curr_cpu;

#define MAX_RUNTIME_1 (uint64_t)10 * 1000 * 1000
#define MAX_RUNTIME_2 (uint64_t)100 * 1000

/* Step size for operand size iterations across all sweeps */
#define SIZE_STEP_SIZE (ap_size_t)10

#define APN_TUNE_ASSERT(expr)           \
        do                              \
        {                               \
            if (!(expr))                \
            {                           \
                apac_restore_dfs();     \
            }                           \
            APAC_ALWAYS_ASSERT((expr)); \
        } while (0)

static void get_karatsuba_mul_threshold(void)
{
    const ap_size_t thresh_start = 10;
    const ap_size_t thresh_end = 60;
    const ap_size_t size_start = 1;
    const ap_size_t size_end = 400;

    const double IMPROVE_PCT = 0.05;   /* 5% improvement required */

    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* res = apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(op2 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp = fopen("karatsuba_mul_balanced_threshold.csv", "w");
    APN_TUNE_ASSERT(fp != NULL);

    fprintf(fp, "threshold,size,min_cycles\n");

    TOOMCOOK3_MUL_THRESHOLD = APN_DIG_MAX;

    for (ap_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Karatsuba Balanced Mul Threshold %" PRI_APN_SIZE " ... \n", thresh);
        fflush(stdout);

        KARATSUBA_MUL_THRESHOLD = thresh;

        for (ap_size_t size = size_start; size <= size_end; size += SIZE_STEP_SIZE)
        {
            apn_set_random(op1, size);
            apn_set_random(op2, size);

            uint64_t best = UINT64_MAX;
            uint64_t last_improve = apac_os_timer();

            for (;;)
            {
                uint64_t t0 = apac_cpu_timer();
                apn_mul_n(res, op1, op2, size);
                uint64_t t1 = apac_cpu_timer();

                uint64_t dur = t1 - t0;

                if (best == UINT64_MAX || dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
                {
                    best = dur;
                    last_improve = apac_os_timer();
                }
                else
                {
                    uint64_t now = apac_os_timer();
                    if ((now - last_improve) >= MAX_RUNTIME_1)
                    {
                        break;
                    }
                }
            }

            fprintf(fp, "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n", thresh, size, best);
        }
    }

    fclose(fp);
    apac_free(op1);
    apac_free(op2);
    apac_free(res);
}

static void get_karatsuba_sqr_threshold(void)
{
    const ap_size_t thresh_start = 10;
    const ap_size_t thresh_end = 80;
    const ap_size_t size_start = 1;
    const ap_size_t size_end = 400;

    const double IMPROVE_PCT = 0.05;

    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* res = apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp = fopen("karatsuba_sqr_threshold.csv", "w");
    APN_TUNE_ASSERT(fp != NULL);

    fprintf(fp, "threshold,size,min_cycles\n");

    TOOMCOOK3_SQR_THRESHOLD = APN_DIG_MAX;

    for (ap_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Karatsuba Sqr Threshold %" PRI_APN_SIZE " ... \n", thresh);
        fflush(stdout);

        KARATSUBA_SQR_THRESHOLD = thresh;

        for (ap_size_t size = size_start; size <= size_end; size += SIZE_STEP_SIZE)
        {
            apn_set_random(op1, size);

            uint64_t best = UINT64_MAX;
            uint64_t last_improve = apac_os_timer();

            for (;;)
            {
                uint64_t t0 = apac_cpu_timer();
                apn_sqr(res, op1, size);
                uint64_t t1 = apac_cpu_timer();

                uint64_t dur = t1 - t0;

                if (best == UINT64_MAX || dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
                {
                    best = dur;
                    last_improve = apac_os_timer();
                }
                else
                {
                    uint64_t now = apac_os_timer();
                    if ((now - last_improve) >= MAX_RUNTIME_1)
                    {
                        break;
                    }
                }
            }

            fprintf(fp, "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n", thresh, size, best);
        }
    }

    fclose(fp);
    apac_free(op1);
    apac_free(res);
}

static void get_toomcook3_mul_threshold(void)
{
    const ap_size_t thresh_start = 150;
    const ap_size_t thresh_end = 280;
    const ap_size_t size_start = 1;
    const ap_size_t size_end = 512;

    const double IMPROVE_PCT = 0.05;

    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* op2 = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* res = apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(op2 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp = fopen("toomcook3_mul_balanced_threshold.csv", "w");
    APN_TUNE_ASSERT(fp != NULL);

    fprintf(fp, "threshold,size,min_cycles\n");

    for (ap_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Toom-Cook3 Balanced Mul Threshold %" PRI_APN_SIZE " ... \n", thresh);
        fflush(stdout);

        TOOMCOOK3_MUL_THRESHOLD = thresh;

        for (ap_size_t size = size_start; size <= size_end; size += SIZE_STEP_SIZE)
        {
            apn_set_random(op1, size);
            apn_set_random(op2, size);

            uint64_t best = UINT64_MAX;
            uint64_t last_improve = apac_os_timer();

            for (;;)
            {
                uint64_t t0 = apac_cpu_timer();
                apn_mul_n(res, op1, op2, size);
                uint64_t t1 = apac_cpu_timer();

                uint64_t dur = t1 - t0;

                if (best == UINT64_MAX || dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
                {
                    best = dur;
                    last_improve = apac_os_timer();
                }
                else
                {
                    uint64_t now = apac_os_timer();
                    if ((now - last_improve) >= MAX_RUNTIME_1)
                    {
                        break;
                    }
                }
            }

            fprintf(fp, "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n", thresh, size, best);
        }
    }

    fclose(fp);
    apac_free(op1);
    apac_free(op2);
    apac_free(res);
}

static void get_toomcook3_sqr_threshold(void)
{
    const ap_size_t thresh_start = 150;
    const ap_size_t thresh_end = 300;
    const ap_size_t size_start = 1;
    const ap_size_t size_end = 512;

    const double IMPROVE_PCT = 0.05;

    ap_dig_t* op1 = apac_malloc(sizeof(ap_dig_t) * size_end);
    ap_dig_t* res = apac_malloc(sizeof(ap_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    FILE* fp = fopen("toomcook3_sqr_threshold.csv", "w");
    APN_TUNE_ASSERT(fp != NULL);

    fprintf(fp, "threshold,size,min_cycles\n");

    for (ap_size_t thresh = thresh_start; thresh <= thresh_end; thresh++)
    {
        printf("Testing Toom-Cook3 Sqr Threshold %" PRI_APN_SIZE " ... \n", thresh);
        fflush(stdout);

        TOOMCOOK3_SQR_THRESHOLD = thresh;

        for (ap_size_t size = size_start; size <= size_end; size += SIZE_STEP_SIZE)
        {
            apn_set_random(op1, size);

            uint64_t best = UINT64_MAX;
            uint64_t last_improve = apac_os_timer();

            for (;;)
            {
                uint64_t t0 = apac_cpu_timer();
                apn_sqr(res, op1, size);
                uint64_t t1 = apac_cpu_timer();

                uint64_t dur = t1 - t0;

                if (best == UINT64_MAX || dur < (uint64_t)((double)best * (1.0 - IMPROVE_PCT)))
                {
                    best = dur;
                    last_improve = apac_os_timer();
                }
                else
                {
                    uint64_t now = apac_os_timer();
                    if ((now - last_improve) >= MAX_RUNTIME_1)
                    {
                        break;
                    }
                }
            }

            fprintf(fp, "%" PRI_APN_SIZE ",%" PRI_APN_SIZE ",%" PRIu64 "\n", thresh, size, best);
        }
    }

    fclose(fp);
    apac_free(op1);
    apac_free(res);
}

static void get_dnc_div_threshold(void)
{
    const ap_size_t thresh_start = 10;
    const ap_size_t thresh_end = 80;
    const ap_size_t size_start = 1;
    const ap_size_t size_end = 384;

    const double IMPROVE_PCT = 0.05;

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