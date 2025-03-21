#ifndef APN_CPY_H
#define APN_CPY_H

#include "../../apn_def.h"

/**
* Copies (size) number of segments from op1 to result.
* 
* Input ->
*/
APAC_API void apn_cpy(u64* result, u64* op1, u64 size);

#endif