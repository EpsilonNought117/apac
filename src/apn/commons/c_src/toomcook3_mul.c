#include "../c_headers/apn_thresholds.h"
#include "../c_headers/hidden_mul.h"

void apn_toomcook3_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size,
    ap_dig_t* temp
)
{
    APAC_ASSERT(temp != NULL);

    /**
     * prevents the pathological case of size being 4,
     * causing a 0/2/2 split, leaving no limbs in 
     * the top most partition of op1 and op2.
     * No other size after 4 has this problem.
     */
    APAC_ASSERT(TOOMCOOK3_MUL_THRESHOLD > 4); 
    
    if (size < TOOMCOOK3_MUL_THRESHOLD)
    {
        apn_karatsuba_mul(result, op1, op2, size, temp);
        return;
    }

    ap_size_t lower = (size + 2) / 3;
    ap_size_t middle = lower;
    ap_size_t upper = size - 2 * lower;

    const ap_dig_t* a0 = op1, * a1 = op1 + lower, * a2 = op1 + 2 * lower;
    const ap_dig_t* b0 = op2, * b1 = op2 + lower, * b2 = op2 + 2 * lower;

    ap_dig_t* a02 = &temp[0], * b02 = &temp[lower + 1];
    
    temp[lower] = apn_add(a02, a0, a2, lower, upper);
    temp[2 * lower + 1] = apn_add(b02, b0, b2, lower, upper);

    ap_dig_t* a02_cpy = &temp[2 * lower + 2];
    ap_dig_t* b02_cpy = &temp[3 * lower + 3];

    apn_cpy(a02_cpy, a02, lower + 1);
    apn_cpy(b02_cpy, b02, lower + 1);

    /*
                |--- a02 ---|--- b02 ---|- a02_cpy -|- b02_cpy -|
        temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |
    */

    temp[lower] += apn_add_n(a02, a02, a1, lower);
    temp[2 * lower + 1] += apn_add_n(b02, b02, b1, lower);

    /*
        v1 = (a02 + a1) * (b02 + b1)
    */

    ap_dig_t* v1 = result;
    apn_toomcook3_mul(v1, a02, b02, lower + 1, &temp[4 * (lower + 1)]);

    apn_cpy(temp, v1, 2 * (lower + 1));
    v1 = temp;
    apn_set(result, 2 * (lower + 1), (ap_dig_t)0);

    ap_dig_t borrow1 = apn_sub(a02_cpy, a02_cpy, a1, lower + 1, lower);
    if (borrow1) { apn_neg(a02_cpy, a02_cpy, lower + 1); }

    ap_dig_t borrow2 = apn_sub(b02_cpy, b02_cpy, b1, lower + 1, lower);
    if (borrow2) { apn_neg(b02_cpy, b02_cpy, lower + 1); }

    /*
        vneg1 = (|a02 - a1|) * (|b02 - b1|)
    */

    ap_dig_t* vneg1 = result;
    apn_toomcook3_mul(result, a02_cpy, b02_cpy, lower + 1, &temp[4 * (lower + 1)]);

    apn_cpy(&temp[2 * (lower + 1)], vneg1, 2 * (lower + 1));
    vneg1 = &temp[2 * (lower + 1)];
    apn_set(result, 2 * (lower + 1), 0);

    /*
                |---------- v1 ---------|-------- vneg1 --------|- a0 + 2a1 + 4a2 -|- b0 + 2b1 + 4b2 -|
        temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |--- lower + 1 ----|--- lower + 1 ----|
    */

    temp[5 * lower + 4] = apn_lshift_add(&temp[4 * (lower + 1)], a0, a1, lower, 1); // 
    ap_dig_t temp1 = apn_lshift_add(&temp[4 * (lower + 1)], &temp[4 * (lower + 1)], a2, upper, 2);
    temp[5 * lower + 4] += apn_add_one(&temp[4 * (lower + 1) + upper], &temp[4 * (lower + 1) + upper], lower - upper, temp1);

    temp[6 * lower + 5] = apn_lshift_add(&temp[5 * (lower + 1)], b0, b1, lower, 1);
    ap_dig_t temp2 = apn_lshift_add(&temp[5 * (lower + 1)], &temp[5 * (lower + 1)], b2, upper, 2);
    temp[6 * lower + 5] += apn_add_one(&temp[5 * (lower + 1) + upper], &temp[5 * (lower + 1) + upper], lower - upper, temp2);
    
    ap_dig_t* v2 = result;
    apn_toomcook3_mul(v2, &temp[4 * (lower + 1)], &temp[5 * (lower + 1)], lower + 1, &temp[6 * (lower + 1)]);

    apn_cpy(&temp[4 * (lower + 1)], v2, 2 * (lower + 1));
    v2 = &temp[4 * (lower + 1)];
    apn_set(result, 2 * (lower + 1), 0);

    /*
                |---------- v1 ---------|-------- vneg1 --------|--------- v2 ----------|
        temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 |
    */

    /* 3 out of 5 recursive mutliplications done (v2, v1 and vneg1) */

    apn_toomcook3_mul(result, a0, b0, lower, &temp[6 * (lower + 1)]);
    apn_toomcook3_mul(&result[4 * lower], a2, b2, upper, &temp[6 * (lower + 1)]);

    ap_dig_t* v0 = result;
    ap_dig_t* vinf = &result[4 * lower];

    /* all 5 recursive multiplication done! */
    /* now to evaluate and interpolate */

    APAC_ASSERT(apn_clamp(v1, 2 * (lower + 1)) == (2 * lower + 1));
    APAC_ASSERT(apn_clamp(vneg1, 2 * (lower + 1)) == (2 * lower + 1));

    // store v1_cpy after v2
    ap_dig_t* v1_cpy = &temp[6 * (lower + 1)];
    apn_cpy(v1_cpy, v1, 2 * lower + 1);

    /*
                |---------- v1 ---------|-------- vneg1 --------|--------- v2 ----------|------- v1_cpy --------|
        temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 |
    */

    temp[6 * lower + 5] += apn_addmul_one(v2, v0, 2 * lower, 3);

    ap_dig_t is_vneg1_neg = (borrow1 != borrow2);

    if (is_vneg1_neg)
    {
        temp[6 * lower + 5] += apn_lshift_sub(v2, v2, vneg1, 2 * lower + 1, 1);
        apn_sub_n(v1_cpy, v1_cpy, vneg1, 2 * lower + 1);
    }
    else
    {
        temp[6 * lower + 5] += apn_lshift_add(v2, v2, vneg1, 2 * lower + 1, 1);
        apn_add_n(v1_cpy, v1_cpy, vneg1, 2 * lower + 1);
    }

    apn_div_one(v2, v2, 6, 2 * (lower + 1));
    apn_sub(v2, v2, vinf, 2 * (lower + 1), 2 * upper);
    ap_dig_t* t1 = v2;
    
    apn_rshift(v1_cpy, v1_cpy, 2 * lower + 1, 1);
    ap_dig_t* t2 = v1_cpy;

    /*
                |---------- v1 ---------|-------- vneg1 --------|--------- t1 ----------|--------- t2 ----------|
        temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 |
    */

    apn_sub_n(v1, v1, t1, 2 * (lower + 1));
    ap_dig_t* c1 = v1;

    apn_sub(vneg1, t2, v0, 2 * (lower + 1), 2 * lower);
    apn_sub(vneg1, vneg1, vinf, 2 * (lower + 1), 2 * upper);
    ap_dig_t* c2 = vneg1;

    /*
                |---------- c1 ---------|--------- c2 ----------|--------- t1 ----------|--------- t2 ----------|
        temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 |
    */

    apn_sub_n(t1, t1, t2, 2 * (lower + 1));
    ap_dig_t* c3 = t1;

    /*
                |---------- c1 ---------|--------- c2 ----------|--------- c3 ----------|
        temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 |
    */

    /*
        | lower | lower | lower | lower | upper | upper |
        |----- c0 ------|
                |----- c1 ------|
                        |----- c2 ------|
                                |------- c3 -------|        <--- this one may extend a little beyond the first upper
                                        |----- c4 ------|

    */

    apn_add_n(&result[lower], &result[lower], c1, 2 * lower);
    apn_add_n(&result[2 * lower], &result[2 * lower], c2, 2 * lower);
    apn_add_n(&result[3 * lower], &result[3 * lower], c3, 2 * lower);

    apn_set(temp, 8 * (lower + 1), 0);
    
    return;
}