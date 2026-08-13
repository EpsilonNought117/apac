#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                        BASECASE SQUARING FUNCTIONS                        |
#   |                                                                           |
#   O---------------------------------------------------------------------------O
    #   Function Arguments
    #
    #   rdi -> result       (apn_dig_t*)
    #   rsi -> op1          (const apn_dig_t*)
    #   rdx -> size         (apn_size_t)

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

sqr_bc_zen4:

.cfi_startproc

    push    rbx
.cfi_adjust_cfa_offset 8
.cfi_rel_offset rbx, 0

    push    r12
.cfi_adjust_cfa_offset 8
.cfi_rel_offset r12, 0

.Lzen4_start_of_func:

    mov     rbx, rdx

    # now size arg is in rbx
    # and rdx is free for mulx

    cmp     rbx, 1
    jne     .Lzen4_pass1

.Lzen4_case1:

    mov     rdx, QWORD PTR [rsi]

    mulx    r11, r10, rdx

    mov     QWORD PTR [rdi], r10
    mov     QWORD PTR [rdi + 8], r11

    jmp     .Lzen4_end_of_func

.Lzen4_pass1:

    # result[0] = 0
    mov     QWORD PTR [rdi], 0

    mov     r12, rbx
    dec     r12
    jz      .Lzen4_pass3

    mov     r8,  r12
    shl     r8,  3
    add     rdi, 8

    mov     rdx, QWORD PTR [rsi]
    xor     rax, rax

    mov     rcx, r12
    mov     r9,  r12

    shr     rcx, 2
    and     r9,  3

    test    rcx, rcx
    jz      .Lzen4_pass1_before_rmdr

.p2align 4
.Lzen4_pass1_loop_unroll:

.set i, 0
.rept 4

    mulx    r11, r10, QWORD PTR [rsi + i * 8 + 8]
    adc     r10, rax
    mov     QWORD PTR [rdi + i * 8], r10
    mov     rax, r11

.set i, i + 1
.endr

    lea     rsi, [rsi + 32]
    lea     rdi, [rdi + 32]
    dec     rcx
    jnz     .Lzen4_pass1_loop_unroll

.Lzen4_pass1_before_rmdr:

    mov     rcx, r9
    jrcxz   .Lzen4_pass1_end

.p2align 4
.Lzen4_pass1_loop_rmdr:

    mulx    r11, r10, QWORD PTR [rsi + 8]
    adc     r10, rax
    mov     QWORD PTR [rdi], r10
    mov     rax, r11

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 8]
    dec     rcx
    jnz     .Lzen4_pass1_loop_rmdr

.Lzen4_pass1_end:

    adc     rax, 0
    mov     QWORD PTR [rdi], rax

    sub     rsi, r8
    sub     rdi, r8
    add     rsi, 8
    add     rdi, 16

    sub     r8,  8
    dec     r12
    jz      .Lzen4_before_pass3

.Lzen4_pass2:

    sub     r8,  8

.p2align 4
.Lzen4_pass2_start:

    mov     rax, QWORD PTR [rdi]
    mov     rdx, QWORD PTR [rsi]

    lea     rcx, [r12 - 1]
    mov     r9,  rcx
    shr     rcx, 3
    and     r9,  7

    test    rcx, rcx
    jz      .Lzen4_pass2_before_inner_rmdr

.p2align 4
.Lzen4_pass2_inner_loop_unroll:

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
    jrcxz   .Lzen4_pass2_before_inner_rmdr
    jmp     .Lzen4_pass2_inner_loop_unroll

.p2align 4
.Lzen4_pass2_before_inner_rmdr:

    mov     rcx, r9
    jrcxz   .Lzen4_pass2_end

.p2align 4
.Lzen4_pass2_inner_loop_rmdr:

    mulx    r11, r10, QWORD PTR [rsi + 8]
    adcx    r10, rax
    adox    r11, QWORD PTR [rdi + 8]
    mov     QWORD PTR [rdi], r10
    mov     rax, r11

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 8]
    loop    .Lzen4_pass2_inner_loop_rmdr

.p2align 4
.Lzen4_pass2_end:

    mulx    r11, r10, QWORD PTR [rsi + 8]
    adcx    r10, rax
    adox    r11, rcx

    mov     QWORD PTR [rdi], r10
    adcx    r11, rcx
    mov     QWORD PTR [rdi + 8], r11

    sub     rsi, r8
    sub     rdi, r8
    add     rsi, 8
    add     rdi, 16

    sub     r8,  8
    dec     r12
    jnz     .Lzen4_pass2_start

.Lzen4_before_pass3:

    mov     QWORD PTR [rdi], 0

    mov     r10, rbx
    mov     r11, rbx

    shl     r10, 3
    shl     r11, 4

    sub     rsi, r10
    sub     rdi, r11

    add     rsi, 8
    add     rdi, 8

.Lzen4_pass3:

    mov     rcx, rbx
    mov     r9,  rbx
    shr     rcx, 2
    and     r9,  3
    
    test    rcx, rcx
    jz      .Lzen4_pass3_before_rmdr

.p2align 4
.Lzen4_pass3_loop_unroll:

.set i, 0
.rept 4

    mov     rdx, QWORD PTR [rsi + i * 8]
    mov     rbx, QWORD PTR [rdi + i * 16]
    mov     rax, QWORD PTR [rdi + i * 16 + 8]

    mulx    r11, r10, rdx
    adcx    rbx, rbx
    adox    rbx, r10
    adcx    rax, rax
    adox    rax, r11

    mov     QWORD PTR [rdi + i * 16], rbx
    mov     QWORD PTR [rdi + i * 16 + 8], rax

.set i, i + 1
.endr

    lea     rsi, [rsi + 32]
    lea     rdi, [rdi + 64]
    lea     rcx, [rcx - 1]
    jrcxz   .Lzen4_pass3_before_rmdr
    jmp     .Lzen4_pass3_loop_unroll

.Lzen4_pass3_before_rmdr:

    mov     rcx, r9
    jrcxz   .Lzen4_end_of_func

.p2align 4
.Lzen4_pass3_loop_rmdr:

    mov     rdx, QWORD PTR [rsi]
    mov     rbx, QWORD PTR [rdi]
    mov     rax, QWORD PTR [rdi + 8]

    mulx    r11, r10, rdx
    adcx    rbx, rbx
    adox    rbx, r10
    adcx    rax, rax
    adox    rax, r11

    mov     QWORD PTR [rdi], rbx
    mov     QWORD PTR [rdi + 8], rax

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 16]
    loop    .Lzen4_pass3_loop_rmdr

.Lzen4_end_of_func:

    pop     r12
.cfi_adjust_cfa_offset -8

    pop     rbx
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

