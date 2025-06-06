#include "../../../../include/apac.h"

extern __apac_cpu_params curr_cpu;

extern u8 _add_n_one(u64* result, const u64* op1, u64 size, u64 val);

extern u8 _add_n_one_till_carry(u64* result, const u64* op1, u64 size, u64 val);

u8 apn_add_n(u64* result, const u64* op1, const u64* op2, u64 size)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	if (curr_cpu.__apn_add_n_ptr == NULL)
		apacGetCPUSpec();
	
	return curr_cpu.__apn_add_n_ptr(result, op1, op2, size);
}

u8 apn_add(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
{
	APAC_ASSERT(size1 != 0);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	if (curr_cpu.__apn_add_n_ptr == NULL)
		apacGetCPUSpec();

	u8 carry = curr_cpu.__apn_add_n_ptr(result, op1, op2, size2);

	if (size1 == size2)
		return carry;
	
	if (result == op1)
		return _add_n_one_till_carry(&result[size2], &op1[size2], size1 - size2, carry);

	return _add_n_one(&result[size2], &op1[size2], size1 - size2, carry);
}

u8 apn_add_one(u64* result, const u64* op1, u64 size, u64 val)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);

	if (result == op1)
		return _add_n_one_till_carry(result, op1, size, val);

	return _add_n_one(result, op1, size, val);
}