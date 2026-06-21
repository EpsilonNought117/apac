
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
.globl mul_one_x64, mul_one_zen4
.type  mul_one_x64, @function
.type  mul_one_zen4, @function

mul_one_zen4:
.cfi_startproc

.Lzen4_start_of_func:
    
    xchg    rcx, rdx    # convenient, innit?
    mov     r8,  rcx
    and     r8,  7
    shr     rcx, 3
    
    mov     rax, 0
    test    rcx, rcx
    jz      .Lzen4_before_remainder

.p2align 6
.Lzen4_unroll8_loop:

.set i, 0
.rept 8

    mulx    r11, r10, QWORD PTR [rsi + i * 8]
    adc     r10, rax
    mov     QWORD PTR [rdi + i * 8], r10
    mov     rax, r11

.set i, i + 1
.endr

    lea     rsi, [rsi + 64]
    lea     rdi, [rdi + 64]
    dec     rcx
    jnz     .Lzen4_unroll8_loop

.p2align 4
.Lzen4_before_remainder:

    setc    r9b
    test    r8,  r8
    bt      r9w, 0
    jz      .Lzen4_end_of_loop

.p2align 6
.Lzen4_rmdr_loop:

    mulx    r11, r10, QWORD PTR [rsi]
    adc     r10, rax
    mov     QWORD PTR [rdi], r10
    mov     rax, r11

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 8]
    dec     r8
    jnz     .Lzen4_rmdr_loop

.p2align 5
.Lzen4_end_of_loop:

    adc     rax, 0
    mov     QWORD PTR [rdi], rax

.Lzen4_end_of_func:

    ret

.cfi_endproc
.size mul_one_zen4, .-mul_one_zen4

#   -------------------------
#
#            MUL/ADC
#
#   -------------------------

mul_one_x64:
.cfi_startproc

    xchg    rcx, rdx
    mov     r9,  rdx
    xor     rdx, rdx
    xor     r10, r10
    mov     rax, r9
    test    rcx, rcx
    jz      .Lx64_end_of_func

.Lx64_main_loop:

    mul     QWORD PTR [rsi]

    add     r10, rax
    adc     rdx, 0
    mov     QWORD PTR [rdi], r10

    mov     r10, rdx
    mov     rax, r9

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     rcx
    jnz     .Lx64_main_loop

.Lx64_end_of_loop:

    mov     QWORD PTR [rdi], r10

.Lx64_end_of_func:

    ret

.cfi_endproc
.size mul_one_x64, .-mul_one_x64

.section .note.GNU-stack,"",@progbits
