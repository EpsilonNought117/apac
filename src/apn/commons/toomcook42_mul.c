#include "../headers/hidden_mul.h"

void apn_toomcook42_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2,
    ap_dig_t* temp
)
{
    APAC_ASSERT(temp != NULL);
    APAC_ASSERT(size1 >= size2);

    // Heuristic obtained from visual bound + trial-and-error
    bool is_toomcook42_valid = (2 * size1 >= 3 * size2 + 4) && (size1 <= 4 * size2 - 12);

    if (size1 < KARATSUBA_MUL_THRESHOLD)
    {
        apn_basecase_mul(result, op1, op2, size1, size2);
    }
    else if (!is_toomcook42_valid)
    {
        apn_mul_dispatcher(result, op1, op2, size1, size2, temp);
    }
    else
    {
        APAC_ASSERT((2 * size1 >= 3 * size2 + 4) && (size1 <= 4 * size2 - 12));

        ap_size_t split = size1 >= 2 * size2 ? (size1 + 3) / 4 : (size2 + 1) / 2;

        ap_size_t b1_size = size2 - split;
        ap_size_t a3_size = size1 - 3 * split;

        APAC_ASSERT((a3_size > 0) && (a3_size <= split));
        APAC_ASSERT((b1_size > 0) && (b1_size <= split));
        APAC_ASSERT((a3_size + b1_size) >= split);

        const ap_dig_t* b0 = op2, * b1 = &op2[split];
        const ap_dig_t* a0 = op1, * a1 = &op1[split];
        const ap_dig_t* a2 = &op1[2 * split], * a3 = &op1[3 * split];
    
        ap_dig_t* a_temp1 = &temp[0];
        ap_dig_t* a_temp2 = &temp[split + 1];

        temp[split] += apn_add_n(a_temp1, a0, a2, split);
        temp[2 * split + 1] += apn_add(a_temp2, a1, a3, split, a3_size);

        /*
            lower = split

                    |- a0 + a2 -|- a1 + a3 -|
            temp -->| lower + 1 | lower + 1 |
        */

        apn_cpy(&temp[2 * (split + 1)], &temp[0], 2 * (split + 1));

        /*
            lower = split

                    |- a0 + a2 -|- a1 + a3 -|- a0 + a2 -|- a1 + a3 -|
            temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |
        */

        ap_dig_t out = apn_add_n(&temp[0], &temp[0], &temp[split + 1], split + 1);
        APAC_ASSERT(out == 0);
        ap_dig_t* a_pos1 = &temp[0];

        ap_dig_t borrow1 = apn_sub_n(&temp[2 * (split + 1)], &temp[2 * (split + 1)], &temp[3 * (split + 1)], split + 1);
        if (borrow1) { apn_neg(&temp[2 * (split + 1)], &temp[2 * (split + 1)], split + 1); } // to get abs(a_neg1)
        ap_dig_t* a_neg1 = &temp[2 * (split + 1)];

        /*
            lower = split

                    |-- a_pos --|- a1 + a3 -|-- a_neg --|- a1 + a3 -|
            temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |
        */

        apn_set(&temp[split + 1], split + 1, 0);
        apn_set(&temp[3 * (split + 1)], split + 1, 0);

        temp[2 * split + 1] = apn_add(&temp[split + 1], b0, b1, split, b1_size);
        
        ap_dig_t borrow2 = apn_sub(&temp[3 * (split + 1)], b0, b1, split, b1_size);
        if (borrow2) { apn_neg(&temp[3 * (split + 1)], &temp[3 * (split + 1)], split); }

        ap_dig_t* b_pos1 = &temp[split + 1];
        ap_dig_t* b_neg1 = &temp[3 * (split + 1)];

        /*
            lower = split

                    |-- a_pos1--|-- b_pos1--|-- a_neg1--|-- b_neg1--|
            temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |
        */

        ap_dig_t* a_pos2 = &temp[4 * (split + 1)];

        temp[5 * split + 4] = apn_lshift_add(a_pos2, a0, a1, split, 1);
        temp[5 * split + 4] += apn_lshift_add(a_pos2, a_pos2, a2, split, 2);
        ap_dig_t temp_val = apn_lshift_add(a_pos2, a_pos2, a3, a3_size, 3);
        if (split > a3_size) { temp_val = apn_add_one(&a_pos2[a3_size], &a_pos2[a3_size], split - a3_size, temp_val); }
        temp[5 * split + 4] += temp_val;

        ap_dig_t* b_pos2 = &temp[5 * (split + 1)];
        temp_val = apn_lshift_add(b_pos2, b0, b1, b1_size, 1);
        if (split > b1_size) { temp_val = apn_add_one(&b_pos2[b1_size], &b0[b1_size], split - b1_size, temp_val); }
        temp[6 * split + 5] = temp_val;

        /*
            lower = split

                    |-- a_pos1--|-- b_pos1--|-- a_neg1--|-- b_neg1--|-- a_pos2--|-- b_pos2--|
            temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 |
        */

        apn_toomcook3_mul(result, a_pos1, b_pos1, split + 1, split + 1, &temp[6 * (split + 1)]);
        apn_cpy(&temp[0], &result[0], 2 * (split + 1));
        apn_set(&result[0], 2 * (split + 1), 0);
        ap_dig_t* w_pos1 = &temp[0];

        apn_toomcook3_mul(result, a_neg1, b_neg1, split + 1, split + 1, &temp[6 * (split + 1)]);
        apn_cpy(&temp[2 * (split + 1)], &result[0], 2 * (split + 1));
        apn_set(&result[0], 2 * (split + 1), 0);
        ap_dig_t w_neg1 = &temp[2 * (split + 1)];

        apn_toomcook3_mul(result, a_pos2, b_pos2, split + 1, split + 1, &temp[6 * (split + 1)]);
        apn_cpy(&temp[4 * (split + 1)], &result[0], 2 * (split + 1));
        apn_set(&result[0], 2 * (split + 1), 0);
        ap_dig_t* w_pos2 = &temp[4 * (split + 1)];

        /*
            lower = split

                    |-------- w_pos1 -------|-------- w_neg1 -------|------- w_pos2 --------|
            temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 | lower + 1 |
        */
    
        apn_toomcook3_mul(result, a0, b0, split, split, &temp[6 * (split + 1)]);

        if (a3_size >= b1_size)
        {
            apn_toomcook42_mul(&result[4 * split], a3, b1, a3_size, b1_size, &temp[6 * (split + 1)]);
        }
        else
        {
            apn_toomcook42_mul(&result[4 * split], b1, a3, b1_size, a3_size, &temp[6 * (split + 1)]);
        }

        ap_dig_t* w0 = &result[0];
        ap_dig_t* winf = &result[4 * split];
        
        APAC_ASSERT(apn_clamp(w_pos1, 2 * (split + 1)) <= (2 * split + 1));
        APAC_ASSERT(apn_clamp(w_neg1, 2 * (split + 1)) <= (2 * split + 1));

        /*
            same interpolation steps as ToomCook3
        */

        ap_dig_t* w_pos1_cpy = &temp[6 * (split + 1)];
        apn_cpy(w_pos1_cpy, w_pos1, 2 * (split + 1));

        temp[6 * split + 5] += apn_addmul_one(w_pos2, w0, 2 * split, 3);

        if (borrow1 != borrow2)
        {
            temp[6 * split + 5] -= apn_lshift_sub(w_pos2, w_pos2, w_neg1, 2 * split + 1, 1);
            apn_sub_n(w_pos1_cpy, w_pos1_cpy, w_neg1, 2 * split + 1);
        }
        else
        {
            temp[6 * split + 5] += apn_lshift_add(w_pos2, w_pos2, w_neg1, 2 * split + 1, 1);
            apn_add_n(w_pos1_cpy, w_pos1_cpy, w_neg1, 2 * split + 1);
        }

        apn_div_one(w_pos2, w_pos2, 6, 2 * (split + 1));
        temp_val = apn_lshift_sub(w_pos2, w_pos2, winf, a3_size + b1_size, 1);
        temp_val = apn_sub_one(
            w_pos2 + b1_size + a3_size, 
            w_pos2 + b1_size + a3_size, 
            2 * split - a3_size - b1_size + 2, 
            temp_val
        );
        ap_dig_t* t1 = w_pos2;

        apn_rshift(w_pos1_cpy, w_pos1_cpy, 2 * split + 1, 1);
        ap_dig_t* t2 = w_pos1_cpy;

        apn_sub_n(w_pos1, w_pos1, t1, 2 * (split + 1));
        ap_dig_t* c1 = w_pos1;

        apn_sub(w_neg1, t2, w0, 2 * (split + 1), 2 * split);
        apn_sub(w_neg1, w_neg1, winf, 2 * (split + 1), a3_size + b1_size);
        ap_dig_t* c2 = w_neg1;

        apn_sub_n(t1, t1, t2, 2 * (split + 1));
        ap_dig_t* c3 = t1;

        ap_size_t temp_size1 = split + a3_size + b1_size;
        ap_size_t temp_size2 = temp_size1 < 2 * split + 1 ? temp_size1 : 2 * split + 1;
        
        apn_add(&result[split], &result[split], c1, 3 * split + a3_size + b1_size, 2 * split + 1);
        apn_add(&result[2 * split], &result[2 * split], c2, 2 * split + a3_size + b1_size, 2 * split + 1);
        apn_add(&result[3 * split], &result[3 * split], c3, temp_size1, temp_size2);

        apn_set(temp, 8 * (split + 1), 0);
    }

    return;
}
