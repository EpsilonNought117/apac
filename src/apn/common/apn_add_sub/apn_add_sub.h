#ifndef APN_ADD_SUB_H
#define APN_ADD_SUB_H

#include "../../apn_def.h"
#include "../../../apacExport.h"

/**
* Adds two operand arrays of type ap_seg_t and stores them in result and returns carry
* 
* Input -> result, operand1, operand2, size (same for both operands)
* Output -> Carry out resulting from multiprecision add (0 or 1)
* NOTE -> Does not perform any memory allocation in result and assumes that result has (size) segments of space
*/
APAC_API ap_seg_t apn_add_n(ap_seg_t* result, ap_seg_t* op1, ap_seg_t* op2, ap_size_t size);

/**
* Adds two operand arrays of type ap_seg_t and stores them in result and returns carry
* 
* Input -> result, operand1, operand2, size1, size2
* Output -> Carry out resulting from multiprecision add (0 or 1)
* NOTE -> Does not perform any memory allocation in result and assumes that result has max(size1, size2) segments of space
*/
APAC_API ap_seg_t apn_add(ap_seg_t* result, ap_seg_t* op1, ap_seg_t* op2, ap_size_t size1, ap_size_t size2);

/**
* Adds one integer value of type ap_seg_t to and operand array of type ap_seg_t to and stores in result, returning any carry
* 
* Input -> result, operand1, val, size (of operand1)
* Output -> Carry out
* NOTE -> Does not perform any memory allocation in result and assumes that result has (size) segments of space
*/
APAC_API ap_seg_t apn_add_one(ap_seg_t* result, ap_seg_t* op1, ap_size_t size, ap_seg_t val);

APAC_API ap_seg_t apn_sub_n(ap_seg_t* result, ap_seg_t* op1, ap_seg_t* op2, ap_size_t size);

APAC_API ap_seg_t apn_sub(ap_seg_t* result, ap_seg_t* op1, ap_seg_t* op2, ap_size_t size1, ap_size_t size2);

APAC_API ap_seg_t apn_sub_one(ap_seg_t* result, ap_seg_t* op1, ap_size_t size, ap_seg_t val);

APAC_API ap_seg_t apn_negate(ap_seg_t* result, ap_seg_t* op1, ap_size_t size);

#endif
