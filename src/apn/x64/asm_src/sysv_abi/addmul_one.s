
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |		         APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS   			|
#   |                                                                           |
#   O---------------------------------------------------------------------------O

	#   Function Arguments
    #
    #   rdi -> result       (apn_dig_t*)
    #   rsi -> op1          (const apn_dig_t*)
    #   rdx -> size         (apn_size_t)
    #   rcx -> val          (apn_dig_t)

.intel_syntax noprefix
.text
.globl addmul_one_x64, addmul_one_adx_bmi2_x64
.type  addmul_one_x64, @function
.type  addmul_one_adx_bmi2_x64, @function

#   -------------------------
#
#        MULX/ADOX/ADCX
#
#   -------------------------

addmul_one_adx_bmi2_x64:
.cfi_startproc

.Ladx_bmi2_x64_start_of_func:
    
    xchg    rcx, rdx    # convenient, innit?
    mov     r8,  rcx
    and     r8,  7
    shr     rcx, 3
    
    mov     rax, QWORD PTR [rdi]
    test    rcx, rcx
    jz      .Ladx_bmi2_x64_before_remainder

.p2align 4
.Ladx_bmi2_x64_unroll8_loop:

.set i, 0
.rept 8

    # Even iteration
    .if (i & 1) == 0
    
        mulx    r11, r10, QWORD PTR [rsi + i * 8]
        adcx    r10, rax
        adox    r11, QWORD PTR [rdi + i * 8 + 8]

    # Odd iteration
    .else

        mulx    rax, r10, QWORD PTR [rsi + i * 8]
        adcx    r10, r11
        adox    rax, QWORD PTR [rdi + i * 8 + 8]
    
    .endif

    mov     QWORD PTR [rdi + i * 8], r10

.set i, i + 1
.endr

    lea     rsi, [rsi + 64]
    lea     rdi, [rdi + 64]
    lea     rcx, [rcx - 1]
    jrcxz   .Ladx_bmi2_x64_before_remainder
    jmp     .Ladx_bmi2_x64_unroll8_loop

.p2align 4
.Ladx_bmi2_x64_before_remainder:

    mov     rcx, r8
    jrcxz   .Ladx_bmi2_x64_end_of_loop

.p2align 4
.Ladx_bmi2_x64_rmdr_loop:

    mulx    r11, r10, QWORD PTR [rsi]
    adcx    r10, rax
    adox    r11, QWORD PTR [rdi + 8]
    mov     QWORD PTR [rdi], r10
    mov     rax, r11

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 8]
    loop    .Ladx_bmi2_x64_rmdr_loop

.p2align 5
.Ladx_bmi2_x64_end_of_loop:

    adcx    rax, rcx
    mov     QWORD PTR [rdi], rax

.Ladx_bmi2_x64_end_of_func:

    seto    al
    movzx   rax, al
    ret

.cfi_endproc
.size addmul_one_adx_bmi2_x64, .-addmul_one_adx_bmi2_x64

#   -------------------------
#
#            MUL/ADC
#
#   -------------------------

addmul_one_x64:
.cfi_startproc

    xchg    rcx, rdx
    mov     r9,  rdx
    xor     rdx, rdx
    xor     r10, r10
    mov     rax, r9

.Lx64_main_loop:

    mul     QWORD PTR [rsi]
    add     r10, rax
    adc     rdx, 0
    add     QWORD PTR [rdi], r10
    mov     r10, rdx
    mov     rax, r9
    adc     r10, 0
    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     rcx
    jnz     .Lx64_main_loop

.Lx64_end_of_loop:

    adc     QWORD PTR [rdi], r10

.Lx64_end_of_func:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size addmul_one_x64, .-addmul_one_x64

.section .note.GNU-stack,"",@progbits

