
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                COMBINED ADD/SUB AND LSHIFT LESS THAN 64 FUNCS             |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

    #   Function Arguments (System V ABI)
    #
    #   rdi -> result   (ap_dig_t*)
    #   rsi -> op1      (const ap_dig_t*)
    #   rdx -> op2      (const ap_dig_t*)
    #   rcx -> size     (ap_size_t)
    #   r8  -> bit_cnt  (ap_dig_t)

    # result = op1 +- (op2 * (2^bit_cnt))
    # returns shift_out val +- carry/borrow

.intel_syntax noprefix
.text

.globl lshift_lt64_add_x64, lshift_lt64_add_zen4, lshift_lt64_sub_x64, lshift_lt64_sub_zen4
.type  lshift_lt64_add_x64,  @function
.type  lshift_lt64_add_zen4, @function
.type  lshift_lt64_sub_x64,  @function
.type  lshift_lt64_sub_zen4, @function

# ---------------------------------------------------------------------------
# GEN_LSHIFT_LT64_ZEN4 op, first_instr, carry_instr
#   op           -> function name prefix                   (add / sub)
#   first_instr  -> first-limb add/sub instruction         (add / sub)
#   carry_instr  -> carry-propagation instruction          (adc / sbb)
# ---------------------------------------------------------------------------

.macro GEN_LSHIFT_LT64_ZEN4 op, first_instr, carry_instr

lshift_lt64_\op\()_zen4:
.cfi_startproc
    push    r12
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r12, 0
    push    r13
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r13, 0

    # lshift value in r8
    # rshift value in r9
    mov     r9, 64
    sub     r9, r8
    xor     rax, rax                        

.L\op\()_zen4_before_loop:

    shlx    r10, QWORD PTR [rdx], r8
    mov     r13, QWORD PTR [rsi]
    \first_instr r13, r10
    mov     QWORD PTR [rdi], r13
    dec     rcx
    
    jz      .L\op\()_zen4_after_loop
    setc    al                            

    mov     r12, rcx
    and     r12, 3
    shr     rcx, 2
    
    bt      ax,  0
    jz      .L\op\()_zen4_before_rmdr_loop

.p2align 4
.L\op\()_zen4_unroll_4x_loop:

.set i, 0
.rept 4

    mov     r13, QWORD PTR [rsi + i * 8 + 8]
    shrx    r10, QWORD PTR [rdx + i * 8], r9
    shlx    r11, QWORD PTR [rdx + i * 8 + 8], r8
    lea     r11, [r11 + r10]
    \carry_instr r13, r11
    mov     QWORD PTR [rdi + i * 8 + 8], r13

.set i, i + 1
.endr
    
    lea     rsi, [rsi + 32]
    lea     rdx, [rdx + 32]
    lea     rdi, [rdi + 32]
    
    dec     rcx
    jnz     .L\op\()_zen4_unroll_4x_loop

.L\op\()_zen4_before_rmdr_loop:

    setc    al
    test    r12, r12
    bt      ax,  0
    jz      .L\op\()_zen4_after_loop

.L\op\()_zen4_rmdr_loop:

    mov     r13, QWORD PTR [rsi + 8]
    shrx    r10, QWORD PTR [rdx], r9
    shlx    r11, QWORD PTR [rdx + 8], r8
    lea     r11, [r11 + r10]
    \carry_instr r13, r11
    mov     QWORD PTR [rdi + 8], r13

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     r12
    jnz     .L\op\()_zen4_rmdr_loop

.L\op\()_zen4_after_loop:

    shrx    rax, QWORD PTR [rdx], r9
    adc     rax, 0                        

.L\op\()_zen4_end_of_func:
    
    pop     r13
.cfi_adjust_cfa_offset -8
    pop     r12
.cfi_adjust_cfa_offset -8
    ret

.cfi_endproc
.size lshift_lt64_\op\()_zen4, . - lshift_lt64_\op\()_zen4
.endm

# ---------------------------------------------------------------------------
# GEN_LSHIFT_LT64_X64 op, first_instr, carry_instr
#   op           -> function name prefix                   (add / sub)
#   first_instr  -> first-limb add/sub instruction         (add / sub)
#   carry_instr  -> carry-propagation instruction          (adc / sbb)
# ---------------------------------------------------------------------------

.macro GEN_LSHIFT_LT64_X64 op, first_instr, carry_instr
lshift_lt64_\op\()_x64:
.cfi_startproc

    xchg    rcx, r8
    # now the size in r8
    # and bit_cnt in rcx

.L\op\()_x64_before_loop:

    mov     r9,  QWORD PTR [rsi]
    mov     r10, QWORD PTR [rdx]
    shl     r10, cl
    xor     rax, rax

    \first_instr r9, r10
    mov     QWORD PTR [rdi], r9

    setc    al

    dec     r8
    jz      .L\op\()_x64_after_loop

.L\op\()_x64_main_loop:

    mov     r9,  QWORD PTR [rsi + 8]
    mov     r10, QWORD PTR [rdx]
    mov     r11, QWORD PTR [rdx + 8]
    shld    r11, r10, cl

    add     al, -1
    \carry_instr r9, r11
    mov     QWORD PTR [rdi + 8], r9
    setc    al

    add     rsi, 8
    add     rdx, 8
    add     rdi, 8

    dec     r8
    jnz     .L\op\()_x64_main_loop

.L\op\()_x64_after_loop:

    mov     r10, QWORD PTR [rdx]
    neg     cl
    add     cl,  64
    shr     r10, cl
    bt      ax,  0
    mov     rax, r10
    adc     rax, 0

.L\op\()_x64_end_of_func:

    ret

.cfi_endproc
.size lshift_lt64_\op\()_x64, . - lshift_lt64_\op\()_x64
.endm

# ---------------------------------------------------------------------------
# Instantiations
# ---------------------------------------------------------------------------

GEN_LSHIFT_LT64_ZEN4 add, add, adc
GEN_LSHIFT_LT64_ZEN4 sub, sub, sbb

GEN_LSHIFT_LT64_X64  add, add, adc
GEN_LSHIFT_LT64_X64  sub, sub, sbb

.section .note.GNU-stack,"",@progbits
