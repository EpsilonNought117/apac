
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
.globl submul_one_x64, submul_one_zen4
.type  submul_one_x64, @function
.type  submul_one_zen4, @function

.macro ADX_MULX_ITER offset
    mulx    r11, r10, QWORD PTR [rsi + \offset]
    adox    r10, r9
    not     r10
    adcx    r10, QWORD PTR [rdi + \offset]
    mov     QWORD PTR [rdi + \offset], r10
    mov     r9,  r11
.endm

#   -------------------------
#
#        MULX/ADOX/ADCX
#
#   -------------------------

submul_one_zen4:
.cfi_startproc

.Lsubmul_one_zen4_start_of_func:

    xchg    rcx, rdx

    mov     r8,  rcx
    and     r8,  3
    shr     rcx, 2

    xor     r9,  r9
    test    rcx, rcx
    jz      .Lsubmul_one_zen4_before_rmdr

.Lsubmul_one_zen4_unroll_loop:

    ADX_MULX_ITER 0
    ADX_MULX_ITER 8
    ADX_MULX_ITER 16
    ADX_MULX_ITER 24

    lea     rdi, [rdi + 32]
    lea     rsi, [rsi + 32]
    lea     rcx, [rcx - 1]
    jrcxz   .Lsubmul_one_zen4_before_rmdr

.Lsubmul_one_zen4_before_rmdr:

    mov     rcx, r8
    jrcxz   .Lsubmul_one_zen4_end_of_loop

.Lsubmul_one_zen4_loop_rmdr:

    ADX_MULX_ITER 0

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    loop    .Lsubmul_one_zen4_loop_rmdr

.Lsubmul_one_zen4_end_of_loop:

    adox    r9,  rcx
    not     r9
    adcx    r9,  QWORD PTR [rdi]
    mov     QWORD PTR [rdi], r9

.Lsubmul_one_zen4_end_of_func:

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
.cfi_start_proc

.Lsubmul_one_x64_start_of_func:

    xchg    rcx, rdx
    mov     r9,  rdx
    xor     r10, r10
    test    rcx, rcx
    jz      .Lsubmul_one_x64_end_of_func

.Lsubmul_one_x64_main_loop:

    mul     QWORD PTR [rsi] # rdx:rax = rax * op1[idx]

    ; mul clobbers the original Carry Flag value

    add     r10, rax                # temp_reg += low64
    adc     rdx, 0                  # high64 += CF
    sub     QWORD PTR [rdi], r10    # result[i] -= temp_reg

    mov     r10, rdx        # temp_reg = high64
    mov     rax, r9         # restore clobbered rax
    adc     r10, 0          # temp_reg += borrow (from last seg)

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 8]
    dec     rcx
    jnz     .Lsubmul_one_x64_main_loop

.Lsubmul_one_x64_end_of_loop:

    sbb     QWORD PTR [rdi], rdx

.Lsubmul_one_x64_end_of_func:

    setc    al
    movzx   rax, al

.cfi_endproc
.size submul_one_x64, .-submul_one_x64
