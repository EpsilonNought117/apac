#ifndef X64_HIDDEN_FUNCS
#define X64_HIDDEN_FUNCS

#include "../../../include/apac.h"

/* ----------------------------- ADDITION FUNCTIONS ----------------------------------- */

extern u8 _add_n_8unroll(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 _add_n_4unroll(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 _add_n_2unroll(u64* result, const u64* op1, const u64* op2, u64 size);

extern u8 _add_n_one(u64* result, const u64* op1, u64 size, u64 val);

extern u8 _add_n_one_till_carry(u64* result, const u64* op1, u64 size, u64 val);

/* ---------------------------- SUBTRACTION FUNCTIONS --------------------------------- */

extern u8 _sub_n_8unroll(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 _sub_n_4unroll(u64* result, const u64* op1, const u64* op2, u64 size);
extern u8 _sub_n_2unroll(u64* result, const u64* op1, const u64* op2, u64 size);

extern u8 _sub_n_one(u64* result, const u64* op1, u64 size, u64 val);

extern u8 _sub_n_one_till_borrow(u64* result, const u64* op1, u64 size, u64 val);

/* ------------------------ BASECASE MULTIPLICATION FUNCTIONS ------------------------- */

extern void _mul_bc(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

extern void _mul_bc_mulx(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

extern void _mul_bc_mulx_adx_2unroll(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);
extern void _mul_bc_mulx_adx_4unroll(u64* result, const u64* op1, const u64* op2, u64 size1, u64 size2);

/* -------------------------------- COPYING FUNCTIONS --------------------------------- */

extern void _cpy_sse2_4unroll(u64* result, const u64* op1, u64 size);

extern void _cpy_avx_4unroll(u64* result, const u64* op1, u64 size);

extern void _cpy_avx512f_4unroll(u64* result, const u64* op1, u64 size);
extern void _cpy_avx512f_2unroll(u64* result, const u64* op1, u64 size);

/* ------------------------------- SET TO VAL FUNCTIONS ------------------------------- */

extern void _set_sse2_4unroll(u64* result, u64 size, u64 val);

extern void _set_avx_4unroll(u64* result, u64 size, u64 val);

extern void _set_avx512f_4unroll(u64* result, u64 size, u64 val);
extern void	_set_avx512f_2unroll(u64* result, u64 size, u64 val);

/* ------------------------------- NEGATION FUNCTIONS --------------------------------- */

extern void _neg_8unroll(u64* result, const u64* op1, u64 size);
extern void _neg_4unroll(u64* result, const u64* op1, u64 size);

#endif