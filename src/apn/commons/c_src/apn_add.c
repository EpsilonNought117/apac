#include "../../../../include/apac.h"

extern apac_cpu_params curr_cpu;

ap_seg_t apn_add_n(
	ap_seg_t* result,
	const ap_seg_t* op1,
	const ap_seg_t* op2,
	ap_size_t size
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_PARTIAL_OVERLAP_BELOW(result, size, op1, size);
	APAC_PARTIAL_OVERLAP_BELOW(result, size, op2, size);
	APAC_ASSERT(curr_cpu.apn_add_n_ptr != NULL);

	ap_seg_t carry = curr_cpu.apn_add_n_ptr(result, op1, op2, size);
	return carry;
}

ap_seg_t apn_add(
	ap_seg_t* result,
	const ap_seg_t* op1,
	const ap_seg_t* op2,
	ap_size_t size1,
	ap_size_t size2
)
{
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_PARTIAL_OVERLAP_BELOW(result, size1, op1, size1);
	APAC_PARTIAL_OVERLAP_BELOW(result, size1, op2, size2);
	APAC_ASSERT(curr_cpu.apn_add_n_ptr != NULL);
	APAC_ASSERT(curr_cpu.apn_add_one_ptr != NULL);

	ap_seg_t carry = curr_cpu.apn_add_n_ptr(result, op1, op2, size2);

	if (size1 == size2)
		return carry;

	carry = curr_cpu.apn_add_one_ptr(&result[size2], &op1[size2], size1 - size2, carry);
	return carry;
}

ap_seg_t apn_add_one(
	ap_seg_t* result,
	const ap_seg_t* op1,
	ap_size_t size,
	ap_seg_t val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);
	APAC_PARTIAL_OVERLAP_BELOW(result, size, op1, size);
	APAC_ASSERT(curr_cpu.apn_add_one_ptr != NULL);
	
	ap_seg_t carry = curr_cpu.apn_add_one_ptr(result, op1, size, val);
	return carry;
}