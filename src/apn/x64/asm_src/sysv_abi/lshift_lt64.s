# O---------------------------------------------------------------------------O
# |                                                                           |
# |                         LEFT BITWISE SHIFT FUNCTIONS                      |
# |                                                                           |
# O---------------------------------------------------------------------------O
    
    # Function Arguments
    #
    # rdi -> result (apn_dig_t*)
    # rsi -> op1 (const apn_dig_t*)
    # rdx -> size (apn_size_t)
    # rcx -> bit_cnt (apn_dig_t)
    
    # The wrapper calling this will assert that (bit_cnt < 64 && bit_cnt > 0)
    # also asserts that size >= 1

.intel_syntax noprefix
.text
.globl lshift_lt64_x64, lshift_lt64_bmi2_x64
.type lshift_lt64_x64, @function
.type lshift_lt64_bmi2_x64, @function

lshift_lt64_bmi2_x64:
.cfi_startproc
    
    mov     r9,  64
    sub     r9,  rcx

.Lbmi2_x64_start_of_func:

    lea     rsi, [rsi + rdx * 8 - 8]
    lea     rdi, [rdi + rdx * 8 - 8]
    shrx    rax, QWORD PTR [rsi], r9

    dec     rdx
    jz      .Lbmi2_x64_end_of_func

    mov     r8, rdx
    and     r8,  3
    shr     rdx, 2
    test    rdx, rdx
    jz      .Lbmi2_x64_before_rmdr_loop

.p2align 4
.Lbmi2_x64_unroll4_loop:

.set i, 0
.rept 4

    shlx    r10, QWORD PTR [rsi - i * 8], rcx
    shrx    r11, QWORD PTR [rsi - i * 8 - 8], r9
    or      r11, r10
    mov     QWORD PTR [rdi - i * 8], r11

.set i, i + 1
.endr

    sub     rsi, 32
    sub     rdi, 32
    dec     rdx
    jnz     .Lbmi2_x64_unroll4_loop

.Lbmi2_x64_before_rmdr_loop:

    test    r8, r8
    jz      .Lbmi2_x64_end_of_func

.Lbmi2_x64_rmdr_loop:

    shlx    r10, QWORD PTR [rsi], rcx
    shrx    r11, QWORD PTR [rsi - 8], r9
    or      r11, r10
    mov     QWORD PTR [rdi], r11
    sub     rsi, 8
    sub     rdi, 8
    dec     r8
    jnz     .Lbmi2_x64_rmdr_loop

.Lbmi2_x64_end_of_func:

    shlx    r11, QWORD PTR [rsi], rcx
    mov     QWORD PTR [rdi], r11
    ret

.cfi_endproc
.size lshift_lt64_bmi2_x64, .-lshift_lt64_bmi2_x64

# ---------------------------------------------------

lshift_lt64_x64:
.cfi_startproc

    xor     rax, rax

.Lx64_start_of_func:

    lea     rsi, [rsi + rdx * 8 - 8]
    lea     rdi, [rdi + rdx * 8 - 8]
    mov     r11, QWORD PTR [rsi]
    shld    rax, r11, cl
    dec     rdx
    jz      .Lx64_end_of_func

.Lx64_main_loop:

    mov     r10, QWORD PTR [rsi - 8]
    mov     r11, QWORD PTR [rsi]
    shld    r11, r10, cl
    mov     QWORD PTR [rdi], r11
    sub     rsi, 8
    sub     rdi, 8
    dec     rdx
    jnz     .Lx64_main_loop

.Lx64_end_of_func:

    mov     r11, QWORD PTR [rsi]
    shl     r11, cl
    mov     QWORD PTR [rdi], r11
    ret

.cfi_endproc
.size lshift_lt64_x64, .-lshift_lt64_x64

.section .note.GNU-stack,"",@progbits

