#ifndef APN_CPY_H
#define APN_CPY_H

#include "../../apn_def.h"

/**
* Copies (size) number of segments from op1 to result.
* 
* Input -> result, op1, size (of both op1 and result)
* Output -> None
* 
* NOTE -> Assumes both result and op1 have (size) amount of segments
*/
APAC_API void apn_cpy(u64* result, const u64* op1, u64 size);

#endif