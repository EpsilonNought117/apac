#include "../../../include/apac.h"
#include "../x64/x64_hidden_funcs.h"

extern apac_cpu_params curr_cpu;

apn_seg apn_add_n(
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

	if (curr_cpu.apn_add_n_ptr == NULL)
		apacGetCPUSpec();
	
	apn_seg carry = curr_cpu.apn_add_n_ptr(result, op1, op2, size);
	return carry;
}

apn_seg apn_add(
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

	if (curr_cpu.apn_add_n_ptr == NULL)
		apacGetCPUSpec();

	apn_seg carry = curr_cpu.apn_add_n_ptr(result, op1, op2, size2);

	if (size1 == size2)
		return carry;

	carry = curr_cpu.apn_add_one_ptr(&result[size2], &op1[size2], size1 - size2, carry);
	return carry;
}

apn_seg apn_add_one(
	apn_seg* result, 
	const apn_seg* op1, 
	apn_size size, 
	apn_seg val
)
{
	APAC_ASSERT(size != 0);
	APAC_ASSERT(op1 != NULL);
	APAC_ASSERT(result != NULL);
	
	if (curr_cpu.apn_add_one_ptr == NULL)
		apacGetCPUSpec();

	apn_seg carry = curr_cpu.apn_add_one_ptr(result, op1, size, val);
	return carry;
}