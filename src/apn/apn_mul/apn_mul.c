#include "../../../include/apac.h"
#include "../apn_thresholds.h"

extern void _apn_mul_basecase(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

void apn_mul_n(u64* result, const u64* op1, const u64* op2, u64 size)
{
	_apn_mul_basecase(result, op1, op2, size, size);
	return; 
}
