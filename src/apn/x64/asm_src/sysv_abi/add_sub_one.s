
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   ADD SINGLE-LIMB TO APN-ARR FUNCTIONS                    |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

    #   Function Arguments
    #
    #   rdi -> result   (apn_dig_t*)
    #   rsi -> op1      (const apn_dig_t*)
    #   rdx -> size     (apn_size_t)
    #   rcx -> val      (apn_dig_t)

.intel_syntax noprefix
.text

.globl add_one_x64, add_one_zen4, sub_one_x64, sub_one_zen4

.type  add_one_x64,  @function
.type  add_one_zen4, @function
.type  sub_one_x64,  @function
.type  sub_one_zen4, @function

# ---------------------------------------------------------------------------
# GEN_ONE_ZEN4 op, first_instr, carry_instr
#   op           -> function name prefix       (add / sub)
#   first_instr  -> first-limb instruction     (add / sub)
#   carry_instr  -> carry-propagation instr    (adc / sbb)
# ---------------------------------------------------------------------------

.macro GEN_ONE_ZEN4 op, first_instr, carry_instr
\op\()_one_zen4:
.cfi_startproc

    mov     rax, QWORD PTR [rsi]
    \first_instr rax, rcx
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]

    setc    al
    dec     rdx

    mov     r11, rdx
    shr     rdx, 2
    and     r11, 3

    bt      ax,  0
    jz      .L\op\()_zen4_before_main_loop

.L\op\()_zen4_rmdr_loop:

    mov     rax, QWORD PTR [rsi]
    \carry_instr rax, 0
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]

    dec     r11
    jnz     .L\op\()_zen4_rmdr_loop

.L\op\()_zen4_before_main_loop:

    setc    r11b
    test    rdx, rdx
    bt      r11w, 0
    jz      .L\op\()_zen4_end_of_func

.p2align 6
.L\op\()_zen4_unroll4_loop:

.set i, 0
.rept 4

    mov     rax, QWORD PTR [rsi + i * 8]
    \carry_instr rax, 0
    mov     QWORD PTR [rdi + i * 8], rax

.set i, i + 1
.endr

    lea     rdi, [rdi + 32]
    lea     rsi, [rsi + 32]
    dec     rdx
    jnz     .L\op\()_zen4_unroll4_loop

.L\op\()_zen4_end_of_func:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size \op\()_one_zen4, . - \op\()_one_zen4
.endm

# ---------------------------------------------------------------------------
# GEN_ONE_X64 op, first_instr, carry_instr
#   op           -> function name prefix       (add / sub)
#   first_instr  -> first-limb instruction     (add / sub)
#   carry_instr  -> carry-propagation instr    (adc / sbb)
# ---------------------------------------------------------------------------

.macro GEN_ONE_X64 op, first_instr, carry_instr
\op\()_one_x64:
.cfi_startproc

    mov     rax, QWORD PTR [rsi]
    \first_instr rax, rcx
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]

    dec     rdx
    jz      .L\op\()_x64_end_of_func

.L\op\()_x64_main_loop:

    mov     rax, QWORD PTR [rsi]
    \carry_instr rax, 0
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]

    dec     rdx
    jnz     .L\op\()_x64_main_loop

.L\op\()_x64_end_of_func:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size \op\()_one_x64, . - \op\()_one_x64
.endm

# ---------------------------------------------------------------------------
# Instantiations
# ---------------------------------------------------------------------------

GEN_ONE_ZEN4 add, add, adc
GEN_ONE_ZEN4 sub, sub, sbb

GEN_ONE_X64  add, add, adc
GEN_ONE_X64  sub, sub, sbb

.section .note.GNU-stack,"",@progbits
