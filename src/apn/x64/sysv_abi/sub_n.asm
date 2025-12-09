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
    jz      2f

1:  # small remainder loop
    mov     rax, [rsi]
    sbb     rax, [rdx]
    mov     [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     r11
    jnz     1b

2:  # after small loop
    setc    al
    test    rcx, rcx
    bt      ax, 0
    jz      4f

3:  # unrolled loop
    .macro SUB4 base
        mov     rax, [rsi + \base]
        sbb     rax, [rdx + \base]
        mov     [rdi + \base], rax
    .endm

    SUB4 0
    SUB4 8
    SUB4 16
    SUB4 24

    lea     rsi, [rsi + 32]
    lea     rdx, [rdx + 32]
    lea     rdi, [rdi + 32]

    dec     rcx
    jnz     3b

4:  # return
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size sub_n_zen4, .-sub_n_zen4

sub_n_x64:
.cfi_startproc

    xor     rax, rax

1:  # simple loop
    mov     rax, [rsi]
    sbb     rax, [rdx]
    mov     [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]

    dec     rcx
    jnz     1b

    .p2align 4

2:  # return
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size sub_n_x64, .-sub_n_x64
