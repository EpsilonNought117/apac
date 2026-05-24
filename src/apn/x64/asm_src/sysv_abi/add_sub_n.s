
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |              BALANCED ADDITION/SUBTRACTION FUNCS (N LIMBS)                |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

    #   Function Arguments (System V ABI)
    #
    #   rdi -> result   (ap_dig_t*)
    #   rsi -> op1      (const ap_dig_t*)
    #   rdx -> op2      (const ap_dig_t*)
    #   rcx -> size     (ap_size_t)

.intel_syntax noprefix
.text

.globl add_n_x64, add_n_zen4, sub_n_x64, sub_n_zen4

.type  add_n_x64,  @function
.type  add_n_zen4, @function
.type  sub_n_x64,  @function
.type  sub_n_zen4, @function

# ---------------------------------------------------------------------------
# GEN_ZEN4 op, instr
#   op    -> function name prefix  (add / sub)
#   instr -> carry instruction     (adc / sbb)
# ---------------------------------------------------------------------------

.macro GEN_ZEN4 op, instr
\op\()_n_zen4:
.cfi_startproc

    xor     rax, rax
    mov     r11, rcx
    shr     rcx, 2
    and     r11, 3
    jz      .L\op\()_zen4_before_main_loop

.L\op\()_zen4_rmdr_loop:

    mov     rax, [rsi]
    \instr  rax, [rdx]
    mov     [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     r11
    jnz     .L\op\()_zen4_rmdr_loop

.L\op\()_zen4_before_main_loop:

    setc    al
    test    rcx, rcx
    bt      ax, 0
    jz      .L\op\()_zen4_end_of_func

.p2align 6
.L\op\()_zen4_unroll4_loop:

.set i, 0
.rept 4

    mov     rax, QWORD PTR [rsi + i * 8]
    \instr  rax, QWORD PTR [rdx + i * 8]
    mov     QWORD PTR [rdi + i * 8], rax

.set i, i + 1
.endr

    lea     rsi, [rsi + 32]
    lea     rdx, [rdx + 32]
    lea     rdi, [rdi + 32]

    dec     rcx
    jnz     .L\op\()_zen4_unroll4_loop

.p2align 4
.L\op\()_zen4_end_of_func:
    
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size \op\()_n_zen4, . - \op\()_n_zen4
.endm

# ---------------------------------------------------------------------------
# GEN_X64 op, instr
#   op    -> function name prefix  (add / sub)
#   instr -> carry instruction     (adc / sbb)
# ---------------------------------------------------------------------------

.macro GEN_X64 op, instr

\op\()_n_x64:
.cfi_startproc

    xor     rax, rax
    test    rcx, rcx
    jz      .L\op\()_x64_end_of_func

.p2align 4
.L\op\()_x64_main_loop:

    mov     rax, QWORD PTR [rsi]
    \instr  rax, QWORD PTR [rdx]
    mov     QWORD PTR [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]
    
    dec     rcx
    jnz     .L\op\()_x64_main_loop

.p2align 4
.L\op\()_x64_end_of_func:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size \op\()_n_x64, . - \op\()_n_x64
.endm

# ---------------------------------------------------------------------------
# Instantiations
# ---------------------------------------------------------------------------

GEN_ZEN4 add, adc
GEN_ZEN4 sub, sbb

GEN_X64  add, adc
GEN_X64  sub, sbb

.section .note.GNU-stack,"",@progbits
