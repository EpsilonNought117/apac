#include "../header/apac_internal.h"

apac_err
apz_get_as_str(
	char* in,
	const apz_t* op1,
	apac_str_base base
)
{
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(base == BASE10 || base == BASE16);

	char* str = NULL;

	if (op1->curr_size == (apn_size_t)0)
	{
		str = (char*)apac_malloc(sizeof(char));
		if (str != NULL) { str[0] = '\0'; }	// immediately place NULL char
		goto func_end;	// else simply return NULL ptr for malloc fail
	}

	apn_size_t len = apz_size_in_base(op1, base);

	len += (apn_size_t)(op1->sign == APZ_NEG);	// if minus sign needed
	len += (apn_size_t)2 * (base == BASE16);		// if "0x" needed due to BASE16

	str = (char*)apac_malloc((len + 1) * sizeof(char));
	if (str == NULL) { goto func_end; }	// return NULL to show allocation failure

	str[len] = '\0';		// place terminating null char immediately
	apn_size_t num_begin = 0;

	if (op1->sign == APZ_NEG)
	{
		str[num_begin] = '-'; // place minus sign
		num_begin++;
	}

	if (base == BASE16)
	{
		str[num_begin] = '0';
		str[num_begin + 1] = 'x';
		num_begin += 2;
	}

	// now num_begin has the index of 
	// the first character in the string

	if (base == BASE10)
	{
		/*
			Need to make a copy of op1->num as it 
			will get destroyed by apn_div_one calls.
		*/

		apn_dig_t* op1_cpy = (apn_dig_t*)apac_malloc(sizeof(apn_dig_t) * op1->curr_size);

		if (!op1_cpy)
		{
			apac_free(str);
			str = NULL;
			goto func_end;
		}

		apn_cpy(op1_cpy, op1->num, op1->curr_size);

		// op1_cpy now has op1->num as is

		apn_size_t idx = len - 1;
		apn_dig_t rmdr = 0;
		apn_size_t curr_size = op1->curr_size;

		while (idx > num_begin)
		{
			rmdr = apn_div_one(op1_cpy, op1_cpy, TEN_TO_POW19, curr_size, 0);
			curr_size = apn_clamp(op1_cpy, curr_size);

			while (rmdr != 0)
			{
				char c = '0' + (rmdr % 10);
				rmdr /= 10;
				
				str[idx] = c;
				idx--;
			}
		}

		APAC_ASSERT(idx == num_begin);
		APAC_ASSERT(curr_size == 0ULL);
		
		apac_free(op1_cpy);
		op1_cpy = NULL;
	}
	else
	{
		apn_size_t idx = len - 1;
		apn_size_t curr_idx = 0;

		while (idx > num_begin)
		{
			apn_dig_t val = op1->num[curr_idx];

			while (val != 0)
			{
				uint8_t curr_dig = (val % 16);
				val /= 16;

				char c = curr_dig + '0' + ('A' - '0' - 10) * (val > 9);
				str[idx] = c;
				idx--;
			}

			curr_idx++;
		}

		APAC_ASSERT(idx == num_begin);
	}

func_end:
	return str;
}