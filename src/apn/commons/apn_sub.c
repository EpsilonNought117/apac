#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

apn_seg apn_sub_n(
	apn_seg* result, 
	const apn_seg* op1, 
	const apn_seg* op2, 
	apn_size size
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(curr_cpu.apn_sub_n_ptr != NULL);

	apn_seg borrow = curr_cpu.apn_sub_n_ptr(result, op1, op2, size);
	return borrow;
}

apn_seg apn_sub(
	apn_seg* result, 
	const apn_seg* op1, 
	const apn_seg* op2, 
	apn_size size1, 
	apn_size size2
)
{
	APAC_ASSERT(size1 != 0);
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(curr_cpu.apn_sub_n_ptr != NULL);

	apn_seg borrow = curr_cpu.apn_sub_n_ptr(result, op1, op2, size2);

	if (size1 == size2)
		return borrow;

	borrow = curr_cpu.apn_sub_one_ptr(&result[size2], &op1[size2], size1 - size2, borrow);
	return borrow;
}

apn_seg apn_sub_one(
	apn_seg* result, 
	const apn_seg* op1, 
	apn_size size, 
	apn_seg val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(curr_cpu.apn_sub_one_ptr != NULL);

	apn_seg borrow = curr_cpu.apn_sub_one_ptr(result, op1, size, val);
	return borrow;
}