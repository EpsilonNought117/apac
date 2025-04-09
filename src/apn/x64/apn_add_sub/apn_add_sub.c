#include "../../../../include/apac.h"

/* ASM FUNCTIONS */

extern u8 _adc_add_n(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 _sbb_sub_n(u64* result, const u64* op1, const u64* op2, u64 size);

/* PUBLIC API FUNCTIONS */
u8 apn_add_n(u64* result, const u64* op1, const u64* op2, u64 size)
{
	APAC_ASSERT(size != 0);

	return _adc_add_n(result, op1, op2, size);
}

u8 apn_add(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
{
	APAC_ASSERT(size1 != 0 && size2 != 0);
	APAC_ASSERT(size1 >= size2);

	u8 carry = _adc_add_n(result, op1, op2, size2);
	u64 counter = size2;

	while (carry && counter < size1)
	{
		carry = _addcarry_u64(carry, op1[counter], 0, &result[counter]);
		counter++;
	}

	if (counter != size1)
	{
		apn_cpy(&result[counter], &op1[counter], size1 - counter);
	}
	return carry;
}




