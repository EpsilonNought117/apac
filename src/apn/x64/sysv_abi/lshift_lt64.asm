
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |						LEFT BITWISE SHIFT FUNCTIONS            			|
#   |                                                                           |
#   O---------------------------------------------------------------------------O

    #   Function Arguments
    #
    #   rdi -> result       (apn_seg_t*)
    #   rsi -> op1          (const apn_seg_t*)
    #   rdx -> size         (apn_size_t)
    #   rcx -> bit_cnt      (apn_seg_t)

    # The wrapper calling this will assert that (bit_cnt < 64 && bit_cnt > 0)
    # also asserts that size >= 1

.intel_syntax noprefix
.text
.globl lshift_lt64_x64, lshift_lt64_zen4
.type  lshift_lt64_x64, @function
.type  lshift_lt64_zen4, @function

lshift_lt64_zen4:
.cfi_startproc
    push    r12
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r12, 0
    push    r13
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r13, 0

    mov     r12, 64
    mov     r13, rcx    # lshift val in r13
    sub     r12, r13    # rshift val in r12 (64 - bit_cnt)

1:
    lea     rsi, [rsi + rdx * 8 - 8]
    lea     rdi, [rdi + rdx * 8 - 8]

    shrx    rax, QWORD PTR [rsi], r12

    dec     rdx
    jz      4f

    mov     r8,  rdx
    and     r8,  3      # size %= 4
    shr     rdx, 2      # size /= 4
    test    rdx, rdx
    jz      3f

.p2align 6
2:
.set i, 0
.rept 4
    shlx    r10, QWORD PTR [rsi - i * 8], r13
    shrx    r11, QWORD PTR [rsi - i * 8 - 8], r12
    or      r11, r10
    mov     QWORD PTR [rdi - i * 8], r11
.set i, i + 1
.endr
    sub     rsi, 32
    sub     rdi, 32
    dec     rdx
    jnz     2b

3:
    test    r8,  r8
    jz      4f

5:
    shlx    r10, QWORD PTR [rsi], r13
    shrx    r11, QWORD PTR [rsi - 8], r12
    or      r11, r10
    mov     QWORD PTR [rdi], r11

    sub     rsi, 8
    sub     rdi, 8
    dec     r8
    jnz     5b

4:
    shlx    r11, QWORD PTR [rsi], r13
    mov     QWORD PTR [rdi], r11

6:
    pop     r13
.cfi_adjust_cfa_offset -8
    pop     r12
.cfi_adjust_cfa_offset -8
    ret

.cfi_endproc
.size lshift_lt64_zen4, .-lshift_lt64_zen4

lshift_lt64_x64:
.cfi_startproc
    push    rbx
.cfi_adjust_cfa_offset 8
.cfi_rel_offset rbx, 0

    mov     rbx, rdi
    xor     rax, rax
    # bit_cnt already in rcx

1:
    lea     rsi, [rsi + rdx * 8 - 8]
    lea     rbx, [rbx + rdx * 8 - 8]

    mov     r11, QWORD PTR [rsi]
    shld    rax, r11, cl
    
    dec     rdx
    jz      3f
    # loop never entered if size is less than 2

2:
    mov     r10, QWORD PTR [rsi - 8]
    mov     r11, QWORD PTR [rsi]
    shld    r11, r10, cl
    mov     QWORD PTR [rbx], r11
    
    sub     rsi, 8
    sub     rbx, 8
    dec     rdx
    jnz     2b

3:
    mov     r11, QWORD PTR [rsi]
    shl     r11, cl
    mov     QWORD PTR [rbx], r11

4:
    pop     rbx
.cfi_adjust_cfa_offset -8
    ret
.cfi_endproc
.size lshift_lt64_x64, .-lshift_lt64_x64