#include "../../../../include/apac.h"

extern u8 _apn_add_n(u64* result, u64* op1, u64* op2, u64 size);	// asm code
	
extern u8 _apn_sub_n(u64* result, u64* op1, u64* op2, u64 size);	// asm code

u8 apn_add_n(u64* result, u64* op1, u64* op2, u64 size)
{
	return _apn_add_n(result, op1, op2, size);
}

u8 apn_add(u64* result, u64* op1, u64* op2, u64 size1, u64 size2)
{
	u64 max_size = (size1 < size2 ? size2 : size1);
	u64 min_size = (size1 < size2 ? size1 : size2);
	u64* max_ptr = (size1 < size2 ? op2 : op1);
	
	u8 carry = _apn_add_n(result, op1, op2, min_size);
	u64 counter = min_size;

	while (carry && (counter < max_size))
	{
		carry = _addcarry_u64(carry, 0, max_ptr[counter], &result[counter]);
		counter++;
	}

	apn_cpy(&result[counter], &max_ptr[counter], (max_size - counter));
	return carry;
}

u8 apn_add_one(u64* result, u64* op1, u64 size, u64 val)
{
	u8 carry = _addcarry_u64(0, op1[0], val, &result[0]);
	u8 counter = 1;

	while (carry && (counter < size))
	{
		carry = _addcarry_u64(carry, 0, op1[counter], &result[counter]);
		counter++;
	}

	apn_cpy(&result[counter], &op1[counter], (size - counter));
	return carry;
}



