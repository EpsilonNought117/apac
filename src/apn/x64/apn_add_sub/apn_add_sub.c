#include "../../../../include/apac.h"

/* ASM FUNCTIONS */

extern u8 _adc_add_n(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 _sbb_sub_n(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 _adc_add_one(u64* result, const u64* op1, u64 size, u64 val);
extern u8 _sbb_sub_one(u64* result, const u64* op1, u64 size, u64 val);
extern u8 _adc_till_carry(u64* result, const u64* op1, u64 size, u64 val);
extern u8 _sbb_till_borrow(u64* result, const u64* op1, u64 size, u64 val);

/* PUBLIC API FUNCTIONS */

u8 apn_add_n(u64* result, const u64* op1, const u64* op2, u64 size)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	return _adc_add_n(result, op1, op2, size);
}

u8 apn_add(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
{
	APAC_ASSERT(size1 != 0);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	u8 carry = _adc_add_n(result, op1, op2, size2);
	
	if (result == op1)
	{
		return _adc_till_carry(&result[size2], &op1[size2], size1 - size2, carry);
	}

	return _adc_add_one(&result[size2], &op1[size2], size1 - size2, carry);
}

u8 apn_add_one(u64* result, const u64* op1, u64 size, u64 val)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);

	if (result == op1)
	{
		return _adc_till_carry(result, op1, size, val);
	}

	return _adc_add_one(result, op1, size, val);
}

u8 apn_sub_n(u64* result, const u64* op1, const u64* op2, u64 size)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	return _sbb_sub_n(result, op1, op2, size);
}

u8 apn_sub(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
{
	APAC_ASSERT(size1 != 0);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	u8 borrow = _sbb_sub_n(result, op1, op2, size2);

	if (result == op1)
	{
		return _sbb_till_borrow(&result[size2], &op1[size2], size1 - size2, borrow);
	}

	return _sbb_sub_one(&result[size2], &op1[size2], size1 - size2, borrow);
}

u8 apn_sub_one(u64* result, const u64* op1, u64 size, u64 val)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);

	if (result == op1)
	{
		return _sbb_till_borrow(result, op1, size, val);
	}

	return _sbb_sub_one(result, op1, size, val);
}
