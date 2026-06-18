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
    ap_size_t str_len = 0;

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

        APAC_ASSERT(str_len != 0);
    }
    else
    {

    }
}