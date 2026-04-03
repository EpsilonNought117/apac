#if defined(_M_X64)   || defined(_M_AMD64)   ||	\
	defined(__x86_64) || defined(__x86_64__) ||	\
	defined(__amd64) || defined(__amd64__)

#ifndef X64_HIDDEN_FUNCS
#define X64_HIDDEN_FUNCS

#include "../../../include/apac.h"

/* --------------------- ADD SINGLE-LIMB TO APN-ARR FUNCTIONS ------------------------- */

extern ap_dig_t add_one_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t val
);

extern ap_dig_t add_one_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t val
);

/* -------------------- SUB SINGLE-LIMB FROM APN-ARR FUNCTIONS ------------------------ */

extern ap_dig_t sub_one_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t val
);

extern ap_dig_t sub_one_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t val
);

/* ----------------------------- ADDITION FUNCTIONS ----------------------------------- */

extern ap_dig_t add_n_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size
);

extern ap_dig_t add_n_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size
);

/* ---------------------------- SUBTRACTION FUNCTIONS --------------------------------- */

extern ap_dig_t sub_n_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size
);

extern ap_dig_t sub_n_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size
);

/* ------------------------------- NEGATION FUNCTIONS --------------------------------- */

extern void neg_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
);

extern void neg_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
);

/* ------------------------ BASECASE MULTIPLICATION FUNCTIONS ------------------------- */

extern void mul_bc_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size1,
	ap_size_t size2
);

extern void mul_bc_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size1,
	ap_size_t size2
);

/* ------------------- APN-ARR TO ONE-LIMB MULTIPLICATION FUNCTIONS ------------------- */

extern ap_dig_t addmul_one_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t val
);

extern ap_dig_t addmul_one_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t val
);

extern ap_dig_t submul_one_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t val
);

extern ap_dig_t submul_one_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t val
);

/* --------------------------- BASECASE SQUARING FUNCTIONS ---------------------------- */

extern void sqr_bc_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
);

extern void sqr_bc_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
);

/* -------------------------------- COPYING FUNCTIONS --------------------------------- */

extern void cpy_avx512f_4unroll(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
);

extern void cpy_avx_4unroll(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
);

extern void cpy_sse2_4unroll(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size
);

/* ------------------------------- SET TO VAL FUNCTIONS ------------------------------- */

extern void set_avx512f_4unroll(
	ap_dig_t* result,
	ap_size_t size,
	ap_dig_t val
);

extern void set_avx_4unroll(
	ap_dig_t* result,
	ap_size_t size,
	ap_dig_t val
);

extern void set_sse2_4unroll(
	ap_dig_t* result,
	ap_size_t size,
	ap_dig_t val
);

/* ---------------------------- BIT-SHIFTING FUNCTIONS -------------------------------- */

extern ap_dig_t lshift_lt64_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t bit_cnt
);

extern ap_dig_t lshift_lt64_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t bit_cnt
);

extern ap_dig_t rshift_lt64_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t bit_cnt
);

extern ap_dig_t rshift_lt64_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	ap_size_t size,
	ap_dig_t bit_cnt
);

/* ----------------------- LSHIFT-ADD/SUB COMBINED FUNCTIONS -------------------------- */

extern ap_dig_t lshift_lt64_add_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t,
	ap_dig_t
);

extern ap_dig_t lshift_lt64_add_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t,
	ap_dig_t
);

extern ap_dig_t lshift_lt64_sub_zen4(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t,
	ap_dig_t
);

extern ap_dig_t lshift_lt64_sub_x64(
	ap_dig_t* result,
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t,
	ap_dig_t
);

/* ------------------------------- COMPARISION FUNCTION ------------------------------- */

extern int cmp_avx2_4unroll(
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size
);

extern int cmp_sse2_4unroll(
	const ap_dig_t* op1,
	const ap_dig_t* op2,
	ap_size_t size
);

/* ---------------------------- CHECK-IF-ZERO FUNCTION -------------------------------- */

extern int is_zero_avx512f_4unroll(
	const ap_dig_t* op1,
	ap_size_t size
);

extern int is_zero_avx2_4unroll(
	const ap_dig_t* op1,
	ap_size_t size
);

extern int is_zero_sse2_4unroll(
	const ap_dig_t* op1,
	ap_size_t size
);

#endif

#endif