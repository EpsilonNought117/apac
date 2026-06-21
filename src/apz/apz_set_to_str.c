#include "../header/apac_internal.h"

apac_err
apz_set_to_str(
    apz_t* op1,
    const char* str,
    apac_str_base base
)
{
    APAC_ASSERT(op1 != NULL);
    APAC_ASSERT(str != NULL);
    APAC_ASSERT(base == BASE10 || base == BASE16);

    apac_err retval = APAC_OK;
    const char* curr = &str[0];
    apn_size_t str_len = 0;

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

        // string is now verified to be correct and
		// we have the required length in count of 
		// decimal digits needed

		apn_size_t digits = (str_len + 18) / 19 + 1;

        if (digits > op1->max_size)
        {
			retval =  apz_resize(op1, digits);
			if (retval != APAC_OK) { goto func_end; }
        }

        apn_set(op1->num, digits, 0);

        apn_dig_t i = 0;
		apn_size_t curr_size = 1;
		apn_dig_t acc = 0;

		while (*curr != '\0')
		{
			acc = acc * 10 + (*curr) - '0';
			i++;

			if (i == (apn_size_t)19)
			{
				apn_mul_one(op1->num, op1->num, curr_size, TEN_TO_POW19);
				op1->num[0] += acc;
				curr_size = apn_clamp(op1->num, curr_size + 1);

				// reset multiplier and acc
				i = 0;
				acc = 0;
			}

			curr++;
		}

		apn_dig_t mult = 1;

		while (i--) { mult *= 10; }

		apn_mul_one(op1->num, op1->num, curr_size, mult);
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

		apn_size_t digits = (str_len + 15) / 16 + 1;

        if (digits > op1->max_size)
        {
			retval =  apz_resize(op1, digits);
			if (retval != APAC_OK) { goto func_end; }
        }

        apn_set(op1->num, digits, 0);

        apn_size_t i = 0;
		apn_size_t curr_dig = 0;
		apn_size_t curr_char = str_len - 1;
		apn_dig_t acc = 0;
	
		// using well-defined unsigned integer 
		// wrap-around behaviour from C11
		while (curr_char < str_len)
		{
			char c = curr[curr_char];
			i++;

			apn_dig_t val = 0;
			
			c |= 0x20; // force lowercase: 'A' to 'a', '0' to '0' (digits unaffected)
			val = c >= 'a' ? (apn_size_t)(c - 'a' + 10) : (apn_size_t)(c - '0');

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
		op1->curr_size = digits;
    }

func_end:
    return retval;
}