#include "../../../include/apac.h"

extern apac_cpu_params curr_cpu;

apn_seg_t apn_add_n(
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
	APAC_PARTIAL_OVERLAP_BELOW(result, size, op1, size);
	APAC_PARTIAL_OVERLAP_BELOW(result, size, op2, size);
	APAC_DETAILED_ASSERT(curr_cpu.apn_add_n_ptr != NULL,
		"apacInit() or apacGetCPUSpec() not invoked!"
	);

	apn_seg_t carry = curr_cpu.apn_add_n_ptr(result, op1, op2, size);
	return carry;
}

apn_seg_t apn_add(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size1,
	apn_size_t size2
)
{
	APAC_ASSERT(size2 != 0);
	APAC_DETAILED_ASSERT(size1 >= size2,
		"Expected size1 >= size2, got size1 (%" PRI_APN_SIZE ") < size2 (%" PRI_APN_SIZE ")\n",
		size1, size2
	);
	APAC_ASSERT(result != NULL);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(op2 != NULL);
	APAC_PARTIAL_OVERLAP_BELOW(result, size1, op1, size1);
	APAC_PARTIAL_OVERLAP_BELOW(result, size1, op2, size2);
	APAC_DETAILED_ASSERT(curr_cpu.apn_add_n_ptr != NULL,
		"apacInit() or apacGetCPUSpec() not invoked!"
	);
	APAC_DETAILED_ASSERT(curr_cpu.apn_add_one_ptr != NULL,
		"apacInit() or apacGetCPUSpec() not invoked!"
	);

	apn_seg_t carry = curr_cpu.apn_add_n_ptr(result, op1, op2, size2);

	if (size1 == size2)
		return carry;

	carry = curr_cpu.apn_add_one_ptr(&result[size2], &op1[size2], size1 - size2, carry);
	return carry;
}

apn_seg_t apn_add_one(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);
	APAC_PARTIAL_OVERLAP_BELOW(result, size, op1, size);
	APAC_DETAILED_ASSERT(curr_cpu.apn_add_one_ptr != NULL,
		"apacInit() or apacGetCPUSpec() not invoked!"
	);
	
	apn_seg_t carry = curr_cpu.apn_add_one_ptr(result, op1, size, val);
	return carry;
}