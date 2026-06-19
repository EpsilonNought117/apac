#include "../header/apac_internal.h"

apac_err
apz_cpy(
	apz_t* result,
	const apz_t* op1
)
{
	APAC_ASSERT(apac_allocator.custom_realloc != NULL);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);

	// assumes result and op1 
	// have been initalized 
	// already in a valid way

	apac_err retval = APAC_OK;

	if (op1->curr_size > result->max_size)
	{
		retval = apz_resize(result, op1->curr_size);
		if (retval != APAC_OK) { goto func_end; }
	}

	apn_cpy(result->num, op1->num, op1->max_size);

	result->curr_size = op1->curr_size;
	result->max_size = op1->max_size;
	result->sign = op1->sign;

func_end:
	return retval;
}