#include "_hidden_mul.h"

extern void _adc_mulx_mul_basecase(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);
extern void	_adc_mul_basecase(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

void _apn_basecase_mul(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
{
	APAC_ASSERT(result != NULL)
	APAC_ASSERT(op1 != NULL)
	APAC_ASSERT(op2 != NULL)
	APAC_ASSERT(size1 != 0)
	APAC_ASSERT(size2 != 0)
	APAC_ASSERT(result != op1)
	APAC_ASSERT(result != op2)

	_adc_mulx_mul_basecase(result, op1, op2, size1, size2);
	return;
}