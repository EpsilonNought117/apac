#include "../../../../include/apac.h"

/* ASM FUNCTIONS */

extern u8 _adc_add_n(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 _sbb_sub_n(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 _adc_add_one(u64* result, const u64* op1, u64 size, u64 val);
extern u8 _sbb_sub_one(u64* result, const u64* op1, u64 size, u64 val);

/* PUBLIC API FUNCTIONS */
u8 apn_add_n(u64* result, const u64* op1, const u64* op2, u64 size)
{
	return _adc_add_n(result, op1, op2, size);
}

u8 apn_add(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
{
	u64 max_size = (size1 < size2 ? size2 : size1);
	u64 min_size = (size1 < size2 ? size1 : size2);
	u64* max_ptr = (size1 < size2 ? op2 : op1);
	
	u8 carry = _adc_add_n(result, op1, op2, min_size);

	return _adc_add_one(&result[min_size], &max_ptr[min_size], max_size - min_size, (u64)carry);
}

u8 apn_add_one(u64* result, const u64* op1, u64 size, u64 val)
{
	return _adc_add_one(result, op1, size, val);
}





