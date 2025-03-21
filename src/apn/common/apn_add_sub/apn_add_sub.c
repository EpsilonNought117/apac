#include "apn_add_sub.h"

extern ap_seg_t _apn_add_n(ap_seg_t* result, ap_seg_t* op1, ap_seg_t* op2, ap_size_t size);

extern ap_seg_t _apn_sub_n(ap_seg_t* result, ap_seg_t* op1, ap_seg_t* op2, ap_size_t size);

ap_seg_t apn_add_n(ap_seg_t* result, ap_seg_t* op1, ap_seg_t* op2, ap_size_t size)
{
	return _apn_add_n(result, op1, op2, size);
}

ap_seg_t apn_add(ap_seg_t* result, ap_seg_t* op1, ap_seg_t* op2, ap_size_t size1, ap_size_t size2)
{
	ap_seg_t* max_ptr, * min_ptr;
	ap_size_t max_size, min_size;

	if (size1 >= size2)
	{
		max_ptr = op1;
		min_ptr = op2;
		max_size = size1;
		min_size = size2;
	}
	else
	{
		max_ptr = op2;
		min_ptr = op1;
		max_size = size2;
		min_size = size1;
	}

	ap_seg_t carry = _apn_add_n(result, max_ptr, min_ptr, min_size);

	
}