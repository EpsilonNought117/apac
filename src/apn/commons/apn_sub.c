#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

extern apn_seg sub_n_one_x64(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);

extern apn_seg sub_n_one_till_borrow_x64(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);

apn_seg apn_sub_n(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	if (curr_cpu.apn_sub_n_ptr == NULL)
		apacGetCPUSpec();

	return curr_cpu.apn_sub_n_ptr(result, op1, op2, size);
}

apn_seg apn_sub(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size1, apn_size size2)
{
	APAC_ASSERT(size1 != 0);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);

	if (curr_cpu.apn_sub_n_ptr == NULL)
		apacGetCPUSpec();

	apn_seg borrow = curr_cpu.apn_sub_n_ptr(result, op1, op2, size2);

	if (size1 == size2)
		return borrow;

	if (result == op1)
		return sub_n_one_till_borrow_x64(&result[size2], &op1[size2], size1 - size2, borrow);

	return sub_n_one_x64(&result[size2], &op1[size2], size1 - size2, borrow);
}

apn_seg apn_sub_one(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);

	if (result == op1)
		return sub_n_one_till_borrow_x64(result, op1, size, val);

	return sub_n_one_x64(result, op1, size, val);
}