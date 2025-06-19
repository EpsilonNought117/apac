#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

extern u8 sub_n_one(u64* result, const u64* op1, u64 size, u64 val);

extern u8 sub_n_one_till_borrow(u64* result, const u64* op1, u64 size, u64 val);

u8 apn_sub_n(u64* result, const u64* op1, const u64* op2, u64 size)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	if (curr_cpu.apn_sub_n_ptr == NULL)
		apacGetCPUSpec();

	return curr_cpu.apn_sub_n_ptr(result, op1, op2, size);
}

u8 apn_sub(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
{
	APAC_ASSERT(size1 != 0);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	if (curr_cpu.apn_sub_n_ptr == NULL)
		apacGetCPUSpec();

	u8 borrow = curr_cpu.apn_sub_n_ptr(result, op1, op2, size2);

	if (size1 == size2)
		return borrow;

	if (result == op1)
		return sub_n_one_till_borrow(&result[size2], &op1[size2], size1 - size2, borrow);

	return sub_n_one(&result[size2], &op1[size2], size1 - size2, borrow);
}

u8 apn_sub_one(u64* result, const u64* op1, u64 size, u64 val)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);

	if (result == op1)
		return sub_n_one_till_borrow(result, op1, size, val);

	return sub_n_one(result, op1, size, val);
}