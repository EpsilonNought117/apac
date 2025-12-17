#if defined(_M_X64)   || defined(_M_AMD64)   ||	\
	defined(__x86_64) || defined(__x86_64__) ||	\
	defined(__amd64) || defined(__amd64__)

#ifndef X64_HIDDEN_FUNCS
#define X64_HIDDEN_FUNCS

#include "../../../include/apac.h"

/* --------------------- ADD SINGLE-LIMB TO APN-ARR FUNCTIONS ------------------------- */

extern apn_seg_t add_n_one_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

extern apn_seg_t add_n_one_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

/* -------------------- SUB SINGLE-LIMB FROM APN-ARR FUNCTIONS ------------------------ */

extern apn_seg_t sub_n_one_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

extern apn_seg_t sub_n_one_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

/* ----------------------------- ADDITION FUNCTIONS ----------------------------------- */

extern apn_seg_t add_n_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
);

extern apn_seg_t add_n_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
);

/* ---------------------------- SUBTRACTION FUNCTIONS --------------------------------- */

extern apn_seg_t sub_n_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
);

extern apn_seg_t sub_n_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
);

/* ------------------------------- NEGATION FUNCTIONS --------------------------------- */

extern void neg_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

extern void neg_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

/* ------------------------ BASECASE MULTIPLICATION FUNCTIONS ------------------------- */

extern void mul_bc_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size1,
	apn_size_t size2
);

extern void mul_bc_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size1,
	apn_size_t size2
);

/* ------------------- APN-ARR TO ONE-LIMB MULTIPLICATION FUNCTIONS ------------------- */

extern apn_seg_t addmul_one_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

extern apn_seg_t addmul_one_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

extern apn_seg_t submul_one_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

extern apn_seg_t submul_one_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

/* --------------------------- BASECASE SQUARING FUNCTIONS ---------------------------- */

extern void sqr_bc_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

extern void sqr_bc_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

/* -------------------------------- COPYING FUNCTIONS --------------------------------- */

extern void cpy_avx512f_4unroll(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

extern void cpy_avx_4unroll(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

extern void cpy_sse2_4unroll(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

/* ------------------------------- SET TO VAL FUNCTIONS ------------------------------- */

extern void set_avx512f_4unroll(
	apn_seg_t* result,
	apn_size_t size,
	apn_seg_t val
);

extern void set_avx_4unroll(
	apn_seg_t* result,
	apn_size_t size,
	apn_seg_t val
);

extern void set_sse2_4unroll(
	apn_seg_t* result,
	apn_size_t size,
	apn_seg_t val
);

/* ---------------------------- BIT-SHIFTING FUNCTIONS -------------------------------- */

extern apn_seg_t lshift_lt64_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t bit_cnt
);

extern apn_seg_t lshift_lt64_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t bit_cnt
);

extern apn_seg_t rshift_lt64_zen4(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t bit_cnt
);

extern apn_seg_t rshift_lt64_x64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t bit_cnt
);

/* ------------------------------- COMPARISION FUNCTION ------------------------------- */

extern int cmp_avx2_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
);

extern int cmp_sse2_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
);

/* ---------------------------- CHECK-IF-ZERO FUNCTION -------------------------------- */

extern int is_zero_avx512f_4unroll(
	const apn_seg_t* op1,
	apn_size_t size
);

extern int is_zero_avx2_4unroll(
	const apn_seg_t* op1,
	apn_size_t size
);

extern int is_zero_sse2_4unroll(
	const apn_seg_t* op1,
	apn_size_t size
);

#endif

#endif