#include "apn_add_sub.h"

extern u8 _adc_apn_add_n(u64* result, u64* op1, u64* op2, u64 size);

extern u8 _sbb_apn_sub_n(u64* result, u64* op1, u64* op2, u64 size);

u8 apn_add_n(u64* result, u64* op1, u64* op2, u64 size)
{
	return _apn_add_n(result, op1, op2, size);
}

u8 apn_add(u64* result, u64* op1, u64* op2, u64 size1, u64 size2)
{
	u8 carry = 0;

	return carry;
}