#if defined(_M_ARM64) || defined(__aarch64__) || defined(__arm64__)

#ifndef ARM64_HIDDEN_FUNCS
#define ARM64_HIDDEN_FUNCS

#include "../../../include/apac.h"

/* --------------------- ADD SINGLE-LIMB TO APN-ARR FUNCTIONS ------------------------- */

extern apn_seg_t add_one_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

/* -------------------- SUB SINGLE-LIMB FROM APN-ARR FUNCTIONS ------------------------ */

extern apn_seg_t sub_one_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

/* ----------------------------- ADDITION FUNCTIONS ----------------------------------- */

extern apn_seg_t add_n_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
);

/* ---------------------------- SUBTRACTION FUNCTIONS --------------------------------- */

extern apn_seg_t sub_n_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
);

/* ------------------------------- NEGATION FUNCTIONS --------------------------------- */

extern void neg_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

/* ------------------------ BASECASE MULTIPLICATION FUNCTIONS ------------------------- */

extern void mul_bc_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size1,
	apn_size_t size2
);

/* ------------------- APN-ARR TO ONE-LIMB MULTIPLICATION FUNCTIONS ------------------- */

extern apn_seg_t addmul_one_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

extern apn_seg_t submul_one_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t val
);

/* --------------------------- BASECASE SQUARING FUNCTIONS ---------------------------- */

extern void sqr_bc_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

/* -------------------------------- COPYING FUNCTIONS --------------------------------- */

extern void cpy_asimd_4unroll(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size
);

/* ------------------------------- SET TO VAL FUNCTIONS ------------------------------- */

extern void set_asimd_4unroll(
	apn_seg_t* result,
	apn_size_t size,
	apn_seg_t val
);

/* ---------------------------- BIT-SHIFTING FUNCTIONS -------------------------------- */

extern apn_seg_t lshift_lt64_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t bit_cnt
);

extern apn_seg_t rshift_lt64_arm64(
	apn_seg_t* result,
	const apn_seg_t* op1,
	apn_size_t size,
	apn_seg_t bit_cnt
);

/* ------------------------------- COMPARISION FUNCTION ------------------------------- */

extern int cmp_asimd_4unroll(
	const apn_seg_t* op1,
	const apn_seg_t* op2,
	apn_size_t size
);

/* ---------------------------- CHECK-IF-ZERO FUNCTION -------------------------------- */

extern int is_zero_asimd_4unroll(
	const apn_seg_t* op1,
	apn_size_t size
);

#endif

#endif