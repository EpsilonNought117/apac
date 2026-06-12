#include "../header/apac_internal.h"

char*
apz_get_as_str(
	const apz_t* op1,
	apac_str_base base
)
{
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(base == BASE10 || base == BASE16);

	char* str = NULL;

	if (op1->curr_size == (ap_size_t)0)
	{
		str = (char*)apac_malloc(sizeof(char));
		if (str != NULL) { str[0] = '\0'; }	// immediately place NULL char
		goto func_end;	// else simply return NULL ptr for malloc fail
	}

	ap_size_t bitcnt = 0;
	ap_size_t len = 0;

	if (op1->sign == APZ_NEG)
		len++;			// for '-' sign

	if (base == BASE16)
		len += 2;		// for '0' and 'x'

	// get bit length of top 
	// most word of op1->num
	CLZ(op1->num[op1->curr_size - 1], bitcnt);
	bitcnt = (ap_size_t)64 - bitcnt;
	
	// add the bits of remaining words
	// multiplied by bits-per-word
	bitcnt += (op1->curr_size - 1) * APN_DIG_BITS;

	// now we know op1 is at most
	// 2^bitcnt - 1 in magnitude

	if (base == BASE10)
	{
		// log10(2) = 0.30102... < 0.303030... = (10 / 33)

		len += (bitcnt * 10) / 33 + 1; // one extra char for '\0'
		// floor((bitcnt * 10) / 33) + 1

		str = (char*)apac_malloc(len * sizeof(char));
		if (str == NULL) { goto func_end; }	// return NULL to show allocation failure

		if (op1->sign == APZ_NEG)
			str[0] = '-';

		
	}
	else
	{

	}

func_end:
	return str;
}