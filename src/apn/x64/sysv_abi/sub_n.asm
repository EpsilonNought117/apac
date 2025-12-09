#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   BALANCED SUBTRACTION FUNCS (N LIMBS)                    |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

    #   Function Arguments
    #
    #   rdi -> result (apn_seg_t*)   
    #   rsi -> op1    (const apn_seg_t*)
    #   rdx -> op2    (const apn_seg_t*)
    #   rcx -> size   (apn_size_t)

.intel_syntax noprefix

.text
.globl sub_n_x64, sub_n_zen4
.type  sub_n_x64, @function
.type  sub_n_zen4, @function

sub_n_zen4:
    .cfi_startproc

    xor     rax, rax
    mov     r11, rcx            # r11 = size
    shr     rcx, 2              # rcx = size / 4
    and     r11, 3              # r11 = size % 4
    jz      L_sub_after_small

L_sub_small_loop:
    mov     rax, [rsi]
    sbb     rax, [rdx]
    mov     [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     r11
    jnz     L_sub_small_loop

L_sub_after_small:
    setc    al
    test    rcx, rcx
    bt      ax, 0
    jz      L_sub_return

.macro SUB4 base
    mov     rax, [rsi + \base]
    sbb     rax, [rdx + \base]
    mov     [rdi + \base], rax
.endm

L_sub_unrolled:
    SUB4 0
    SUB4 8
    SUB4 16
    SUB4 24

    lea     rsi, [rsi + 32]
    lea     rdx, [rdx + 32]
    lea     rdi, [rdi + 32]

    dec     rcx
    jnz     L_sub_unrolled

L_sub_return:
    setc    al
    movzx   rax, al
    ret

    .cfi_endproc
.size sub_n_zen4, .-sub_n_zen4

sub_n_x64:
    .cfi_startproc

    xor     rax, rax

L_sub_loop_x64:
    mov     rax, [rsi]
    sbb     rax, [rdx]
    mov     [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     rcx
    jnz     L_sub_loop_x64

    .p2align 4

L_sub_done_x64:
    setc    al
    movzx   rax, al
    ret

    .cfi_endproc
.size sub_n_x64, .-sub_n_x64
