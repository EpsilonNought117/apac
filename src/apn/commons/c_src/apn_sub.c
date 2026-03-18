#include "../../../../include/apac.h"

extern apac_cpu_params curr_cpu;

ap_dig_t apn_sub_n(
	ap_dig_t* result, 
	const ap_dig_t* op1, 
	const ap_dig_t* op2, 
	ap_size_t size
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_PARTIAL_OVERLAP_ABOVE(result, size, op1, size);
	APAC_PARTIAL_OVERLAP_ABOVE(result, size, op2, size);
	APAC_ASSERT(curr_cpu.apn_sub_n_ptr != NULL);

	ap_dig_t borrow = curr_cpu.apn_sub_n_ptr(result, op1, op2, size);
	return borrow;
}

ap_dig_t apn_sub(
	ap_dig_t* result, 
	const ap_dig_t* op1, 
	const ap_dig_t* op2, 
	ap_size_t size1, 
	ap_size_t size2
)
{
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_PARTIAL_OVERLAP_ABOVE(result, size1, op1, size1);
	APAC_PARTIAL_OVERLAP_ABOVE(result, size1, op2, size2);
	APAC_ASSERT(curr_cpu.apn_sub_n_ptr != NULL);
	APAC_ASSERT(curr_cpu.apn_sub_one_ptr != NULL);

	ap_dig_t borrow = curr_cpu.apn_sub_n_ptr(result, op1, op2, size2);

	if (size1 == size2)
		return borrow;

	borrow = curr_cpu.apn_sub_one_ptr(&result[size2], &op1[size2], size1 - size2, borrow);
	return borrow;
}

ap_dig_t apn_sub_one(
	ap_dig_t* result, 
	const ap_dig_t* op1, 
	ap_size_t size, 
	ap_dig_t val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);
	APAC_PARTIAL_OVERLAP_ABOVE(result, size, op1, size);
	APAC_ASSERT(curr_cpu.apn_sub_one_ptr != NULL);

	ap_dig_t borrow = curr_cpu.apn_sub_one_ptr(result, op1, size, val);
	return borrow;
}