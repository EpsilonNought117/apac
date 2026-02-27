
# O---------------------------------------------------------------------------O
# |                                                                           |
# |                     RIGHT BITWISE SHIFT FUNCTIONS                         |
# |                                                                           |
# O---------------------------------------------------------------------------O

    # Function Arguments
    #
    # rdi -> result (ap_seg_t*)
    # rsi -> op1 (const ap_seg_t*)
    # rdx -> size (ap_size_t)
    # rcx -> bit_cnt (ap_seg_t)
    
    # The wrapper calling this will assert that (bit_cnt < 64 && bit_cnt > 0)
    # also asserts that size >= 1

.intel_syntax noprefix
.text
.globl rshift_lt64_x64, rshift_lt64_zen4
.type rshift_lt64_x64, @function
.type rshift_lt64_zen4, @function

rshift_lt64_zen4:
.cfi_startproc

    mov     r9,  64
    sub     r9,  rcx # lshift val in r9 (64 - bit_cnt)

.Lzen4_start_of_func:

    shlx    rax, QWORD PTR [rsi], r9
    
    dec     rdx
    jz      .Lzen4_end_of_func
    
    mov     r8,  rdx
    and     r8,  3  # size %= 4
    shr     rdx, 2  # size /= 4
    test    rdx, rdx
    jz      .Lzen4_before_rmdr_loop

.p2align 6
.Lzen4_unroll_loop:

.set i, 0
.rept 4

    shrx    r10, QWORD PTR [rsi + i * 8], rcx
    shlx    r11, QWORD PTR [rsi + i * 8 + 8], r9
    or      r11, r10
    mov     QWORD PTR [rdi + i * 8], r11

.set i, i + 1
.endr

    add     rsi, 32
    add     rdi, 32
    dec     rdx
    jnz     .Lzen4_unroll_loop

.Lzen4_before_rmdr_loop:

    test    r8,  r8
    jz      .Lzen4_end_of_func

.Lzen4_rmdr_loop:

    shrx    r10, QWORD PTR [rsi], rcx
    shlx    r11, QWORD PTR [rsi + 8], r9
    or      r11, r10
    mov     QWORD PTR [rdi], r11
    
    add     rsi, 8
    add     rdi, 8
    dec     r8
    jnz     .Lzen4_rmdr_loop

.Lzen4_end_of_func:

    shrx    r11, QWORD PTR [rsi], rcx
    mov     QWORD PTR [rdi], r11
    ret

.cfi_endproc
.size rshift_lt64_zen4, .-rshift_lt64_zen4

rshift_lt64_x64:
.cfi_startproc

    xor     rax, rax
    # bit_cnt already in rcx

.Lx64_start_of_func:

    mov     r11, QWORD PTR [rsi]
    shrd    rax, r11, cl
    
    dec     rdx
    jz      .Lx64_end_of_func

.Lx64_main_loop:

    mov     r10, QWORD PTR [rsi + 8]
    mov     r11, QWORD PTR [rsi]
    shrd    r11, r10, cl
    mov     QWORD PTR [rdi], r11
    
    add     rsi, 8
    add     rdi, 8
    dec     rdx
    jnz     .Lx64_main_loop

.Lx64_end_of_func:

    mov     r11, QWORD PTR [rsi]
    shr     r11, cl
    mov     QWORD PTR [rdi], r11
    ret

.cfi_endproc
.size rshift_lt64_x64, .-rshift_lt64_x64

.section .note.GNU-stack,"",@progbits

