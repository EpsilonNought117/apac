
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                        BASECASE SQUARING FUNCTIONS                        |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (apn_seg_t*)   
    ;   rdx -> op1 (const apn_seg_t*)
    ;   r8  -> size (apn_size_t)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------


sqr_bc_zen4 PROC FRAME

    push    rbx
.pushreg    rbx
    push    rsi
.pushreg    rsi
    push    rdi
.pushreg    rdi
    push    r12
.pushreg    r12
    push    r13
.pushreg    r13
    push    r14
.pushreg    r14
    push    r15
.pushreg    r15
.endprolog

    xchg    rax, rcx    ; exchange rcx and rax, freeing up rcx for jrcxz/loop
    xchg    rbx, rdx    ; exchange rdx and rbx, freeing up rdx for mulx

    mov     r9,  r8
    dec     r9          
    
    test    r9,  r9
    jz      before_pass2

    ; The whole function works exactly as sqr_bc_x64 with a few changes
    ; Passes 1 and 2 can be merged into one thanks to adcx & adox from ADX
    ; Can propagate two "carries" per limb at once utilizing both OF & CF
    ; BMI2's mulx adds more convenience of not affecting any flags

    xor     r13, r13    ; counter (starts at 0)
    xor     r15, r15    ; register for holding 0

    ; Pass-1 (O(n^2) step)

outer_loop_pass1:

    mov     rdx, QWORD PTR [rbx + r13*8]    ; op1[i]
    lea     r10, [rax + r13*8 + 8]          
    lea     r11, [rbx + r13*8 + 8]
    mov     rcx, r9
    mov     r12, r9
    shr     rcx, 2      ; rcx /= 4
    and     r12, 3      ; r12 %= 4
    test    rcx, rcx
    jz      inner_loop_befr_rem_pass1

ALIGN 16
inner_loop_4unroll_pass1:

    mulx    rdi, rsi, QWORD PTR [r11]
    adcx    rsi, rsi
    adox    rsi, QWORD PTR [r10]
    mov     QWORD PTR [r10], rsi
    adcx    rdi, rdi
    adox    rdi, QWORD PTR [r10 + 8]
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 8]
    adcx    rsi, rsi
    adox    rsi, r14
    mov     QWORD PTR [r10 + 8], rsi
    adcx    rdi, rdi
    adox    rdi, QWORD PTR [r10 + 16]
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 16]
    adcx    rsi, rsi
    adox    rsi, r14
    mov     QWORD PTR [r10 + 16], rsi
    adcx    rdi, rdi
    adox    rdi, QWORD PTR [r10 + 24]
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 24]
    adcx    rsi, rsi
    adox    rsi, r14
    mov     QWORD PTR [r10 + 24], rsi
    adcx    rdi, rdi
    adox    rdi, QWORD PTR [r10 + 32]
    mov     QWORD PTR [r10 + 32], rdi

    lea     r11, [r11 + 32]
    lea     r10, [r11 + 32]
    lea     rcx, [rcx - 1]
    jrcxz   inner_loop_befr_rem_pass1
    jmp     inner_loop_4unroll_pass1

ALIGN 16
inner_loop_befr_rem_pass1:

    mov     rcx, r12
    jrcxz   outer_loop_end_pass1

ALIGN 16
inner_loop_rem_pass1:

    mulx    rdi, rsi, QWORD PTR [r11]
    adcx    rsi, rsi
    adox    rsi, QWORD PTR [r10]
    mov     QWORD PTR [r10], rsi
    adcx    rdi, rdi
    adox    rdi, QWORD PTR [r10 + 8]
    mov     QWORD PTR [r10 + 8], rdi

    lea     r11, [r11 + 8]
    lea     r10, [r11 + 8]
    loop    inner_loop_rem_pass1

ALIGN 16
outer_loop_end_pass1:

    mov     r14, QWORD PTR [r10]
    adcx    r14, r15
    adox    r14, r15
    mov     QWORD PTR [r10], r14
    inc     r13
    dec     r9
    jnz     outer_loop_pass1
    
    ; Pass-2 (O(n) step)

before_pass2:

    mov     r10, rax
    mov     r11, rbx
    mov     rcx, r8
    xor     r12, r12    ; to clear CF/OF

loop_pass2:

    mov     rdx, QWORD PTR [r11]
    mulx    rdi, rsi, rdx

    adc     QWORD PTR [r10], rsi
    adc     QWORD PTR [r10 + 8], rdi

    lea     r11, [r11 + 8]
    lea     r10, [r10  + 16]
    loop    loop_pass2

end_of_func:

    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rdi
    pop     rsi
    pop     rbx
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
    push    r13
.pushreg    r13
.endprolog

    ;   5-limb bignum squaring matrix example with the 
    ;   symmetric non-diagonal terms marked as DUP
    ;
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |     |    a0     |    a1     |    a2     |    a3     |    a4     |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a0 |   a0*a0   |    DUP    |    DUP    |    DUP    |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a1 |  2*a1*a0  |   a1*a1   |    DUP    |    DUP    |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a2 |  2*a2*a0  |  2*a2*a1  |   a2*a2   |    DUP    |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a3 |  2*a3*a0  |  2*a3*a1  |  2*a3*a2  |   a3*a3   |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a4 |  2*a4*a0  |  2*a4*a1  |  2*a4*a2  |  2*a4*a3  |   a4*a4   |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+

    ; the whole function works in 3 Passes

    ; Pass 1: Accumulate non-diagonal products in the result
    ;       
    ;   For an (n * n) sized squaring operation, there will be exactly (n * (n - 1)) / 2
    ;   number of non-diagonal multiplications since those products occur twice in the 
    ;   multiplication matrix. In pass 1, add all those sums once.
    ;
    ; Pass 2: Shift left the accumulated non-diag prods by 1 bit
    ;   
    ;   Shift the accumulated products left by 1 bit, essentially multiplying 
    ;   the whole lower-triangular part of the matrix by 2, getting the desired value.
    ;
    ; Pass 3: Accumulate the diagonal products (squares) in the result
    ;
    ;   These squares only need to be added once as shown along the diagonal.

    xchg    rdi, rcx        ; exchange rcx and rdi, freeing up rcx for jrcxz/loop
    xchg    rsi, rdx        ; exchange rdx and rsi, freeing up rdx for mul

    ; op1 in rsi, result in rdi

    mov     r9,  r8         ; copy of size in r9
    dec     r9              ; lower triangular matrix elems start with
                            ; (n - 1) -> (n - 2) -> ... -> 2 -> 1 -> 0
    test    r9,  r9         ; test if the basecase sqr size is 1, then no double sum prods
    jz      before_pass3    ; do the single diagonal sqr prod

    ; PASS-1 (O(n^2) step)

    xor     r12, r12    ; counter (starts at 0)
    xor     r13, r13    ; to restore rax after it has been clobbered by mul
    
outer_loop_pass1:

    ; rdx:rax for accumulating the product via mul
    ; r10 <- result (copy for loop)
    ; r11 <- op1    (copy for loop)
    ; r9 <- (size - 1) (copy for loop)

    mov     rcx,  r9    ; rcx is inner loop counter
    xor     rbx, rbx    ; temp_reg
    xor     rdx, rdx    ; high64 = 0
    mov     r13, QWORD PTR [rsi + r12*8]
    lea     r10, [rdi + r12*8 + 8]      ; result[counter + 1]
    lea     r11, [rsi + r12*8 + 8]      ; op1[counter + 1]

ALIGN 16
inner_loop_pass1:

    mov     rax, r13            ; restore clobbered rax or load for the first time
    adc     rbx, rdx            ; temp_reg += (CF + high64)
    mul     QWORD PTR [r11]     ; rdx:rax = rax * op1[counter + 1]

    add     rbx, rax                ; temp_reg += low64
    adc     rdx, 0                  ; high64 += CF
    add     QWORD PTR [r10], rbx    ; result[counter + 1] += temp_reg

    mov     rbx, 0
    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    loop    inner_loop_pass1

outer_loop_end_pass1:

    inc     r12
    adc     QWORD PTR [r10], rdx
    dec     r9
    jnz     outer_loop_pass1

    ; PASS-2 (O(n) step)

before_pass2:

    lea     r10, [rdi + 8]
    mov     rcx, r8
    sub     rcx, 1
    
    ; 2 * (n - 1) limbs in result need to be shifted left by 1
    ; the first and last limb don't contain any values
    ; limbs (inclusive) of result[1:2n-2]
    ; Process two limbs at once for each decrement in rcx

    xor     r11, r11    ; zeroes out the carry flag
                        ; before the rcl
loop_pass2:

    rcl     QWORD PTR [r10], 1
    rcl     QWORD PTR [r10 + 8], 1
    
    lea     r10, [r10 + 16]
    loop    loop_pass2

    ; PASS-3 (O(n) step)

before_pass3:

    mov     r10, rdi
    mov     r11, rsi
    mov     rcx, r8

    xor     r12, r12            ; to clear CF/OF

loop_pass3:

    adc     r12, 0                  ; accumulate CF from last iter as mul clobbers both 
                                    ; OF & CF, does nothing in first iter
    mov     rax, QWORD PTR [r11]
    mul     rax                     ; rdx:rax = rax * rax (rax contains op1[i])
    add     r12, rax                ; mul can set both OF/CF, to take care of that
                                    ; add rax to an empty r12                 
    add     QWORD PTR [r10], r12
    adc     QWORD PTR [r10 + 8], rdx    
    mov     r12, 0

    lea     r11, [r11 + 8]
    lea     r10, [r10 + 16]
    loop    loop_pass3
    
end_of_func:

    pop     r13
    pop     r12
    pop     rdi
    pop     rsi
    pop     rbx
    ret

sqr_bc_x64 ENDP

END