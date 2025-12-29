
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

.macro ADX_MULX_ITER offset
    mulx    r11, r10, QWORD PTR [rsi + \offset]
    adcx    r10, rax
    adox    r11, QWORD PTR [rdi + \offset + 8]
    mov     QWORD PTR [rdi + \offset], r10
    mov     rax, r11
.endm

#   -------------------------
#
#        MULX/ADOX/ADCX        
#
#   -------------------------

addmul_one_zen4:
    .cfi_startproc

    jmp     .Laddmul_one_zen4_start_of_func

.p2align 4
.Laddmul_one_zen4_jump_table:
    .quad .Laddmul_one_zen4_end_of_loop
    .quad .Laddmul_one_zen4_rem1
    .quad .Laddmul_one_zen4_rem2
    .quad .Laddmul_one_zen4_rem3
    .quad .Laddmul_one_zen4_rem4
    .quad .Laddmul_one_zen4_rem5
    .quad .Laddmul_one_zen4_rem6
    .quad .Laddmul_one_zen4_rem7

.Laddmul_one_zen4_start_of_func:

    xchg    rcx, rdx

    mov     r8,  rcx
    and     r8,  7
    shr     rcx, 3

    lea     r9,  [rip + .Laddmul_one_zen4_jump_table]
    lea     r9,  [r9 + r8 * 8]

    mov     rax, QWORD PTR [rdi]
    test    rcx, rcx
    jz      .Laddmul_one_zen4_before_remainder

.p2align 6
.Laddmul_one_zen4_unroll8_loop:

    ADX_MULX_ITER 0
    ADX_MULX_ITER 8
    ADX_MULX_ITER 16
    ADX_MULX_ITER 24
    ADX_MULX_ITER 32
    ADX_MULX_ITER 40
    ADX_MULX_ITER 48
    ADX_MULX_ITER 56

    lea     rsi, [rsi + 64]
    lea     rdi, [rdi + 64]
    lea     rcx, [rcx - 1]
    jrcxz   .Laddmul_one_zen4_before_remainder
    jmp     .Laddmul_one_zen4_unroll8_loop

.p2align 4
.Laddmul_one_zen4_before_remainder:
    jmp     QWORD PTR [r9]

.p2align 4
.Laddmul_one_zen4_rem7:
    ADX_MULX_ITER 0
    ADX_MULX_ITER 8
    ADX_MULX_ITER 16
    ADX_MULX_ITER 24
    ADX_MULX_ITER 32
    ADX_MULX_ITER 40
    ADX_MULX_ITER 48

    jmp .Laddmul_one_zen4_end_of_loop

.p2align 4
.Laddmul_one_zen4_rem6:
    ADX_MULX_ITER 0
    ADX_MULX_ITER 8
    ADX_MULX_ITER 16
    ADX_MULX_ITER 24
    ADX_MULX_ITER 32
    ADX_MULX_ITER 40

    jmp .Laddmul_one_zen4_end_of_loop

.p2align 4
.Laddmul_one_zen4_rem5:
    ADX_MULX_ITER 0
    ADX_MULX_ITER 8
    ADX_MULX_ITER 16
    ADX_MULX_ITER 24
    ADX_MULX_ITER 32

    jmp .Laddmul_one_zen4_end_of_loop    

.p2align 4
.Laddmul_one_zen4_rem4:
    ADX_MULX_ITER 0
    ADX_MULX_ITER 8
    ADX_MULX_ITER 16
    ADX_MULX_ITER 24

    jmp .Laddmul_one_zen4_end_of_loop

.p2align 4
.Laddmul_one_zen4_rem3:
    ADX_MULX_ITER 0
    ADX_MULX_ITER 8
    ADX_MULX_ITER 16

    jmp .Laddmul_one_zen4_end_of_loop

.p2align 4
.Laddmul_one_zen4_rem2:
    ADX_MULX_ITER 0
    ADX_MULX_ITER 8

    jmp .Laddmul_one_zen4_end_of_loop

.p2align 4
.Laddmul_one_zen4_rem1:
    ADX_MULX_ITER 0

.p2align 5
.Laddmul_one_zen4_end_of_loop:
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
    jz      .Lend_of_func

.Laddmul_one_x64_main_loop:
    mov     rax, r9         # restore clobbered rax
    adc     r10, 0          # temp_reg += (CF + high64)
    mul     QWORD PTR [rsi] # rdx:rax = rax * op1[idx]

    add     r10, rax
    adc     rdx, 0
    add     QWORD PTR [rdi], r10

    mov     r10, rdx

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     rcx
    jnz     .Laddmul_one_x64_main_loop

.Laddmul_one_end_of_loop:
    adc     QWORD PTR [rdi], rdx

.Laddmul_one_end_of_func:
    setc    al
    movzx   rax, al
    ret

    .cfi_endproc
.size addmul_one_x64, .-addmul_one_x64