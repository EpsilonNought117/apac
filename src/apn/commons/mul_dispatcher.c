#include "../headers/hidden_mul.h"

void apn_mul_dispatcher(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2,
    ap_dig_t* temp
)
{
    bool is_basecase_valid = (size1 < KARATSUBA_MUL_THRESHOLD);
    bool is_karatsuba_valid = (size2 > (size1 + 1) / 2) && (size1 >= KARATSUBA_MUL_THRESHOLD);
    bool is_toomcook3_valid = (size2 >= 2 * ((size1 + 2) / 3) + 2) && (size1 >= TOOMCOOK3_MUL_THRESHOLD);
	bool is_toomcook32_valid = (size2 + 2 <= size1) && (size1 + 6 <= 3 * size2);
	bool is_toomcook42_valid = 0;

    if (is_basecase_valid)
    {
        apn_basecase_mul(result, op1, op2, size1, size2);
    }
    else if (is_toomcook32_valid)
    {
        apn_toomcook32_mul(result, op1, op2, size1, size2, temp);
    }
    else if (is_toomcook42_valid)
    {
        apn_toomcook42_mul(result, op1, op2, size1, size2, temp);
    }
    else if (is_karatsuba_valid)
    {
        apn_karatsuba_mul(result, op1, op2, size1, size2, temp);
    }
    else if (is_toomcook3_valid)
    {
        apn_karatsuba_mul(result, op1, op2, size1, size2, temp);
    }
    else
    {
        apn_basecase_mul(result, op1, op2, size1, size2);
    }

    return;
}