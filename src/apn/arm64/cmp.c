#include "../../../include/apac.h"
#include "../commons/hidden_helpers.h" // for CLZ64

int cmp_asimd_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
)
{
    while (size > 0) {
        size--;
        if (op1[size] != op2[size]) {
            return (op1[size] > op2[size]) ? 1 : -1;
        }
    }

    return 0;
}