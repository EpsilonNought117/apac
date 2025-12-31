
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

.Lzen4_start_of_func:
    
    xchg    rcx, rdx
    mov     r8,  rcx
    and     r8,  7
    shr     rcx, 3
    lea     r9,  [rip + .Lzen4_jump_table]
    lea     r9,  [r9 + r8 * 8]
    
    mov     rax, QWORD PTR [rdi]
    test    rcx, rcx
    jz      .Lzen4_before_remainder

.p2align 6
.Lzen4_unroll8_loop:

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
    jrcxz   .Lzen4_before_remainder
    jmp     .Lzen4_unroll8_loop

.p2align 4
.Lzen4_before_remainder:

    jmp     QWORD PTR [r9]

.p2align 4
.Lzen4_jump_table:

    .quad .Lzen4_end_of_loop
    .quad .Lzen4_rem1
    .quad .Lzen4_rem2
    .quad .Lzen4_rem3
    .quad .Lzen4_rem4
    .quad .Lzen4_rem5
    .quad .Lzen4_rem6
    .quad .Lzen4_rem7

.macro REM_CASE outer
.p2align 4

.Lzen4_rem\outer\():

.set i, 0
.rept \outer

    mulx    r11, r10, QWORD PTR [rsi + i * 8]
    adcx    r10, rax
    adox    r11, QWORD PTR [rdi + i * 8 + 8]
    mov     QWORD PTR [rdi + i * 8], r10
    mov     rax, r11

.set i, i + 1
.endr

    jmp     .Lzen4_end_of_loop

.endm

REM_CASE 7
REM_CASE 6
REM_CASE 5
REM_CASE 4
REM_CASE 3
REM_CASE 2
REM_CASE 1

.p2align 5
.Lzen4_end_of_loop:

    adcx    rax, rcx
    mov     QWORD PTR [rdi + r8 * 8], rax

.Lzen4_end_of_func:

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
    mov     r9,  rdx
    xor     rdx, rdx
    xor     r10, r10
    test    rcx, rcx
    jz      .Lx64_end_of_func

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

    adc     QWORD PTR [rdi], rdx

.Lx64_end_of_func:

    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size addmul_one_x64, .-addmul_one_x64

