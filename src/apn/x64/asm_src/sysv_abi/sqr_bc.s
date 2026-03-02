#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                        BASECASE SQUARING FUNCTIONS                        |
#   |                                                                           |
#   O---------------------------------------------------------------------------O
    #   Function Arguments
    #
    #   rdi -> result       (ap_dig_t*)
    #   rsi -> op1          (const ap_dig_t*)
    #   rdx -> size         (ap_size_t)

.intel_syntax noprefix
.text
.globl sqr_bc_x64, sqr_bc_zen4
.type  sqr_bc_x64, @function
.type  sqr_bc_zen4, @function

#   -------------------------
#
#        MULX/ADOX/ADCX
#
#   -------------------------

# Optimized routine for AMD Zen 4

sqr_bc_zen4:

.cfi_startproc
    push    r12
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r12, 0
    push    r13
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r13, 0
    push    r14
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r14, 0

.Lzen4_start_of_func:

    mov     r14, rdx        # save original size in r14
    mov     r13, rdx        # copy of size in r13
    dec     r13             # curr_size = size - 1
    jz      .Lzen4_pass2

    mov     r8,  r13
    shl     r8,  3          # curr_size * sizeof(ap_dig_t)
    add     rdi, 8

.Lzen4_pass1_outer_loop_start:

    mov     rdx, QWORD PTR [rsi]
    mov     rax, QWORD PTR [rdi]
    
    mov     rcx, r13
    mov     r9,  r13        # for later finding the jump_table label
    shr     rcx, 3          # curr_size /= 8 (for 8x unrolled loop)
    and     r9,  7          # curr_size %= 8

    lea     r12, [rip + .Lzen4_pass1_jump_table]
    lea     r12, [r12 + r9 * 8]
    test    rcx, rcx
    jz      .Lzen4_pass1_before_rmdr

.p2align 6
.Lzen4_pass1_inner_unroll_loop:

.set i, 0
.rept 8

    mulx    r11, r10, QWORD PTR [rsi + i * 8 + 8]
    adcx    r10, rax
    adox    r11, QWORD PTR [rdi + i * 8 + 8]
    mov     QWORD PTR [rdi + i * 8], r10
    mov     rax, r11

.set i, i + 1
.endr

    lea     rsi, [rsi + 64]
    lea     rdi, [rdi + 64]
    lea     rcx, [rcx - 1]
    jrcxz   .Lzen4_pass1_before_rmdr
    jmp     .Lzen4_pass1_inner_unroll_loop

.p2align 4
.Lzen4_pass1_before_rmdr:

    jmp     QWORD PTR [r12]

.p2align 4
.Lzen4_pass1_jump_table:

    .quad .Lpass1_outer_loop_end
    .quad .Lpass1_rem1
    .quad .Lpass1_rem2
    .quad .Lpass1_rem3
    .quad .Lpass1_rem4
    .quad .Lpass1_rem5
    .quad .Lpass1_rem6
    .quad .Lpass1_rem7

.macro PASS1_REM_CASE count

.p2align 4
.Lpass1_rem\count\():

.set i, 0
.rept \count

    mulx    r11, r10, QWORD PTR [rsi + i * 8 + 8]
    adcx    r10, rax
    adox    r11, QWORD PTR [rdi + i * 8 + 8]
    mov     QWORD PTR [rdi + i * 8], r10
    mov     rax, r11

.set i, i + 1
.endr

    lea     rdi, [rdi + \count * 8]
    lea     rsi, [rsi + \count * 8]
    jmp     .Lpass1_outer_loop_end

.endm

# Generate all remainder cases from 7 down to 1
PASS1_REM_CASE 7
PASS1_REM_CASE 6
PASS1_REM_CASE 5
PASS1_REM_CASE 4
PASS1_REM_CASE 3
PASS1_REM_CASE 2
PASS1_REM_CASE 1

.p2align 4
.Lpass1_outer_loop_end:

    adc     QWORD PTR [rdi], rax
    sub     rsi, r8
    sub     rdi, r8
    add     rsi, 8
    add     rdi, 16
    sub     r8,  8
    dec     r13
    jnz     .Lzen4_pass1_outer_loop_start

.Lzen4_before_pass2:

    mov     r12, r14
    mov     r13, r14
    shl     r13, 4
    shl     r12, 3
    sub     rsi, r12
    sub     rdi, r13
    add     rdi, 8
    add     rsi, 8

.p2align 5
.Lzen4_pass2:

    mov     rcx, r14
    mov     r9,  r14
    shr     rcx, 2
    and     r9,  3

    test    rcx, rcx
    jz      .Lzen4_pass2_before_rmdr

.p2align 6
.Lzen4_pass2_unroll4_loop:

.set i, 0
.rept 4

    mov     rdx, QWORD PTR [rsi + i * 8]
    mov     r10, QWORD PTR [rdi + i * 16]
    mov     r11, QWORD PTR [rdi + i * 16 + 8]
    adcx    r10, r10
    adcx    r11, r11
    mulx    r13, r12, rdx
    adox    r10, r12
    adox    r11, r13
    mov     QWORD PTR [rdi + i * 16], r10
    mov     QWORD PTR [rdi + i * 16 + 8], r11

.set i, i + 1
.endr

    lea     rsi, [rsi + 32]
    lea     rdi, [rdi + 64]
    lea     rcx, [rcx - 1]
    jrcxz   .Lzen4_pass2_before_rmdr
    jmp     .Lzen4_pass2_unroll4_loop

.p2align 4
.Lzen4_pass2_before_rmdr:

    mov     rcx, r9
    jrcxz   .Lzen4_end_of_func

.p2align 4
.Lzen4_pass2_rmdr_loop:

    mov     rdx, QWORD PTR [rsi]
    mov     r10, QWORD PTR [rdi]
    mov     r11, QWORD PTR [rdi + 8]
    adcx    r10, r10
    adcx    r11, r11
    mulx    r13, r12, rdx
    adox    r10, r12
    adox    r11, r13
    mov     QWORD PTR [rdi], r10
    mov     QWORD PTR [rdi + 8], r11

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 16]
    loop    .Lzen4_pass2_rmdr_loop

.Lzen4_end_of_func:

    pop     r14
.cfi_adjust_cfa_offset -8
    pop     r13
.cfi_adjust_cfa_offset -8
    pop     r12
.cfi_adjust_cfa_offset -8
    ret

.cfi_endproc
.size sqr_bc_zen4, .-sqr_bc_zen4

#   -------------------------
#
#           MUL/ADC
#
#   -------------------------

sqr_bc_x64:

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

    #   5-limb bignum squaring matrix example with the
    #   symmetric non-diagonal terms marked as DUP
    #
    #   +-----+-----------+-----------+-----------+-----------+-----------+
    #   |     |    a0     |    a1     |    a2     |    a3     |    a4     |
    #   +-----+-----------+-----------+-----------+-----------+-----------+
    #   |  a0 |   a0*a0   |    DUP    |    DUP    |    DUP    |    DUP    |
    #   +-----+-----------+-----------+-----------+-----------+-----------+
    #   |  a1 |  2*a1*a0  |   a1*a1   |    DUP    |    DUP    |    DUP    |
    #   +-----+-----------+-----------+-----------+-----------+-----------+
    #   |  a2 |  2*a2*a0  |  2*a2*a1  |   a2*a2   |    DUP    |    DUP    |
    #   +-----+-----------+-----------+-----------+-----------+-----------+
    #   |  a3 |  2*a3*a0  |  2*a3*a1  |  2*a3*a2  |   a3*a3   |    DUP    |
    #   +-----+-----------+-----------+-----------+-----------+-----------+
    #   |  a4 |  2*a4*a0  |  2*a4*a1  |  2*a4*a2  |  2*a4*a3  |   a4*a4   |
    #   +-----+-----------+-----------+-----------+-----------+-----------+

    # the whole function works in 3 Passes

    # Pass 1: Accumulate non-diagonal products in the result
    #
    #   For an (n * n) sized squaring operation, there will be exactly (n * (n - 1)) / 2
    #   number of non-diagonal multiplications since those products occur twice in the
    #   multiplication matrix. In pass 1, add all those sums once.
    #
    # Pass 2: Shift left the accumulated non-diag prods by 1 bit
    #
    #   Shift the accumulated products left by 1 bit, essentially multiplying
    #   the whole lower-triangular part of the matrix by 2, getting the desired value.
    #
    # Pass 3: Accumulate the diagonal products (squares) in the result
    #
    #   These squares only need to be added once as shown along the diagonal.

    mov     rcx, rdx
    mov     r9,  rcx        # copy of size in r9
    dec     r9              # lower triangular matrix elems start with
                            # (n - 1) -> (n - 2) -> ... -> 2 -> 1 -> 0
    test    r9,  r9         # test if the basecase sqr size is 1, then no double sum prods
    jz      .Lx64_pass3     # do the single diagonal sqr prod

    # PASS-1 (O(n^2) step)

    xor     r12, r12        # counter (starts at 0)
    xor     r13, r13        # to restore rax after it has been clobbered by mul

.Lx64_pass1_outer_loop:

    # rdx:rax for accumulating the product via mul
    # r10 <- result (copy for loop)
    # r11 <- op1    (copy for loop)
    # r9 <- (size - 1) (copy for loop)

    mov     r8,  r9
    xor     rbx, rbx        # temp_reg
    xor     rdx, rdx        # high64 = 0
    mov     r13, QWORD PTR [rsi + r12 * 1]
    mov     rax, r13
    lea     r11, [rsi + r12 * 1 + 8]
    lea     r10, [rdi + r12 * 2 + 8]

.p2align 4
.Lx64_pass1_inner_loop:

    mul     QWORD PTR [r11] # rdx:rax = rax * op1[counter + 1]

    add     rbx, rax                # temp_reg += low64
    adc     rdx, 0                  # high64 += CF
    add     QWORD PTR [r10], rbx    # result[counter + 1] += temp_reg

    mov     rbx, 0
    mov     rax, r13                # restore clobbered rax or load for the first time
    adc     rbx, rdx                # temp_reg += (CF + high64)

    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    dec     r8
    jnz     .Lx64_pass1_inner_loop

.Lx64_pass1_outer_loop_end:
    adc     QWORD PTR [r10], rbx
    add     r12, 8
    dec     r9
    jnz     .Lx64_pass1_outer_loop

    # PASS-2 (O(n) step)

.Lx64_pass2:

    lea     r10, [rdi + 8]
    mov     r9,  rcx
    dec     r9

    # 2 * (n - 1) limbs in result need to be shifted left by 1
    # the first and last limb don't contain any values
    # limbs (inclusive) of result[1:2n-2]
    # Process two limbs at once for each decrement in rcx

    xor     r11, r11        # zeroes out the carry flag
                            # before the rcl
.Lx64_pass2_loop:
    
    rcl     QWORD PTR [r10], 1
    rcl     QWORD PTR [r10 + 8], 1

    lea     r10, [r10 + 16]
    dec     r9
    jnz     .Lx64_pass2_loop

    # PASS-3 (O(n) step)

.Lx64_pass2_end:

    adc     QWORD PTR [r10], 0

.Lx64_pass3:

    mov     r10, rdi
    mov     r11, rsi

    xor     r12, r12        # to clear CF/OF

.Lx64_pass3_loop:

    adc     r12, 0                      # accumulate CF from last iter as mul clobbers both
                                        # OF & CF, does nothing in first iter
    mov     rax, QWORD PTR [r11]
    mul     rax                         # rdx:rax = rax * rax (rax contains op1[i])
    add     r12, rax                    # mul can set both OF/CF, to take care of that
                                        # add rax to an empty r12
    add     QWORD PTR [r10], r12
    adc     QWORD PTR [r10 + 8], rdx
    mov     r12, 0

    lea     r11, [r11 + 8]
    lea     r10, [r10 + 16]
    loop    .Lx64_pass3_loop

.Lx64_end_of_func:

    pop     r13
.cfi_adjust_cfa_offset -8
    pop     r12
.cfi_adjust_cfa_offset -8
    pop     rbx
.cfi_adjust_cfa_offset -8
    ret

.cfi_endproc
.size sqr_bc_x64, .-sqr_bc_x64

.section .note.GNU-stack,"",@progbits

