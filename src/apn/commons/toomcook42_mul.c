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

    bool is_toomcook42_valid = false;


}
