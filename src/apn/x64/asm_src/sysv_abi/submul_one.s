
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
.globl submul_one_x64, submul_one_zen4
.type  submul_one_x64, @function
.type  submul_one_zen4, @function

#   -------------------------
#
#        MULX/ADOX/ADCX
#
#   -------------------------

submul_one_zen4:
.cfi_startproc

    xchg    rcx, rdx
    mov     r8,  rcx
    and     r8,  3
    shr     rcx, 2
    xor     r9,  r9
    test    rcx, rcx
    stc
    jz      .Lzen4_before_rmdr

.Lzen4_unroll4_loop:

.set i, 0
.rept 4

    mulx    r11, r10, QWORD PTR [rsi + i * 8]
    adox    r10, r9
    not     r10
    adcx    r10, QWORD PTR [rdi + i * 8]
    mov     QWORD PTR [rdi + i * 8], r10
    mov     r9,  r11

.set i, i + 1
.endr

    lea     rdi, [rdi + 32]
    lea     rsi, [rsi + 32]
    lea     rcx, [rcx - 1]

    jrcxz   .Lzen4_before_rmdr
    jmp     .Lzen4_unroll4_loop

.p2align 4
.Lzen4_before_rmdr:
    
    mov     rcx, r8
    jrcxz   .Lzen4_final_carry

.Lzen4_rmdr_loop:
    
    mulx    r11, r10, QWORD PTR [rsi]
    adox    r10, r9
    not     r10
    adcx    r10, QWORD PTR [rdi]
    mov     QWORD PTR [rdi], r10
    mov     r9,  r11
    
    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    loop    .Lzen4_rmdr_loop

.Lzen4_final_carry:

    adox    r9,  rcx
    not     r9
    adcx    r9,  QWORD PTR [rdi]
    mov     QWORD PTR [rdi], r9

.Lzen4_end_of_func:

    setnc   al
    movzx   rax, al
    ret

.cfi_endproc
.size submul_one_zen4, .-submul_one_zen4

#   -------------------------
#
#            MUL/ADC
#
#   -------------------------

submul_one_x64:
.cfi_startproc

    xchg    rcx, rdx
    mov     r9,  rdx
    xor     r10, r10
    mov     rax, r9

.Lx64_main_loop:

    mul     QWORD PTR [rsi] # rdx:rax = rax * op1[idx]
    
    # mul clobbers the original Carry Flag value
    
    add     r10, rax                # temp_reg += low64
    adc     rdx, 0                  # high64 += CF
    sub     QWORD PTR [rdi], r10    # result[i] -= temp_reg
    mov     r10, rdx                # temp_reg = high64
    mov     rax, r9                 # restore clobbered rax
    adc     r10, 0                  # temp_reg += borrow (from last seg)
    
    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 8]
    dec     rcx
    jnz     .Lx64_main_loop

.Lx64_loop_end:

    sbb     QWORD PTR [rdi], r10

.Lx64_final_borrow:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size submul_one_x64, .-submul_one_x64

.section .note.GNU-stack,"",@progbits

