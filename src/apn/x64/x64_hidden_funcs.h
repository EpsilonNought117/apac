#if defined(_M_X64)   || defined(_M_AMD64)   ||		\
	defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__)

#ifndef X64_HIDDEN_FUNCS
#define X64_HIDDEN_FUNCS

#include "../../../include/apac.h"

/* --------------------- ADD SINGLE-LIMB TO APN-ARR FUNCTIONS ------------------------- */

extern apn_seg add_n_one_zen4(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);
extern apn_seg add_n_one_x64(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);

/* -------------------- SUB SINGLE-LIMB FROM APN-ARR FUNCTIONS ------------------------ */

extern apn_seg sub_n_one_zen4(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);
extern apn_seg sub_n_one_x64(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);

/* ----------------------------- ADDITION FUNCTIONS ----------------------------------- */

extern apn_seg add_n_zen4(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size);
extern apn_seg add_n_x64(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size);

/* ---------------------------- SUBTRACTION FUNCTIONS --------------------------------- */

extern apn_seg sub_n_zen4(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size);
extern apn_seg sub_n_x64(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size);

/* ------------------------------- NEGATION FUNCTIONS --------------------------------- */

extern void neg_zen4(apn_seg* result, const apn_seg* op1, apn_size size);
extern void neg_x64(apn_seg* result, const apn_seg* op1, apn_size size);

/* ------------------------ BASECASE MULTIPLICATION FUNCTIONS ------------------------- */

extern void mul_bc_zen4(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size1, apn_size size2);
extern void mul_bc_x64(apn_seg* result, const apn_seg* op1, const apn_seg* op2, apn_size size1, apn_size size2);

/* ------------------- APN-ARR TO ONE-LIMB MULTIPLICATION FUNCTIONS ------------------- */

extern apn_seg addmul_one_zen4(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);
extern apn_seg addmul_one_x64(apn_seg* result, const apn_seg* op1, apn_size size, apn_seg val);

/* --------------------------- BASECASE SQUARING FUNCTIONS ---------------------------- */

extern void sqr_bc_zen4(apn_seg* result, const apn_seg* op1, apn_size size);
extern void sqr_bc_x64(apn_seg* result, const apn_seg* op1, apn_size size);

/* -------------------------------- COPYING FUNCTIONS --------------------------------- */

extern void cpy_avx512f_4unroll(apn_seg* result, const apn_seg* op1, apn_size size);
extern void cpy_avx512f_2unroll(apn_seg* result, const apn_seg* op1, apn_size size);
extern void cpy_avx_4unroll(apn_seg* result, const apn_seg* op1, apn_size size);
extern void cpy_avx_2unroll(apn_seg* result, const apn_seg* op1, apn_size size);
extern void cpy_sse2_4unroll(apn_seg* result, const apn_seg* op1, apn_size size);

/* ------------------------------- SET TO VAL FUNCTIONS ------------------------------- */

extern void set_avx512f_4unroll(apn_seg* result, apn_size size, apn_seg val);
extern void set_avx512f_2unroll(apn_seg* result, apn_size size, apn_seg val);
extern void set_avx_4unroll(apn_seg* result, apn_size size, apn_seg val);
extern void set_avx_2unroll(apn_seg* result, apn_size size, apn_seg val);
extern void set_sse2_4unroll(apn_seg* result, apn_size size, apn_seg val);

/* ----------------------------- COMPARISION FUNCTIONS -------------------------------- */

#endif

#endif