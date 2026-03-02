#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   SUB SINGLE-LIMB FROM APN-ARR FUNCTIONS                  |
#   |                                                                           |
#   O---------------------------------------------------------------------------O
    
    #   Function Arguments
    #
    #   rdi -> result   (ap_dig_t*)
    #   rsi -> op1      (const ap_dig_t*)
    #   rdx -> size     (ap_size_t)
    #   rcx -> val      (ap_dig_t)

.intel_syntax noprefix
.text
.globl sub_one_x64, sub_one_zen4
.type  sub_one_x64, @function
.type  sub_one_zen4, @function

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
    bt      ax,  0          # doesn't modify zero flag
    jz      .Lzen4_before_unroll4

.Lzen4_rmdr_loop:

    mov     rax, QWORD PTR [rsi]
    sbb     rax, 0
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     r11
    jnz     .Lzen4_rmdr_loop

.Lzen4_before_unroll4:

    setc    r11b
    test    rdx, rdx
    bt      r11w, 0
    jz      .Lzen4_end_of_func

.Lzen4_unroll4_loop:

.set i, 0
.rept 4

    mov     rax, QWORD PTR [rsi + i * 8]
    sbb     rax, 0
    mov     QWORD PTR [rdi + i * 8], rax

.set i, i + 1
.endr
    
    lea     rdi, [rdi + 32]
    lea     rsi, [rsi + 32]
    dec     rdx
    jnz     .Lzen4_unroll4_loop

.Lzen4_end_of_func:

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
    jz      .Lx64_end_of_func

.Lx64_loop:

    mov     rax, QWORD PTR [rsi]
    sbb     rax, 0
    mov     QWORD PTR [rdi], rax
    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     rdx
    jnz     .Lx64_loop

.Lx64_end_of_func:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size sub_one_x64, .-sub_one_x64

.section .note.GNU-stack,"",@progbits

