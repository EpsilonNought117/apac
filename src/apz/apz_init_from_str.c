#include "../header/apac_internal.h"

apac_err
apz_init_from_str(
	apz_t* op1,
	const char* str,
	apac_str_base base
)
{
	APAC_ASSERT(apac_allocator.custom_malloc != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(str != NULL);
	APAC_ASSERT(base == BASE10 || base == BASE16);

	apac_err result = APAC_OK;
	const char* curr = &str[0];
	ap_size_t str_len = 0;

	// if negative integer
	// skip the minus sign
	// and set the sign field
	// in the apz_t struct

	if (curr[0] == '-')
	{
		op1->sign = APZ_NEG;
		curr++;
	}
	else
	{
		op1->sign = APZ_POS;
	}

	if (base == BASE10)
	{
		// verify string is correct first
		// and simulataneously calculate
		// count of decimal digits

		while (curr[str_len] != '\0')
		{
			char c = curr[str_len];

			if ((c < '0') || (c > '9'))
			{
				fprintf(
					stderr,
					"Invalid character in string"
					"for Base-10 at index %" PRI_APN_SIZE
					"\nMalformed String: %s\n",
					str_len + (curr - str),
					str
				);

				APAC_ALWAYS_ASSERT((c >= '0') || (c <= '9'));
			}

			str_len++;
		}

		APAC_ALWAYS_ASSERT(str_len != 0);

		// string is now verified to be correct and
		// we have the required length in count of 
		// decimal digits needed, allocate op1->num

		/*  Always allocate 1 digit extra as it  */
		/*  helps for using apn_mul_one in-place */

		ap_size_t digits = (str_len + 18) / 19 + 1;

		op1->num = (ap_dig_t*)apac_malloc(sizeof(ap_dig_t) * digits);

		if (!(op1->num)) { result = APAC_OOM; goto func_end; }

		op1->max_size = digits;
		apn_set(op1->num, digits, 0);

		ap_dig_t i = 1;
		ap_size_t curr_size = 1;
		ap_dig_t acc = 0;

		while (curr != '\0')
		{
			acc = acc * 10 + (*curr) - '0';
			i *= 10;

			if (i == TEN_TO_POW19)
			{
				apn_mul_one(op1->num, op1->num, curr_size, TEN_TO_POW19);
				op1->num[0] += acc;
				curr_size = apn_clamp(op1->num, curr_size + 1);

				// reset multiplier and acc
				i = 1;
				acc = 0;
			}

			curr++;
		}

		apn_mul_one(op1->num, op1->num, curr_size, i);
		op1->num[0] += acc;
		op1->curr_size = apn_clamp(op1->num, curr_size + 1);
	}
	else
	{
		// 0x or 0X at start of hex string should be skipped
		if (curr[0] == '0' && (curr[1] == 'x' || curr[1] == 'X'))
		{
			curr += 2;
		}

		while (curr[str_len] != '\0')
		{
			char c = curr[str_len];

			if (!(
				(c >= '0' && c <= '9') ||
				(c >= 'a' && c <= 'f') ||
				(c >= 'A' && c <= 'F')
				))
			{
				fprintf(
					stderr,
					"Invalid character in string"
					"for Base-16 at index %" PRI_APN_SIZE
					"\nMalformed String: %s\n",
					str_len + (curr - str),
					str
				);

				APAC_ALWAYS_ASSERT(
					(c >= '0' && c <= '9') ||
					(c >= 'a' && c <= 'f') ||
					(c >= 'A' && c <= 'F')
				);
			}

			str_len++;
		}

		APAC_ALWAYS_ASSERT(str_len != 0);

		ap_size_t digits = (str_len + 15) / 16;

		op1->num = (ap_dig_t*)apac_malloc(sizeof(ap_dig_t) * digits);

		if (!(op1->num)) { result = APAC_OOM; goto func_end; }

		op1->max_size = digits;
		apn_set(op1->num, digits, 0);

		ap_size_t i = 0;
		ap_size_t curr_dig = 0;
		ap_size_t curr_char = str_len - 1;
		ap_dig_t acc = 0;
	
		// using well-defined unsigned integer 
		// wrap-around behaviour from C11
		while (curr_char < str_len)
		{
			char c = curr[curr_char];
			i++;

			ap_dig_t val = 0;

			if (c >= '0' && c <= '9')
			{
				val = (ap_size_t)c - '0';
			}
			else if (c >= 'a' && c <= 'f')
			{
				val = 10ULL + c - 'a';
			}
			else
			{
				val = 10ULL + c - 'A';
			}

			acc = (acc << 4) | val;

			if (i == 16ULL)
			{
				op1->num[curr_dig] = acc;
				acc = 0; i = 0;
				curr_dig++;
			}

			curr_char--;
		}

		op1->num[curr_dig] = acc;
		APAC_ALWAYS_ASSERT(curr_dig == digits - 1);
		op1->curr_size = digits;
	}

func_end:
	return result;
}
