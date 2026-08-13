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

.Lx64_start_of_func:

    mov     rbx, rdx    # free up rdx for mul
    
    # size now in rbx

    cmp     rbx, 1
    jne     .Lx64_pass1

.Lx64_case1:

    mov     rax, QWORD PTR [rsi] 
    mul     rax

    mov     QWORD PTR [rdi], rax
    mov     QWORD PTR [rdi + 8], rdx

    jmp     .Lx64_end_of_func

.Lx64_pass1:

    mov     QWORD PTR [rdi], 0

    mov     r12, rbx
    dec     r12
    jz      .Lx64_pass4

    mov     r8,  r12
    shl     r8,  3
    add     rdi, 8
    
    mov     r10, QWORD PTR [rsi]
    mov     rax, r10
    xor     r11, r11

    mov     rcx, r12
    mov     r9,  r12
    shr     rcx, 2
    and     r9,  3

    test    rcx, rcx
    jz      .Lx64_pass1_before_rmdr

.p2align 4
.Lx64_pass1_loop_unroll:

.set i, 0
.rept 4

    mul     QWORD PTR [rsi + i * 8 + 8]
    
    add     rax, r11
    adc     rdx, 0

    mov     QWORD PTR [rdi + i * 8], rax
    mov     r11, rdx
    mov     rax, r10

.set i, i + 1
.endr

    add     rsi, 32
    add     rdi, 32
    dec     rcx
    jnz     .Lx64_pass1_loop_unroll

.Lx64_pass1_before_rmdr:

    mov     rcx, r9
    jrcxz   .Lx64_pass1_end

.p2align 4
.Lx64_pass1_loop_rmdr:

    mul     QWORD PTR [rsi + 8]
    
    add     rax, r11
    adc     rdx, 0

    mov     QWORD PTR [rdi], rax
    mov     r11, rdx
    mov     rax, r10
    
    add     rsi, 8
    add     rdi, 8
    dec     rcx
    jnz     .Lx64_pass1_loop_rmdr

.Lx64_pass1_end:

    mov     QWORD PTR [rdi], r11

    sub     rsi, r8
    sub     rdi, r8

    add     rsi, 8
    add     rdi, 16

    sub     r8,  8
    dec     r12
    jz      .Lx64_before_pass3

.p2align 4
.Lx64_pass2_start:

    mov     r10, QWORD PTR [rsi]
    xor     r11, r11
    mov     rax, r10

    mov     rcx, r12
    mov     r9,  r12
    shr     rcx, 2
    and     r9,  3

    test    rcx, rcx
    jz      .Lx64_pass2_before_inner_rmdr

.p2align 4
.Lx64_pass2_inner_loop_unroll:

.set i, 0
.rept 4

    mul     QWORD PTR [rsi + i * 8 + 8]

    add     rax, QWORD PTR [rdi + i * 8]
    adc     rdx, 0
    add     rax, r11
    adc     rdx, 0

    mov     QWORD PTR [rdi + i * 8], rax
    mov     r11, rdx
    mov     rax, r10

.set i, i + 1
.endr

    add     rsi, 32
    add     rdi, 32
    dec     rcx
    jnz     .Lx64_pass2_inner_loop_unroll

.p2align 4
.Lx64_pass2_before_inner_rmdr:

    mov     rcx, r9
    jrcxz   .Lx64_pass2_end

.p2align 4
.Lx64_pass2_inner_loop_rmdr:

    mul     QWORD PTR [rsi + 8]

    add     rax, QWORD PTR [rdi]
    adc     rdx, 0
    add     rax, r11
    adc     rdx, 0

    mov     QWORD PTR [rdi], rax
    mov     r11, rdx
    mov     rax, r10

    add     rsi, 8
    add     rdi, 8
    dec     rcx
    jnz     .Lx64_pass2_inner_loop_rmdr

.p2align 4
.Lx64_pass2_end:

    mov     QWORD PTR [rdi], r11

    sub     rsi, r8
    sub     rdi, r8
    add     rsi, 8
    add     rdi, 16

    sub     r8, 8
    dec     r12
    jnz     .Lx64_pass2_start

.Lx64_before_pass3:

    mov     QWORD PTR [rdi], 0

    mov     r10, rbx
    mov     r11, rbx

    shl     r10, 3
    shl     r11, 4

    sub     rsi, r10
    sub     rdi, r11

    add     rsi, 8
    add     rdi, 8

.Lx64_pass3_start:

    xor     rax, rax
    mov     rcx, rbx
    mov     r9,  rbx

    dec     rcx
    dec     r9

    shr     rcx, 2
    and     r9,  3

    test    rcx, rcx
    jz      .Lx64_pass3_before_rmdr

.p2align 4
.Lx64_pass3_loop_unroll:

.set i, 0
.rept 4

    mov     rax, QWORD PTR [rdi + i * 16 + 8]
    adc     rax, rax
    mov     QWORD PTR [rdi + i * 16 + 8], rax

    mov     rax, QWORD PTR [rdi + i * 16 + 16]
    adc     rax, rax
    mov     QWORD PTR [rdi + i * 16 + 16], rax

.set i, i + 1
.endr

    lea     rdi, [rdi + 64]
    dec     rcx
    jnz     .Lx64_pass3_loop_unroll

.Lx64_pass3_before_rmdr:

    mov     rcx, r9
    jrcxz   .Lx64_pass3_end

.p2align 4
.Lx64_pass3_loop_rmdr:

    mov     rax, QWORD PTR [rdi + 8]
    adc     rax, rax
    mov     QWORD PTR [rdi + 8], rax

    mov     rax, QWORD PTR [rdi + 16]
    adc     rax, rax
    mov     QWORD PTR [rdi + 16], rax

    lea     rdi, [rdi + 16]
    dec     rcx
    jnz     .Lx64_pass3_loop_rmdr

.Lx64_pass3_end:

    adc     QWORD PTR [rdi + 8], 0

.Lx64_before_pass4:

    mov     r11, rbx
    dec     r11
    shl     r11, 4
    sub     rdi, r11

.Lx64_pass4:

    xor     rcx, rcx

    mov     r10, rbx
    mov     r11, rbx

    shr     r10, 2
    and     r11, 3

    test    r10, r10
    jz      .Lx64_pass4_before_rmdr

.p2align 4
.Lx64_pass4_loop_unroll:

.set i, 0
.rept 4

    mov     rax, QWORD PTR [rsi + i * 8]
    mul     rax

    add     cl, -1
    adc     QWORD PTR [rdi + i * 16], rax
    adc     QWORD PTR [rdi + i * 16 + 8], rdx
    setc    cl

.set i, i + 1
.endr

    add     rsi, 32
    add     rdi, 64
    dec     r10
    jnz     .Lx64_pass4_loop_unroll

.Lx64_pass4_before_rmdr:

    test    r11, r11
    jz      .Lx64_end_of_func

.p2align 4
.Lx64_pass4_loop_rmdr:

    mov     rax, QWORD PTR [rsi]
    mul     rax

    add     cl, -1
    adc     QWORD PTR [rdi], rax
    adc     QWORD PTR [rdi + 8], rdx
    setc    cl

    add     rsi, 8
    add     rdi, 16
    dec     r11
    jnz     .Lx64_pass4_loop_rmdr

.Lx64_end_of_func:

    pop     r12
.cfi_adjust_cfa_offset -8

    pop     rbx
.cfi_adjust_cfa_offset -8
    
    ret

.cfi_endproc
.size sqr_bc_x64, .-sqr_bc_x64

.section .note.GNU-stack,"",@progbits
