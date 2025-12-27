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

.macro ADD_CY base
    mov     rax, [rsi + \base]
    adc     rax, [rdx + \base]
    mov     [rdi + \base], rax
.endm

###############################################################################
# add_n_zen4
###############################################################################

add_n_zen4:
    .cfi_startproc

    xor     rax, rax
    mov     r11, rcx        # r11 = size
    shr     rcx, 2          # rcx = size / 4
    and     r11, 3          # r11 = size % 4
    jz      .Ladd_n_zen4_after_small

.Ladd_n_zen4_small_loop:
    mov     rax, [rsi]
    adc     rax, [rdx]
    mov     [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     r11
    jnz     .Ladd_n_zen4_small_loop

.Ladd_n_zen4_after_small:
    setc    al
    test    rcx, rcx
    bt      ax, 0
    jz      .Ladd_n_zen4_return

.p2align 4
.Ladd_n_zen4_unrolled_loop:

    ADD_CY 0
    ADD_CY 8
    ADD_CY 16
    ADD_CY 24

    lea     rsi, [rsi + 32]
    lea     rdx, [rdx + 32]
    lea     rdi, [rdi + 32]

    dec     rcx
    jnz     .Ladd_n_zen4_unrolled_loop

.p2align 4
.Ladd_n_zen4_return:
    setc    al
    movzx   rax, al
    ret

    .cfi_endproc
.size add_n_zen4, .-add_n_zen4

###############################################################################
# add_n_x64
###############################################################################

add_n_x64:
    .cfi_startproc

    xor     rax, rax

.p2align 4
.Ladd_n_x64_loop:

    ADD_CY 0

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     rcx
    jnz     .Ladd_n_x64_loop

.p2align 4
.Ladd_n_x64_return:
    setc    al
    movzx   rax, al
    ret

    .cfi_endproc
.size add_n_x64, .-add_n_x64
