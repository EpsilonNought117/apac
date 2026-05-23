#include "../headers/hidden_mul.h"

/*
    This function will not dispatch to NTT, that is totally separate.
 
    It will also not dispatch to apn_basecase_mul, as that is handled by
    a simple if-else condition inside other multiplication functions
    by themselves.
*/
void apn_mul_dispatcher(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2,
    ap_dig_t* temp
)
{
    bool is_toomcook32_valid = (size2 <= 2 * ((size1 + 2) / 3)) && (size2 >= ((size1 + 2) / 3) + 2);
    bool is_toomcook42_valid = (size2 <= 2 * ((size1 + 3) / 4)) && (size2 >= ((size1 + 3) / 4) + 3);
    bool is_karatsuba_valid = (size2 > (size1 + 1) / 2) && (size1 >= KARATSUBA_MUL_THRESHOLD);
    bool is_toomcook3_valid = (size2 >= 2 * ((size1 + 2) / 3) + 2) && (size1 >= TOOMCOOK3_MUL_THRESHOLD);
    
    if (is_toomcook32_valid)
    {
        apn_toomcook32_mul(result, op1, op2, size1, size2, temp);
    }
    else if (is_toomcook42_valid)
    {
        apn_toomcook42_mul(result, op1, op2, size1, size2, temp);
    }
    else if (is_toomcook3_valid)
    {
        apn_toomcook3_mul(result, op1, op2, size1, size2, temp);
    }
    else if (is_karatsuba_valid)
    {
        apn_karatsuba_mul(result, op1, op2, size1, size2, temp);
    }
    else
    {
        apn_basecase_mul(result, op1, op2, size1, size2);
    }

    return;
}