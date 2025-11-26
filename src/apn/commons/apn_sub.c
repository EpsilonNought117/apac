#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

apn_seg_t apn_sub_n(
	apn_seg_t* result, 
	const apn_seg_t* op1, 
	const apn_seg_t* op2, 
	apn_size_t size
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(curr_cpu.apn_sub_n_ptr != NULL);

	apn_seg_t borrow = curr_cpu.apn_sub_n_ptr(result, op1, op2, size);
	return borrow;
}

apn_seg_t apn_sub(
	apn_seg_t* result, 
	const apn_seg_t* op1, 
	const apn_seg_t* op2, 
	apn_size_t size1, 
	apn_size_t size2
)
{
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_ASSERT(curr_cpu.apn_sub_n_ptr != NULL);

	apn_seg_t borrow = curr_cpu.apn_sub_n_ptr(result, op1, op2, size2);

	if (size1 == size2)
		return borrow;

	borrow = curr_cpu.apn_sub_one_ptr(&result[size2], &op1[size2], size1 - size2, borrow);
	return borrow;
}

apn_seg_t apn_sub_one(
	apn_seg_t* result, 
	const apn_seg_t* op1, 
	apn_size_t size, 
	apn_seg_t val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(curr_cpu.apn_sub_one_ptr != NULL);

	apn_seg_t borrow = curr_cpu.apn_sub_one_ptr(result, op1, size, val);
	return borrow;
}