
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   BALANCED ADDITION FUNCS (N LIMBS)                       |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

    #   Function Arguments
    #
    #   rdi -> result   (apn_seg_t*)
    #   rsi -> op1      (const apn_seg_t*)
    #   rdx -> op2      (const apn_seg_t*)
    #   rcx -> size     (apn_size_t)

.intel_syntax noprefix
.text
.globl add_n_x64, add_n_zen4
.type  add_n_x64, @function
.type  add_n_zen4, @function

add_n_zen4:
.cfi_startproc

    xor     rax, rax
    mov     r11, rcx
    shr     rcx, 2
    and     r11, 3
    jz      .Lzen4_before_main_loop

.Lzen4_rmdr_loop:

    mov     rax, [rsi]
    adc     rax, [rdx]
    mov     [rdi], rax
    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]
    dec     r11
    jnz     .Lzen4_rmdr_loop

.Lzen4_before_main_loop:

    setc    al
    test    rcx, rcx
    bt      ax, 0
    jz      .Lzen4_end_of_func

.p2align 6
.Lzen4_unroll4_loop:

.set i, 0
.rept 4

    mov     rax, QWORD PTR [rsi + i * 8]
    adc     rax, QWORD PTR [rdx + i * 8]
    mov     QWORD PTR [rdi + i * 8], rax

.set i, i + 1
.endr

    lea     rsi, [rsi + 32]
    lea     rdx, [rdx + 32]
    lea     rdi, [rdi + 32]
    dec     rcx
    jnz     .Lzen4_unroll4_loop

.p2align 4
.Lzen4_end_of_func:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size add_n_zen4, .-add_n_zen4

add_n_x64:
.cfi_startproc

    xor     rax, rax
    test    rcx, rcx
    jz      .Lx64_end_of_func

.p2align 4
.Lx64_main_loop:

    mov     rax, QWORD PTR [rsi]
    adc     rax, QWORD PTR [rdx]
    mov     QWORD PTR [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     rcx
    jnz     .Lx64_main_loop

.p2align 4
.Lx64_end_of_func:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size add_n_x64, .-add_n_x64