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
    ap_size_t str_len = 0;

    if (str[0] == '-')
    {
        op1->sign = APZ_NEG;
        str_len++;
    }
    else
    {
        op1->sign = APZ_POS;
    }

    if (base == BASE10)
    {
        // verify string is correct first

        while (str[str_len] != '\0')
        {
            char c = str[str_len];
            
            if ((c < '0') || (c > '9'))
            {
                fprintf(
                    stderr, 
                    "Invalid character in string"
                    "for Base 10 at index %" PRI_APN_SIZE 
                    "\nMalformed String: %s\n", 
                    str_len,
                    str
                );
                APAC_ALWAYS_ASSERT((c >= '0') || (c <= '9'));
            }

            str_len++;
        }

        // string is now verified to be correct and
        // we have the required length in count of 
        // decimal digits needed, allocate op1->num

        /* Always end up allocating up to 2 digits extra, which is fine */
        /* it also helps for using apn_addmul_one in-place as it expects */
        /* one extra digit in the result, can clamp later */

        ap_size_t digits = (str_len / 19) + 2;

        op1->num = (ap_dig_t*)apac_malloc(sizeof(ap_dig_t) * digits);

        if (!(op1->num)) { result = APAC_OOM; goto func_end; }
    
        apn_set(op1->num, digits, 0);

        
    }
    else
    {

    }

func_end:
    return result;
}
