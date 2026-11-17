#include "../x64_hidden_funcs.h"

// Tested on Ryzen 7 8845HS running at 3.8 GHz Base Clock
void zen4_set_params(void)
{
	KARATSUBA_MUL_THRESHOLD  = (apn_size_t)(30);
	KARATSUBA_SQR_THRESHOLD  = (apn_size_t)(55);
	DNC_DIV_THRESHOLD        = (apn_size_t)(32);
}