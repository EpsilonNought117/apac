
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |		         APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS   			|
#   |                                                                           |
#   O---------------------------------------------------------------------------O

	#   Function Arguments
    #
    #   rdi -> result       (apn_seg_t*)
    #   rsi -> op1          (const apn_seg_t*)
    #   rdx -> size         (apn_size_t)
    #   rcx -> val          (apn_seg_t)

.intel_syntax noprefix
.text
.globl addmul_one_x64, addmul_one_zen4
.type  addmul_one_x64, @function
.type  addmul_one_zen4, @function

#   -------------------------
#
#        MULX/ADOX/ADCX
#
#   -------------------------

addmul_one_zen4:
.cfi_startproc

    jmp     1f

.p2align 4
0:
    .quad 9f
    .quad 10f
    .quad 11f
    .quad 12f
    .quad 13f
    .quad 14f
    .quad 15f
    .quad 16f

1:
    xchg    rcx, rdx
    
    mov     r8,  rcx
    and     r8,  7
    shr     rcx, 3
    
    lea     r9,  [rip + 0b]
    lea     r9,  [r9 + r8 * 8]

    mov     rax, QWORD PTR [rdi]
    test    rcx, rcx
    jz      3f

.p2align 6
2:
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

    jrcxz   3f
    jmp     2b

.p2align 4
3:
    jmp     QWORD PTR [r9]

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

    jmp     9f

.endm

REM_CASE 7
REM_CASE 6
REM_CASE 5
REM_CASE 4
REM_CASE 3
REM_CASE 2
REM_CASE 1

.p2align 5
9:
    adcx    rax, rcx
    mov     QWORD PTR [rdi + r8 * 8], rax
    
    seto    al
    movzx   rax, al
    ret

.cfi_endproc
.size addmul_one_zen4, .-addmul_one_zen4

#   -------------------------
#
#            MUL/ADC
#
#   -------------------------

addmul_one_x64:
.cfi_startproc
    
    xchg    rcx, rdx
    
    # rbx is temp_reg
    # val   -> rdx
    # size  -> rcx
    
    mov     r9,  rdx        # copy of val in r9
    xor     rdx, rdx
    xor     r10, r10
    test    rcx, rcx
    jz      3f

1:
    mul     QWORD PTR [rsi] # rdx:rax = rax * op1[idx]
    
    add     r10, rax
    adc     rdx, 0
    add     QWORD PTR [rdi], r10
    
    mov     r10, rdx
    mov     rax, r9         # restore clobbered rax
    adc     r10, 0          # temp_reg += (CF + high64)
    
    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     rcx
    jnz     1b

2:
    adc     QWORD PTR [rdi], rdx

3:
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size addmul_one_x64, .-addmul_one_x64