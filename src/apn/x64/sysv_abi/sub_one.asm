
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   SUB SINGLE-LIMB FROM APN-ARR FUNCTIONS                  |
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
.globl sub_one_x64, sub_one_zen4
.type  sub_one_x64, @function
.type  sub_one_zen4, @function

.macro SUB_BRW_ZERO base
    mov     rax, QWORD PTR [rsi + \base]
    sbb     rax, 0
    mov     QWORD PTR [rdi + \base], rax
.endm

sub_one_zen4:
.cfi_startproc

    mov     rax, QWORD PTR [rsi]
    sub     rax, rcx
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    setc    al
    dec     rdx
    
    mov     r11, rdx
    shr     rdx, 2
    and     r11, 3
    bt      ax,  0          ; doesn't modify zero flag
    jz      2f

1:
    SUB_BRW_ZERO 0

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     r11
    jnz     1b

2:
    setc    r11
    test    rdx, rdx
    bt      ax,  0
    jz      4f

3:
    SUB_BRW_ZERO 0
    SUB_BRW_ZERO 8
    SUB_BRW_ZERO 16
    SUB_BRW_ZERO 24

    lea     rdi, [rdi + 32]
    lea     rsi, [rsi + 32]
    dec     rdx
    jnz     3b

4:
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size sub_one_zen4, .-sub_one_zen4

sub_one_x64:
.cfi_startproc

    mov     rax, QWORD PTR [rsi]
    sub     rax, rcx
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     rdx
    jz      2f

1:
    SUB_BRW_ZERO 0
    
    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     rdx
    jnz     1b

2:
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size sub_one_x64, .-sub_one_x64