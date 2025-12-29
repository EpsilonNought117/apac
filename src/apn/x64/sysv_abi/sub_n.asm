#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   BALANCED SUBTRACTION FUNCS (N LIMBS)                    |
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
.globl sub_n_x64, sub_n_zen4
.type  sub_n_x64, @function
.type  sub_n_zen4, @function

.macro SUB_BRW base
    mov     rax, QWORD PTR [rsi + \base]
    sbb     rax, QWORD PTR [rdx + \base]
    mov     QWORD PTR [rdi + \base], rax
.endm

sub_n_zen4:
.cfi_startproc

    xor     rax, rax
    mov     r11, rcx            # r11 = size
    shr     rcx, 2              # rcx = size / 4
    and     r11, 3              # r11 = size % 4
    jz      2f

1:
    mov     rax, [rsi]
    sbb     rax, [rdx]
    mov     [rdi], rax
    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]
    dec     r11
    jnz     1b

2:
    setc    al
    test    rcx, rcx
    bt      ax, 0
    jz      4f

.p2align 4
3:
    SUB_BRW 0
    SUB_BRW 8
    SUB_BRW 16
    SUB_BRW 24
    lea     rsi, [rsi + 32]
    lea     rdx, [rdx + 32]
    lea     rdi, [rdi + 32]
    dec     rcx
    jnz     3b

.p2align 4
4:
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size sub_n_zen4, .-sub_n_zen4

###############################################################################
# sub_n_x64
###############################################################################

sub_n_x64:
.cfi_startproc
    
    xor     rax, rax
    test    rcx, rcx
    jz      2f

.p2align 4
1:
    SUB_BRW 0
    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]
    dec     rcx
    jnz     1b

.p2align 4
2:
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size sub_n_x64, .-sub_n_x64