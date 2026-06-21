#include "../header/apac_internal.h"

apac_err
apz_add(
    apz_t* result,
    const apz_t* op1,
    const apz_t* op2
)
{
    APAC_ASSERT(result != NULL);
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(op2 != NULL);

    apac_err retval = APAC_OK;

    apn_size_t larger = op1->curr_size > op2->curr_size ? 
                       op1->curr_size : op2->curr_size;

    if (result->max_size < larger)
    {
        apn_size_t new_size = ((result->max_size * 3) / 2) + 8;
        new_size = new_size > larger ? new_size : larger; 
        new_size++;

        retval = apz_resize(result, new_size);
        if (retval != APAC_OK) { goto func_end; }
        result->max_size = new_size;
    }

    const apz_t* max_op = NULL;
    const apz_t* min_op = NULL;

    if (op1->curr_size != op2->curr_size)
    {
        max_op = op1->curr_size > op2->curr_size ? op1 : op2;
        min_op = op1->curr_size > op2->curr_size ? op2 : op1;
    }
    else
    {
        int flag = apn_cmp(op1->num, op2->num, op1->curr_size);
        max_op = flag >= 0 ? op1 : op2;
        min_op = flag >= 0 ? op2 : op1;
    }

    if (max_op->sign == min_op->sign)
    {
        result->num[larger] = apn_add(
            result->num, 
            max_op->num, 
            min_op->num, 
            max_op->curr_size, 
            min_op->curr_size
        );
    }
    else
    {
        result->num[larger] = apn_sub(
            result->num, 
            max_op->num, 
            min_op->num, 
            max_op->curr_size, 
            min_op->curr_size
        );
    }

    result->curr_size = apn_clamp(result->num, larger);
    result->sign = result->curr_size ? max_op->sign : APZ_POS /* if the result is zero */;
    /* can't have zero with sign as APZ_NEG */

func_end:
    return retval;
}