#ifndef X64_HIDDEN_FUNCS
#define X64_HIDDEN_FUNCS

#include "../../../include/apac.h"

/* ----------------------------- ADDITION FUNCTIONS ----------------------------------- */

extern u8 add_n_8unroll(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 add_n_4unroll(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 add_n_2unroll(u64* result, const u64* op1, const u64* op2, u64 size);

extern u8 add_n_one(u64* result, const u64* op1, u64 size, u64 val);

extern u8 add_n_one_till_carry(u64* result, const u64* op1, u64 size, u64 val);

/* ---------------------------- SUBTRACTION FUNCTIONS --------------------------------- */

extern u8 sub_n_8unroll(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 sub_n_4unroll(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 sub_n_2unroll(u64* result, const u64* op1, const u64* op2, u64 size);

extern u8 sub_n_one(u64* result, const u64* op1, u64 size, u64 val);

extern u8 sub_n_one_till_borrow(u64* result, const u64* op1, u64 size, u64 val);

/* ------------------------ BASECASE MULTIPLICATION FUNCTIONS ------------------------- */

extern void mul_bc(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

extern void mul_bc_mulx(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

extern void mul_bc_mulx_adx_2unroll(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);
extern void mul_bc_mulx_adx_4unroll(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);
extern void mul_bc_mulx_adx_8unroll(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/* -------------------------------- COPYING FUNCTIONS --------------------------------- */

extern void cpy_sse2_4unroll(u64* result, const u64* op1, u64 size);

extern void cpy_avx_4unroll(u64* result, const u64* op1, u64 size);

extern void cpy_avx512f_4unroll(u64* result, const u64* op1, u64 size);
extern void cpy_avx512f_2unroll(u64* result, const u64* op1, u64 size);

/* ------------------------------- SET TO VAL FUNCTIONS ------------------------------- */

extern void set_sse2_4unroll(u64* result, u64 size, u64 val);

extern void set_avx_4unroll(u64* result, u64 size, u64 val);

extern void set_avx512f_4unroll(u64* result, u64 size, u64 val);
extern void	set_avx512f_2unroll(u64* result, u64 size, u64 val);

/* ------------------------------- NEGATION FUNCTIONS --------------------------------- */

extern void neg_8unroll(u64* result, const u64* op1, u64 size);
extern void neg_4unroll(u64* result, const u64* op1, u64 size);

#endif