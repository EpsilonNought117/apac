#include "../src/header/apac_internal.h"

#define ITERS_1 256ULL
#define ITERS_2 16ULL

/*
 * Small step size is important because recursive crossover
 * regions can shift sharply across only a few limbs.
 */
#define STEP_SIZE1 ((apn_size_t)1)
#define STEP_SIZE2 ((apn_size_t)5)

#define APN_TUNE_ASSERT(expr)           \
    do                                  \
    {                                   \
        if (!(expr))                    \
        {                               \
            apac_restore_dfs();         \
        }                               \
        APAC_ALWAYS_ASSERT((expr));     \
    } while (0)

static apn_size_t get_karatsuba_mul_threshold(void)
{
    const apn_size_t thresh_start = 10;
    const apn_size_t thresh_end = 50;

    const apn_size_t size_start = 1;
    const apn_size_t size_end = 128;

    apn_dig_t* op1 =
        apac_malloc(sizeof(apn_dig_t) * size_end);

    apn_dig_t* op2 =
        apac_malloc(sizeof(apn_dig_t) * size_end);

    apn_dig_t* res =
        apac_malloc(sizeof(apn_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(op2 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    double global_best_avg = 1e300;
    apn_size_t best_thresh = thresh_start;

    printf("Obtaining optimal Karatsuba multiplication threshold ... ");

    for (apn_size_t thresh = thresh_start;
        thresh <= thresh_end;
        thresh++)
    {
        KARATSUBA_MUL_THRESHOLD = thresh;

        double total = 0.0;
        uint64_t count = 0;

        for (apn_size_t size = size_start;
            size <= size_end;
            size += STEP_SIZE1)
        {
            apn_set_random(op1, size);
            apn_set_random(op2, size);

            uint64_t best = UINT64_MAX;

            for (uint64_t iter = 0; iter < ITERS_1; iter++)
            {
                uint64_t t0 = apac_cpu_timer();

                apn_mul(res, op1, op2, size, size);

                uint64_t t1 = apac_cpu_timer();

                uint64_t dur = t1 - t0;

                if (dur < best)
                {
                    best = dur;
                }
            }

            total += (double)best;
            count++;
        }

        double avg = total / (double)count;

        if (avg < global_best_avg)
        {
            global_best_avg = avg;
            best_thresh = thresh;
        }
    }

    apac_free(op1);
    apac_free(op2);
    apac_free(res);

    KARATSUBA_MUL_THRESHOLD = best_thresh;

    printf("Done\n");

    return best_thresh;
}

static apn_size_t get_karatsuba_sqr_threshold(void)
{
    const apn_size_t thresh_start = 20;
    const apn_size_t thresh_end = 70;

    const apn_size_t size_start = 1;
    const apn_size_t size_end = 128;

    apn_dig_t* op1 =
        apac_malloc(sizeof(apn_dig_t) * size_end);

    apn_dig_t* res =
        apac_malloc(sizeof(apn_dig_t) * (2 * size_end));

    APN_TUNE_ASSERT(op1 != NULL);
    APN_TUNE_ASSERT(res != NULL);

    double global_best_avg = 1e300;
    apn_size_t best_thresh = thresh_start;

    printf("Obtaining optimal Karatsuba squaring threshold ... ");

    for (apn_size_t thresh = thresh_start;
        thresh <= thresh_end;
        thresh++)
    {
        KARATSUBA_SQR_THRESHOLD = thresh;

        double total = 0.0;
        uint64_t count = 0;

        for (apn_size_t size = size_start;
            size <= size_end;
            size += STEP_SIZE1)
        {
            apn_set_random(op1, size);

            uint64_t best = UINT64_MAX;

            for (uint64_t iter = 0; iter < ITERS_1; iter++)
            {
                uint64_t t0 = apac_cpu_timer();

                apn_sqr(res, op1, size);

                uint64_t t1 = apac_cpu_timer();

                uint64_t dur = t1 - t0;

                if (dur < best)
                {
                    best = dur;
                }
            }

            total += (double)best;
            count++;
        }

        double avg = total / (double)count;

        if (avg < global_best_avg)
        {
            global_best_avg = avg;
            best_thresh = thresh;
        }
    }

    apac_free(op1);
    apac_free(res);

    KARATSUBA_SQR_THRESHOLD = best_thresh;

    printf("Done\n");

    return best_thresh;
}

static apn_size_t get_dnc_div_threshold(void)
{
    const apn_size_t thresh_start = 20;
    const apn_size_t thresh_end = 60;

    const apn_size_t size_start = 1;
    const apn_size_t size_end = 128;

    apn_dig_t* dividend =
        apac_malloc(sizeof(apn_dig_t) * (size_end * 2));

    apn_dig_t* divisor =
        apac_malloc(sizeof(apn_dig_t) * size_end);

    apn_dig_t* quot =
        apac_malloc(sizeof(apn_dig_t) * (size_end * 2));

    apn_dig_t* rem =
        apac_malloc(sizeof(apn_dig_t) * size_end);

    APN_TUNE_ASSERT(dividend != NULL);
    APN_TUNE_ASSERT(divisor != NULL);
    APN_TUNE_ASSERT(quot != NULL);
    APN_TUNE_ASSERT(rem != NULL);

    apn_set_random(dividend, size_end * 2);

    double global_best_avg = 1e300;
    apn_size_t best_thresh = thresh_start;

    printf("Obtaining optimal Divide-&-Conquer division threshold ... ");

    for (apn_size_t thresh = thresh_start;
        thresh <= thresh_end;
        thresh++)
    {            
        DNC_DIV_THRESHOLD = thresh;

        double total = 0.0;
        uint64_t count = 0;

        for (apn_size_t size_dvsr = size_start;
            size_dvsr <= size_end;
            size_dvsr += STEP_SIZE2)
        {
            do
            {
                apn_set_random(divisor, size_dvsr);
            } while (divisor[size_dvsr - 1] == 0);

            for (apn_size_t size_divd = size_dvsr;
                size_divd <= size_end * 2;
                size_divd += STEP_SIZE2)
            {
                apn_set(
                    quot,
                    size_divd - size_dvsr + 1,
                    0);

                apn_set(
                    rem,
                    size_dvsr,
                    0);

                uint64_t best = UINT64_MAX;

                for (uint64_t iter = 0; iter < ITERS_2; iter++)
                {
                    uint64_t t0 = apac_cpu_timer();

                    apn_div(
                        quot,
                        rem,
                        dividend,
                        divisor,
                        size_divd,
                        0,
                        size_dvsr
                    );

                    uint64_t t1 = apac_cpu_timer();

                    uint64_t dur = t1 - t0;

                    if (dur < best)
                    {
                        best = dur;
                    }
                }

                total += (double)best;
                count++;
            }
        }

        double avg = total / (double)count;

        if (avg < global_best_avg)
        {
            global_best_avg = avg;
            best_thresh = thresh;
        }
    }

    apac_free(dividend);
    apac_free(divisor);
    apac_free(quot);
    apac_free(rem);

    DNC_DIV_THRESHOLD = best_thresh;

    printf("Done\n");

    return best_thresh;
}

int main(int argc, char** argv)
{
    apac_get_cpu_spec();

    if (argc < 2 || argc > 3)
    {
        fprintf(
            stderr,
            "Usage: %s <core_id> <hex_seed>\n",
            argv[0]);

        return EXIT_FAILURE;
    }

    char* endptr = NULL;

    unsigned long tmp =
        strtoul(argv[1], &endptr, 10);

    if (endptr == argv[1] || *endptr != '\0')
    {
        return EXIT_FAILURE;
    }

    uint32_t core_id = (uint32_t)tmp;

    uint64_t seed = 0xC0FFEE;

    if (argc == 3)
    {
        endptr = NULL;

        seed =
            strtoull(argv[2], &endptr, 16);

        if (endptr == argv[2] || *endptr != '\0')
        {
            return EXIT_FAILURE;
        }
    }

    apac_pin_thread_to_core(core_id);

    apn_seed_prng(seed);

    printf(
        "Pinned to core %u, seed 0x%016llX\n\n",
        core_id,
        (unsigned long long)seed);

    apac_disable_dfs();

    apn_size_t kara_mul_thresh = get_karatsuba_mul_threshold();

    apn_size_t kara_sqr_thresh = get_karatsuba_sqr_threshold();

    apn_size_t dnc_div_thresh = get_dnc_div_threshold();

    apac_restore_dfs();

    printf(
        "KARATSUBA_MUL_THRESHOLD  = (apn_size_t)(%" PRI_APN_SIZE ");\n",
        kara_mul_thresh);

    printf(
        "KARATSUBA_SQR_THRESHOLD  = (apn_size_t)(%" PRI_APN_SIZE ");\n",
        kara_sqr_thresh);

    printf(
        "DNC_DIV_THRESHOLD        = (apn_size_t)(%" PRI_APN_SIZE ");\n",
        dnc_div_thresh);

    return EXIT_SUCCESS;
}