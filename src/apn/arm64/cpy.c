#include "../../../include/apac.h"

void cpy_asimd_4unroll(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
)
{
	apn_size_t counter = 0;

	while (counter < size)
	{
		result[counter] = op1[counter];
		counter++;
	}
}