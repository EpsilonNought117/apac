
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                      BASECASE MULTIPLICATION FUNCTIONS                    |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

    #   Function Arguments
    #
    #   rdi -> result           (apn_seg_t*)
    #   rsi -> op1              (const apn_seg_t*)
    #   rdx -> op2              (const apn_seg_t*)
    #   rcx -> size1            (apn_size_t)
    #   r8  -> size2            (apn_size_t)

    #   ASSUMPTION
    #   (size1 >= size2)

.intel_syntax noprefix
.text
.globl mul_bc_x64, mul_bc_zen4
.type  mul_bc_x64, @function
.type  mul_bc_zen4, @function

#   -------------------------
#
#        MULX/ADOX/ADCX
#
#   -------------------------

# The fastest procedures utilizing ADX and BMI2 x64 ISA extensions
# 8x unroll adx/bmi2 variant with jump table

mul_bc_zen4:
.cfi_startproc
    push    rbx
.cfi_adjust_cfa_offset 8
.cfi_rel_offset rbx, 0
    push    r12
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r12, 0
    push    r13
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r13, 0
    push    r14
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r14, 0

    jmp     2f

.p2align 4
1:
    .quad 10f
    .quad 11f
    .quad 12f
    .quad 13f
    .quad 14f
    .quad 15f
    .quad 16f
    .quad 17f

2:
    mov     rbx, rdx        # op2 in rbx
    mov     r9,  rcx        # size1 copy in r9
    mov     r14, rcx        # another copy for remainder
    shr     r9,  3
    and     r14, 7
    mov     r13, r9
    shl     r13, 6          # floor(size1 / 8) * 64
    lea     r12, [rip + 1b]
    lea     r12, [r12 + r14 * 8]

3:
    mov     rcx, r9
    mov     rax, QWORD PTR [rdi]
    mov     rdx, QWORD PTR [rbx]
    test    rcx, rcx
    jz      5f

.p2align 6
4:
.set i, 0
.rept 8

    mulx    r11, r10, QWORD PTR [rsi + i * 8]
    adcx    r10, rax
    adox    r11, QWORD PTR [rdi + i * 8 + 8]
    mov     QWORD PTR [rdi + i * 8], r10
    mov     rax, r11

.set i, i + 1
.endr

    lea     rsi, [rsi + 64]
    lea     rdi, [rdi + 64]
    lea     rcx, [rcx - 1]
    jrcxz   5f
    jmp     4b

.p2align 5
5:
    jmp     QWORD PTR [r12]

# Generate remainder cases using a macro
.macro REM_CASE outer

.p2align 4
1\outer\():

.set i, 0
.rept \outer

    mulx    r11, r10, QWORD PTR [rsi + i * 8]
    adcx    r10, rax
    adox    r11, QWORD PTR [rdi + i * 8 + 8]
    mov     QWORD PTR [rdi + i * 8], r10
    mov     rax, r11

.set i, i + 1
.endr

    jmp     10f

.endm

# Generate all remainder cases from 7 down to 1
REM_CASE 7
REM_CASE 6
REM_CASE 5
REM_CASE 4
REM_CASE 3
REM_CASE 2
REM_CASE 1

10:
    adc     rax, 0
    mov     QWORD PTR [rdi + r14 * 8], rax

    add     rbx, 8
    sub     rsi, r13
    sub     rdi, r13
    add     rdi, 8

    dec     r8
    jnz     3b

20:
    pop     r14
.cfi_adjust_cfa_offset -8
    pop     r13
.cfi_adjust_cfa_offset -8
    pop     r12
.cfi_adjust_cfa_offset -8
    pop     rbx
.cfi_adjust_cfa_offset -8
    ret

.cfi_endproc
.size mul_bc_zen4, .-mul_bc_zen4

#   -------------------------
#
#            MUL/ADC
#
#   -------------------------

# Lowest common denominator x64 multiplication routine
# Not particularly optimized

mul_bc_x64:
.cfi_startproc
    push    rbx
.cfi_adjust_cfa_offset 8
.cfi_rel_offset rbx, 0

1:
    mov     rbx, rdx        # op2 in rbx

2:
    mov     r11, rcx        # temp size1
    xor     r10, r10        # temp_reg
    mov     r9,  QWORD PTR [rbx]    # op2[i]
    mov     rax, r9

.p2align 4
3:
    mul     QWORD PTR [rsi]         # rdx : rax = rax * op1[i]

    # now product in rdx:rax
    # rax = low64
    # rdx = high64

    add     r10, rax                # temp_reg += low64
    adc     rdx, 0                  # high64 += CF
    add     QWORD PTR [rdi], r10    # result[i + j] += temp_reg

    mov     rax, r9                 # restore rax for next mul
    mov     r10, rdx
    adc     r10, 0

    lea     rsi, [rsi + 8]          # update ptrs
    lea     rdi, [rdi + 8]
    dec     r11
    jnz     3b

4:
    adc     QWORD PTR [rdi], rdx
    mov     r11, rcx
    shl     r11, 3
    
    sub     rdi, r11
    sub     rsi, r11
    add     rdi, 8
    add     rbx, 8

    dec     r8
    jnz     2b

5:
    pop     rbx
.cfi_adjust_cfa_offset -8
    ret

.cfi_endproc
.size mul_bc_x64, .-mul_bc_x64