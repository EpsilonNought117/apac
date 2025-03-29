#ifndef APN_NEG_H
#define APN_NEG_H

#include "../../apn_def.h"

/**
* Computes the two's complement (negation) of a big integer stored as 64-bit segments.
*
* Parameters:
*   - op1: Input/output operand array representing the big integer.
*   - size: Number of segments in op1.
*
* Note:
*   - Modifies op1 in place.
*/
APAC_API void apn_negate(u64* op1, u64 size);

#endif