#include "../../../include/apac.h"

void set_zen4(apn_seg* result, apn_size size, apn_seg val)
{
	__m512i my_val = _mm512_set1_epi64(val);

	apn_size blocks = size & ((apn_size)-32);
	apn_size counter = 0;

	while (counter < blocks)
	{
		_mm512_storeu_epi64(&result[counter], my_val);
		_mm512_storeu_epi64(&result[counter + 8], my_val);
		_mm512_storeu_epi64(&result[counter + 16], my_val);
		_mm512_storeu_epi64(&result[counter + 24], my_val);
		counter += 32;
	}

	apn_size remaining = size - counter;
	apn_size limb_mask = _bzhi_u64(~0ULL, remaining);

	_mm512_mask_storeu_epi64(&result[counter], (unsigned char)(limb_mask), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 8], (unsigned char)(limb_mask >> 8), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 16], (unsigned char)(limb_mask >> 16), my_val);
	_mm512_mask_storeu_epi64(&result[counter + 24], (unsigned char)(limb_mask >> 24), my_val);

	return;
}

void set_x64(apn_seg* result, apn_size size, apn_seg val)
{
	__m128i my_val = _mm_set1_epi64x(val);

	apn_size blocks = size & ((apn_size)-8);
	apn_size counter = 0;

	while (counter < blocks)
	{
		_mm_storeu_si128((__m128i*)(&result[counter]), my_val);
		_mm_storeu_si128((__m128i*)(&result[counter + 2]), my_val);
		_mm_storeu_si128((__m128i*)(&result[counter + 4]), my_val);
		_mm_storeu_si128((__m128i*)(&result[counter + 6]), my_val);

		counter += 8;
	}

	while (counter < size)
	{
		result[counter] = val;
		counter++;
	}

	return;
}
