#include "_hidden_mul.h"

extern void _adx_mulx_mul_basecase(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);
extern void _adc_mulx_mul_basecase(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);
extern void	_adc_mul_basecase(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

void (*_apn_mul_basecase_ptr)(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2) = NULL;

void _apn_basecase_mul(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2)
{
	APAC_ASSERT(size2 != 0);
	APAC_ASSERT(size1 >= size2);

	if (_apn_mul_basecase_ptr == NULL)
	{
		if (adx_chk)
		{
			_apn_mul_basecase_ptr = _adx_mulx_mul_basecase;
		}
		else if (bmi2_chk)
		{
			_apn_mul_basecase_ptr = _adc_mulx_mul_basecase;
		}
		else
		{
			_apn_mul_basecase_ptr = _adc_mul_basecase;
		}
	}

	_apn_mul_basecase_ptr(result, op1, op2, size1, size2);
	return;
}
