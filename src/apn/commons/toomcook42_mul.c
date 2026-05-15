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

    }
}
