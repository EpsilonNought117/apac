#ifndef APN_CPY_H
#define APN_CPY_H

#include "../../apn_def.h"

/**
* Copies (size) number of segments from op1 to result.
* 
* Input ->
*/
APAC_API void apn_cpy(ap_seg_t* result, ap_seg_t* op1, ap_size_t size);

#endif