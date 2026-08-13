
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                        BASECASE SQUARING FUNCTIONS                        |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

SQR_BASECASE SEGMENT ALIGN(64) 'CODE'

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result       (apn_dig_t*)   
    ;   rdx -> op1          (const apn_dig_t*)
    ;   r8  -> size         (apn_size_t)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

;   Optimized routine for AMD Zen 4

sqr_bc_zen4 PROC FRAME

    push    rbp
.pushreg    rbp
    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
.endprolog

start_of_func:

    xchg    rbp, rcx        ; free up rcx for jrcxz/loop
    xchg    rbx, rdx        ; free up rdx for mul

    cmp     r8,  1
    jne     pass1

case1:

    mov     rdx, QWORD PTR [rbx]
    
    mulx    rdi, rsi, rdx
    mov     QWORD PTR [rbp], rsi
    mov     QWORD PTR [rbp + 8], rdi

    jmp     end_of_func

pass1:

    ; result[0] = 0
    mov     QWORD PTR [rbp], 0

    mov     r11, r8
    dec     r11
    jz      pass3

    mov     r10, r11
    shl     r10, 3
    add     rbp, 8

    mov     rdx, QWORD PTR [rbx]
    xor     rax, rax

    mov     rcx, r11
    mov     r9,  r11
    shr     rcx, 2
    and     r9,  3

    test    rcx, rcx
    jz      pass1_before_rmdr

ALIGN 16
pass1_inner_loop_unroll:

i = 0
WHILE i LT 4

    mulx    rdi, rsi, QWORD PTR [rbx + i * 8 + 8]
    adc     rsi, rax
    mov     QWORD PTR [rbp + i * 8], rsi
    mov     rax, rdi

i = i + 1
ENDM

    lea     rbx, [rbx + 32]
    lea     rbp, [rbp + 32]
    dec     rcx
    jnz     pass1_inner_loop_unroll

pass1_before_rmdr:

    mov     rcx, r9
    jrcxz   pass1_end

ALIGN 16
pass1_inner_loop_rmdr:

    mulx    rdi, rsi, QWORD PTR [rbx + 8]
    adc     rsi, rax
    mov     QWORD PTR [rbp], rsi
    mov     rax, rdi

    lea     rbx, [rbx + 8]
    lea     rbp, [rbp + 8]
    dec     rcx
    jnz     pass1_inner_loop_rmdr

pass1_end:

    adc     rax, 0
    mov     QWORD PTR [rbp], rax

    sub     rbx, r10
    sub     rbp, r10
    add     rbx, 8
    add     rbp, 16

    sub     r10, 8
    dec     r11
    jz      before_pass3

pass2:

    sub     r10, 8

ALIGN 16
pass2_outer_loop_start:

    mov     rdx, QWORD PTR [rbx]
    mov     rax, QWORD PTR [rbp]

    lea     rcx, [r11 - 1]
    mov     r9,  rcx

    shr     rcx, 3
    and     r9,  7
    
    test    rcx, rcx
    jz      pass2_before_inner_rmdr

ALIGN 16
pass2_inner_loop_unroll:

i = 0
WHILE i LT 8

    mulx    rdi, rsi, QWORD PTR [rbx + i * 8 + 8]
    adcx    rsi, rax
    adox    rdi, QWORD PTR [rbp + i * 8 + 8]
    mov     QWORD PTR [rbp + i * 8], rsi
    mov     rax, rdi

i = i + 1
ENDM

    lea     rbx, [rbx + 64]
    lea     rbp, [rbp + 64]
    lea     rcx, [rcx - 1]
    jrcxz   pass2_before_inner_rmdr
    jmp     pass2_inner_loop_unroll

ALIGN 16
pass2_before_inner_rmdr:

    mov     rcx, r9
    jrcxz   pass2_outer_loop_end

ALIGN 16
pass2_inner_loop_rmdr:

    mulx    rdi, rsi, QWORD PTR [rbx + 8]
    adcx    rsi, rax
    adox    rdi, QWORD PTR [rbp + 8]
    mov     QWORD PTR [rbp], rsi
    mov     rax, rdi

    lea     rbx, [rbx + 8]
    lea     rbp, [rbp + 8]
    loop    pass2_inner_loop_rmdr

ALIGN 16
pass2_outer_loop_end:

    mulx    rdi, rsi, QWORD PTR [rbx + 8]
    adcx    rsi, rax
    adox    rdi, rcx
    
    mov     QWORD PTR [rbp], rsi
    adcx    rdi, rcx
    mov     QWORD PTR [rbp + 8], rdi

    sub     rbx, r10
    sub     rbp, r10
    add     rbx, 8
    add     rbp, 16

    sub     r10, 8
    dec     r11
    jnz     pass2_outer_loop_start

before_pass3:

    mov     QWORD PTR [rbp], 0

    mov     r10, r8
    mov     r11, r8

    shl     r10, 3
    shl     r11, 4

    sub     rbx, r10
    sub     rbp, r11

    add     rbx, 8
    add     rbp, 8

pass3:

    mov     rcx, r8
    mov     r9,  r8
    shr     rcx, 2
    and     r9,  3

    test    rcx, rcx
    jz      pass3_before_rmdr

ALIGN 16
pass3_unroll_loop:

i = 0
WHILE i LT 4

    mov     rdx, QWORD PTR [rbx + i * 8]

    mov     r10, QWORD PTR [rbp + i * 16]
    mov     r11, QWORD PTR [rbp + i * 16 + 8]
    
    mulx    rdi, rsi, rdx
    adcx    r10, r10
    adox    r10, rsi
    adcx    r11, r11
    adox    r11, rdi

    mov     QWORD PTR [rbp + i * 16], r10
    mov     QWORD PTR [rbp + i * 16 + 8], r11
        
i = i + 1
ENDM        

    lea     rbx, [rbx + 32]
    lea     rbp, [rbp + 64]
    lea     rcx, [rcx - 1]
    jrcxz   pass3_before_rmdr
    jmp     pass3_unroll_loop

ALIGN 16
pass3_before_rmdr:

    mov     rcx, r9
    jrcxz   end_of_func

ALIGN 16
pass3_rmdr_loop:

    mov     rdx, QWORD PTR [rbx]
    
    mov     r10, QWORD PTR [rbp]
    mov     r11, QWORD PTR [rbp + 8]
    
    mulx    rdi, rsi, rdx
    adcx    r10, r10
    adox    r10, rsi
    adcx    r11, r11
    adox    r11, rdi

    mov     QWORD PTR [rbp], r10
    mov     QWORD PTR [rbp + 8], r11

    lea     rbx, [rbx + 8]
    lea     rbp, [rbp + 16]
    loop    pass3_rmdr_loop

end_of_func:

    pop     rsi
    pop     rdi
    pop     rbx
    pop     rbp
    ret
    
sqr_bc_zen4 ENDP

;   -------------------------
;
;           MUL/ADC        
;
;   -------------------------

; generic x64 fallback routine

sqr_bc_x64 PROC FRAME

    push    rbx
.pushreg    rbx
    push    rsi
.pushreg    rsi
    push    rdi
.pushreg    rdi
    push    r12
.pushreg    r12
.endprolog

start_of_func:

    mov     rbx, rdx    ; free up rdx for mul

    cmp     r8,  1
    jne     pass1

case1:

    mov     rax, QWORD PTR [rbx]
    mul     rax

    mov     QWORD PTR [rcx], rax
    mov     QWORD PTR [rcx + 8], rdx

    jmp     end_of_func

pass1:

    ; result[0] = 0
    mov     QWORD PTR [rcx], 0

    mov     r9,  r8
    dec     r9
    jz      pass4

    mov     r12, r9
    shl     r12, 3
    add     rcx, 8

pass1_outer:

    mov     r10, QWORD PTR [rbx]
    xor     r11, r11
    mov     rax, r10

    mov     rdi, r9
    mov     rsi, r9
    shr     rdi, 2
    and     rsi, 3

    test    rdi, rdi
    jz      pass1_before_rmdr

ALIGN 16
pass1_loop_unroll:

i = 0
WHILE i LT 4

    mul     QWORD PTR [rbx + i * 8 + 8]

    add     rax, r11
    adc     rdx, 0

    mov     QWORD PTR [rcx + i * 8], rax
    mov     r11, rdx
    mov     rax, r10

i = i + 1
ENDM

    add     rbx, 32
    add     rcx, 32
    dec     rdi
    jnz     pass1_loop_unroll
    
pass1_before_rmdr:

    test    rsi, rsi
    jz      pass1_end

ALIGN 16
pass1_loop_rmdr:

    mul     QWORD PTR [rbx + 8]
    
    add     rax, r11
    adc     rdx, 0

    mov     QWORD PTR [rcx], rax
    mov     r11, rdx
    mov     rax, r10

    add     rbx, 8
    add     rcx, 8
    dec     rsi
    jnz     pass1_loop_rmdr

pass1_end:

    mov     QWORD PTR [rcx], r11
    
    sub     rbx, r12
    sub     rcx, r12
    add     rbx, 8
    add     rcx, 16

    sub     r12, 8
    dec     r9
    jz      before_pass3

ALIGN 16
pass2_start:

    mov     r10, QWORD PTR [rbx]
    xor     r11, r11
    mov     rax, r10

    mov     rdi, r9
    mov     rsi, r9
    shr     rdi, 2
    and     rsi, 3

    test    rdi, rdi
    jz      pass2_inner_before_rmdr

ALIGN 16
pass2_inner_loop_unroll:

i = 0
WHILE i LT 4

    mul     QWORD PTR [rbx + i * 8 + 8]

    add     rax, QWORD PTR [rcx + i * 8]
    adc     rdx, 0
    add     rax, r11
    adc     rdx, 0

    mov     QWORD PTR [rcx + i * 8], rax
    mov     r11, rdx
    mov     rax, r10

i = i + 1
ENDM

    add     rbx, 32
    add     rcx, 32
    dec     rdi
    jnz     pass2_inner_loop_unroll

ALIGN 16
pass2_inner_before_rmdr:

    test    rsi, rsi
    jz      pass2_end

ALIGN 16
pass2_inner_loop_rmdr:

    mul     QWORD PTR [rbx + 8]

    add     rax, QWORD PTR [rcx]
    adc     rdx, 0
    add     rax, r11
    adc     rdx, 0

    mov     QWORD PTR [rcx], rax
    mov     r11, rdx
    mov     rax, r10

    add     rbx, 8
    add     rcx, 8
    dec     rsi
    jnz     pass2_inner_loop_rmdr

ALIGN 16
pass2_end:

    mov     QWORD PTR [rcx], r11

    sub     rbx, r12
    sub     rcx, r12
    add     rbx, 8
    add     rcx, 16
    
    sub     r12, 8
    dec     r9
    jnz     pass2_start

before_pass3:
    
    mov     QWORD PTR [rcx], 0

    mov     r10, r8
    mov     r11, r8

    shl     r10, 3
    shl     r11, 4

    sub     rbx, r10
    sub     rcx, r11

    add     rbx, 8
    add     rcx, 8

pass3:

    xor     r10, r10
    xor     rax, rax
    
    mov     rdi, r8
    mov     rsi, r8
    
    dec     rdi
    dec     rsi
    
    shr     rdi, 2
    and     rsi, 3

    test    rdi, rdi
    jz      pass3_before_rmdr

ALIGN 16
pass3_loop_unroll:

i = 0
WHILE i LT 4

    mov     r10, QWORD PTR [rcx + i * 16 + 8]
    adc     r10, r10
    mov     QWORD PTR [rcx + i * 16 + 8], r10

    mov     r10, QWORD PTR [rcx + i * 16 + 16]
    adc     r10, r10
    mov     QWORD PTR [rcx + i * 16 + 16], r10

i = i + 1
ENDM

    lea     rcx, [rcx + 64]
    dec     rdi
    jnz     pass3_loop_unroll

pass3_before_rmdr:

    setc    al
    test    rsi, rsi
    bt      ax, 0
    jz      pass3_end

ALIGN 16
pass3_loop_rmdr:

    mov     r10, QWORD PTR [rcx + 8]
    adc     r10, r10
    mov     QWORD PTR [rcx + 8], r10

    mov     r10, QWORD PTR [rcx + 16]
    adc     r10, r10
    mov     QWORD PTR [rcx + 16], r10

    lea     rcx, [rcx + 16]
    dec     rsi
    jnz     pass3_loop_rmdr

pass3_end:

    adc     QWORD PTR [rcx + 8], 0

before_pass4:

    mov     r11, r8
    dec     r11

    shl     r11, 4
    sub     rcx, r11

pass4:

    mov     rdi, r8
    mov     rsi, r8
    xor     r11d, r11d
    xor     r9d, r9d

    shr     rdi, 2
    and     rsi, 3

    test    rdi, rdi
    jz      pass4_before_rmdr

ALIGN 16
pass4_loop_unroll:

i = 0
WHILE i LT 4

    mov     rax, QWORD PTR [rbx + i * 8]
    mul     rax

    add     r11b, -1
    adc     QWORD PTR [rcx + i * 16], rax
    adc     QWORD PTR [rcx + i * 16 + 8], rdx
    setc    r11b

i = i + 1
ENDM

    add     rbx, 32
    add     rcx, 64
    dec     rdi
    jnz     pass4_loop_unroll

pass4_before_rmdr:

    test    rsi, rsi
    jz      end_of_func

ALIGN 16
pass4_loop_rmdr:

    mov     rax, QWORD PTR [rbx]
    mul     rax

    add     r11b, -1
    adc     QWORD PTR [rcx], rax
    adc     QWORD PTR [rcx + 8], rdx
    setc    r11b

    add     rbx, 8
    add     rcx, 16
    dec     rsi
    jnz     pass4_loop_rmdr

end_of_func:

    pop     r12
    pop     rdi
    pop     rsi
    pop     rbx
    ret

sqr_bc_x64 ENDP

SQR_BASECASE ENDS

END