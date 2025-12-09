
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   BALANCED ADDITION FUNCS (N LIMBS)                       |
#   |                                                                           |
#   O---------------------------------------------------------------------------O


    #   Function Arguments
    #
    #   rdi -> result       (apn_seg_t*)   
    #   rsi -> op1          (const apn_seg_t*)
    #   rdx -> op2          (const apn_seg_t*)
    #   rcx -> size         (apn_size_t)

.intel_syntax noprefix

.text
.globl add_n_x64, add_n_zen4
.type  add_n_x64, @function
.type  add_n_zen4, @function

add_n_zen4:
.cfi_startproc

    xor     rax, rax
    mov     r11, rcx        # r11 = size
    shr     rcx, 2          # rcx = size / 4
    and     r11, 3          # r11 = size % 4
    jz      L_after_small

L_small_loop:
    mov     rax, [rsi]
    adc     rax, [rdx]
    mov     [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     r11
    jnz     L_small_loop

L_after_small:
    
    setc    al
    test    rcx, rcx
    bt      ax, 0
    jz      L_ret_zen4

L_unrolled:
    
.macro UNROLL4 base
    mov     rax, [rsi + \base]
    adc     rax, [rdx + \base]
    mov     [rdi + \base], rax
.endm

    UNROLL4 0
    UNROLL4 8
    UNROLL4 16
    UNROLL4 24

    lea     rsi, [rsi + 32]
    lea     rdx, [rdx + 32]
    lea     rdi, [rdi + 32]

    dec     rcx
    jnz     L_unrolled


L_ret_zen4:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size add_n_zen4, .-add_n_zen4


add_n_x64:
    .cfi_startproc

    xor     rax, rax

L_loop_x64:
    mov     rax, [rsi]
    adc     rax, [rdx]
    mov     [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     rcx
    jnz     L_loop_x64

    .p2align 4

L_done_x64:
    setc    al
    movzx   rax, al
    ret

    .cfi_endproc
.size add_n_x64, .-add_n_x64
