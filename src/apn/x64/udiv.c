#include "../../../include/apac.h"
#include "x64_hidden_funcs.h"

/**
* Research Paper: Improved Division by Invariant Integers
* Authors: Neils Moller and Torbjorn Granlund
*/
inline apn_seg_t udiv21_x64(
	apn_seg_t divd1,
	apn_seg_t divd0,
	apn_seg_t dvsr,
	apn_seg_t recip,
	apn_seg_t* rmdr
)
{
	APAC_ASSERT(divd1 < dvsr);
	APAC_ASSERT(dvsr & (1ULL << 63));

	// v = recip
	// dvsr = d
	// <u1, u0> = <divd1, divd0>

#if defined(_MSC_VER)

	uint64_t q[2] = { 0 };	// <q1, q0>
	uint64_t r = 0;		
	uint8_t carry = 0;			
	uint64_t high64 = 0, low64 = 0;	// p = high64 * (2 ^ 64) + low64

	// <q1, q0> = v * u1
	q[0] = _umul128(divd1, recip, &q[1]);
	
	// <q1, q0> += <u1, u0>
	carry = _addcarry_u64(carry, q[0], divd0, &q[0]);
	carry = _addcarry_u64(carry, q[1], divd1, &q[1]);
	carry = 0;

	q[1]++; // q1 = (q1 + 1) (mod (2 ^ 64))
	low64 = _umul128(q[1], dvsr, &high64);	//q1 * d
	carry = _subborrow_u64(carry, divd0, low64, &r); // r = (u0 - q1 * d) (mod (2 ^ 64))
	
	if (r > q[0])
	{
		q[1]--; // q1 = (q1 - 1) (mod (2 ^ 64))
		r += dvsr;	// r = (r + d) (mod (2 ^ 64))
	}

	if (r >= dvsr)
	{
		q[1]++;	// q1 += 1
		r -= dvsr;	// r -= d
	}

	uint64_t q1 = q[1];
	*rmdr = (apn_seg_t)r;

#elif defined(__GNUC__) || defined(__clang__)

	// same as MSVC code here

	__uint128_t q = 0;
	uint64_t r = 0;

	q = (__uint128_t)recip * divd1;
	q += (((__uint128_t)divd1 << 64) | ((__uint128_t)divd0));
	
	uint64_t q1 = (uint64_t)(q >> 64);
	uint64_t q0 = (uint64_t)q;
	
	q1++;
	r = (uint64_t)((__uint128_t)divd0 - (__uint128_t)q1 * dvsr);

	if (r > q0)
	{
		q1--;
		r = (uint64_t)((__uint128_t)r + dvsr);
	}

	if (r >= dvsr)
	{
		q1++;
		r -= dvsr;
	}

	*rmdr = (apn_seg_t)r;

#else
	#error "Unknown Compiler!"
#endif

	return (apn_seg_t)q1;
}

/**
* Research Paper: Improved Division by Invariant Integers
* Authors: Neils Moller and Torbjorn Granlund
*/
inline apn_seg_t udiv32_quot_x64(
	apn_seg_t divd2,
	apn_seg_t divd1,
	apn_seg_t divd0,
	apn_seg_t dvsr1,
	apn_seg_t dvsr0,
	apn_seg_t recip
)
{
	APAC_ASSERT((dvsr1 > divd2) || (dvsr1 == divd2 && dvsr0 > divd1));
	APAC_ASSERT(dvsr1 & (1ULL << 63));

	// <u2, u1, u0> = <divd2, divd1, divd0>
	// <d1, d0> = <dvsr1, dvsr0>
	// v = recip

#if defined(_MSC_VER)

	apn_seg_t q[2] = { 0 };
	apn_seg_t r[2] = { 0 };
	uint8_t carry = 0;

	q[0] = _umul128(recip, divd2, &q[1]);
	carry = _addcarry_u64(carry, q[0], divd1, &q[0]);
	carry = _addcarry_u64(carry, q[1], divd2, &q[1]);

	uint64_t high64 = 0, low64 = 0;

	carry = 0;
	low64 = _umul128(divd1, dvsr1, &high64);
	
	/* carry now acts as borrow */
	// r1 = (u1 - q1 * d1) (mod 2 ^ 64) 
	carry = _subborrow_u64(carry, r[0], low64, &r[0]);
	carry = _subborrow_u64(carry, r[1], high64, &r[1]);

	uint64_t t[2] = { 0 };

	// <t1, t0> = d0 * q1
	t[0] = _umul128(divd0, q[1], &t[1]);
	carry = 0;

	carry = _subborrow_u64(carry, divd0, t[0], &low64);
	carry = _subborrow_u64(carry, r[1], t[1], &high64);

	carry = 0;

	carry = _subborrow_u64(carry, low64, dvsr0, &r[0]);
	carry = _subborrow_u64(carry, high64, dvsr1, &r[1]);

	q[1]++;

	if (r[1] >= q[0])
	{
		q[1]--;

		carry = 0;
		carry = _addcarry_u64(carry, r[0], dvsr0, &r[0]);
		carry = _addcarry_u64(carry, r[1], dvsr1, &r[1]);
	}

	if ((r[1] > dvsr1) || (r[0] > dvsr0) || (r[1] == dvsr1 && r[0] == dvsr0))
	{
		q[1]++;
		
		carry = 0;
		carry = _subborrow_u64(carry, r[0], dvsr0, &r[0]);
		carry = _subborrow_u64(carry, r[1], dvsr1, &r[1]);
	}

	apn_seg_t q1 = (apn_seg_t)q[1];

#elif defined(__GNUC__) || defined(__clang__)

	__uint128_t q = 0, r = 0;
	
	q = (__uint128_t)recip * divd2;
	q += (((__uint128_t)divd2 << 64) | (__uint128_t)divd1);		// <q1, q0> += <u2, u1>
	r = (__uint128_t)divd1 - (q >> 64) * dvsr1;					// r = (u1 - q1 * d1) (mod 2 ^ 64) 
	__uint128_t t = (q >> 64) * dvsr0;							// <t1, t0> = d0 * q1
	
	uint64_t r1 = (r >> 64);
	__uint128_t d = (((__uint128_t)dvsr1 << 64) | (__uint128_t)dvsr0);
	r = (((__uint128_t)r1 << 64) | (__uint128_t)divd0) - t - d;
	
	uint64_t q1 = (uint64_t)(q >> 64);
	uint64_t q0 = (uint64_t)q;

	if (r1 >= q0)
	{
		q1--;
		r += d;
	}

	if (r >= d)
	{
		q1++;
		r -= d;
	}

#else
	#error "Unknown Compiler!"
#endif

	return (apn_seg_t)q1;
}