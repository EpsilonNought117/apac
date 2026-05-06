#include "../headers/hidden_sqr.h"

void apn_toomcook3_sqr(
	ap_dig_t* result,
	const ap_dig_t* op1,
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
    APAC_ASSERT(TOOMCOOK3_SQR_THRESHOLD > 4);

    if (size < TOOMCOOK3_SQR_THRESHOLD)
    {
        apn_karatsuba_sqr(result, op1, size, temp);
        return;
    }

    ap_size_t lower = (size + 2) / 3;
    ap_size_t middle = lower;
    ap_size_t upper = size - 2 * lower;

    const ap_dig_t* a0 = op1, * a1 = op1 + lower, * a2 = op1 + 2 * lower;

    ap_dig_t* a02 = &temp[0];
    temp[lower] = apn_add(a02, a0, a2, lower, upper);

    ap_dig_t* a02_cpy = &temp[2 * (lower + 1)];
    apn_cpy(a02_cpy, a02, lower + 1);

    /*
                |--- a02 ---|   empty   |- a02_cpy -|   empty   |
        temp -->| lower + 1 | lower + 1 | lower + 1 | lower + 1 |
    */

    temp[lower] += apn_add_n(a02, a02, a1, lower);

    ap_dig_t* v1 = result;
    apn_toomcook3_sqr(v1, a02, lower + 1, &temp[4 * (lower + 1)]);

    apn_cpy(temp, v1, 2 * (lower + 1));
    v1 = temp;
    apn_set(result, 2 * (lower + 1), (ap_dig_t)0);

    ap_dig_t borrow1 = apn_sub(a02_cpy, a02_cpy, a1, lower + 1, lower);
    if (borrow1) { apn_neg(a02_cpy, a02_cpy, lower + 1); }

    

    return;
}