#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   ADD SINGLE-LIMB TO APN-ARR FUNCTIONS                    |
#   |                                                                           |
#   O---------------------------------------------------------------------------O


    #   Function Arguments
    #
    #   rdi -> result   (apn_seg_t*)
    #   rsi -> op1      (const apn_seg_t*)
    #   rdx -> size     (apn_size_t)
    #   rcx -> val      (apn_seg_t)

.intel_syntax noprefix

.text
.globl add_one_x64, add_one_zen4
.type  add_one_x64, @function
.type  add_one_zen4, @function

.macro ADD_CY_ZERO base
    mov     rax, QWORD PTR [rsi + \base]
    adc     rax, 0
    mov     QWORD PTR [rdi + \base], rax
.endm

###############################################################################
# add_one_zen4
###############################################################################

add_one_zen4:
.cfi_startproc

    mov     rax, QWORD PTR [rsi]
    adc     rax, rcx
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    setc    al
    dec     rdx

    mov     r11, rdx
    shr     rdx, 2
    and     r11, 3
    bt      ax,  0          ; doesn't modify zero flag
    jz      .Ladd_one_zen4_before_unroll

.Ladd_one_zen4_small_loop:

    ADD_CY_ZERO 0

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     r11
    jnz     .Ladd_one_zen4_small_loop

.Ladd_one_zen4_before_unroll:

    setc    r11
    test    rdx, rdx
    bt      ax,  0
    jz      .Ladd_one_zen4_end_of_func

.Ladd_one_zen4_unrolled_loop:

    ADD_CY_ZERO 0
    ADD_CY_ZERO 8
    ADD_CY_ZERO 16
    ADD_CY_ZERO 24

    lea     rdi, [rdi + 32]
    lea     rsi, [rsi + 32]
    dec     rdx
    jnz     .Ladd_one_zen4_unrolled_loop

.Ladd_one_zen4_end_of_func:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size add_one_zen4, .-add_one_zen4


