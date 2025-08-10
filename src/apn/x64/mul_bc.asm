
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                      BASECASE MULTIPLICATION FUNCTIONS                    |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> op2 (const u64*)
    ;   r9  -> size1 (u64)
    ;   [rsp + 40] -> size2 (u64) (where rsp is initially at function entering)

    ;   ASSUMPTION
    ;   (size1 >= size2)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

; The fastest procedures utilizing ADX and BMI2 instruction set extensions

; Basecase multiplication procedure optimized for 
; AMD Zen4 Microarchitecture & unrolled 8x

mul_bc_zen4 PROC FRAME

    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
    push    r12
.pushreg    r12
    push    r13
.pushreg    r13
    push    r14
.pushreg    r14
    push    r15
.pushreg    r15
    push    rbp
.pushreg    rbp
.pushframe
.endprolog

    xor     rax, rax        ; i
    xchg    rbp, rcx        ; result in rbp
    xchg    rbx, rdx        ; op1 in rbx
    mov     r10, QWORD PTR [rsp + 104] ; size2 in r10
    mov     r13, rbp        ; store a copy of result in r13
    mov     r11, rbx        ; store a copy of op1 in r11
    mov     r12, r9
    shr     r9,  3          ; size1 /= 8
    and     r12, 7          ; size1 %= 8
    xor     r15, r15

loop_outer:

    mov     rcx, r9
    mov     rdx, QWORD PTR [r8 + rax*8] ; load op2[i] into rdx
    lea     rbp, [r13 + rax*8]           
    lea     rbx, [r11]

    test    rcx, rcx
    jz      before_remainder

ALIGN 16
inner_unrolled:

    mulx    rdi, rsi, QWORD PTR [rbx]
    adcx    rsi, QWORD PTR [rbp]
    adox    rdi, QWORD PTR [rbp + 8]
    mov     QWORD PTR [rbp], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 8]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 16]
    mov     QWORD PTR [rbp + 8], rsi
    mov     r14, rdi
    
    mulx    rdi, rsi, QWORD PTR [rbx + 16]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 24]
    mov     QWORD PTR [rbp + 16], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 24]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 32]
    mov     QWORD PTR [rbp + 24], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 32]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 40]
    mov     QWORD PTR [rbp + 32], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 40]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 48]
    mov     QWORD PTR [rbp + 40], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 48]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 56]
    mov     QWORD PTR [rbp + 48], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 56]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 64]
    mov     QWORD PTR [rbp + 56], rsi
    mov     QWORD PTR [rbp + 64], rdi

    lea     rbx, [rbx + 64]
    lea     rbp, [rbp + 64]
    lea     rcx, [rcx - 1]
    jrcxz   before_remainder
    jmp     inner_unrolled

ALIGN 16
before_remainder:

    mov     rcx, r12
    jrcxz   outer_loop_end

ALIGN 16
remainder:

    mulx    rdi, rsi, QWORD PTR [rbx]
    adcx    rsi, QWORD PTR [rbp]
    adox    rdi, QWORD PTR [rbp + 8]
    mov     QWORD PTR [rbp], rsi
    mov     QWORD PTR [rbp + 8], rdi

    lea     rbx, [rbx + 8]
    lea     rbp, [rbp + 8]
    loop    remainder

outer_loop_end:

    adc     QWORD PTR [rbp], r15

    inc     rax
    cmp     r10, rax
    jnz     loop_outer

end_of_func:

    pop     rbp
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    ret     

mul_bc_zen4 ENDP

;   -------------------------
;
;            MUL/ADC        
;
;   -------------------------

; Lowest common denominator x64 multiplication routine
; Not particularly optimized 

mul_bc_x64 PROC FRAME
    
    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
    push    r12
.pushreg    r12
    push    r13
.pushreg    r13
.pushframe
.endprolog

    xchg    rbx, rdx    ; high64 in rdx
    xor     r10, r10    ; i

    ; now rbx contains op1 (const u64*)

outer_loop:

    xor     rdi, rdi        ; temp_reg
    xor     r11, r11        ; j
    xor     rdx, rdx        ; high64
    mov     r12, r10        ; r12 is indexer for result
    mov     r13, r9         ; load size1 into r13
    mov     rsi, QWORD PTR [r8 + r10*8]  ; op2[i]
    mov     rax, rsi

inner_loop:

    adc     rdi, rdx                 ; temp_reg += (CF + high64)
    mul     QWORD PTR [rbx + r11*8]  ; rdx:rax = rax * op1[j]

    add     rdi, rax                 ; temp_reg += low64
    adc     rdx, 0                   ; high64 += CF
    add     QWORD PTR [rcx + r12*8], rdi    ; result[i + j] += temp_reg

    mov     rax, rsi        ; load op2[i] once again into rax for next iter
    mov     rdi, 0          ; zero out temp_reg
    inc     r11
    inc     r12
    dec     r13
    jnz     inner_loop

loop_end:

    adc     QWORD PTR [rcx + r12*8], rdx
    inc     r10
    mov     rsi, QWORD PTR [rsp + 80]   ; load size2 into rsi
    cmp     rsi, r10        ; check if (rsi - r10) != 0 (means rsi > r10 for unsigned)
    jnz     outer_loop

end_of_func:

    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    ret     0

mul_bc_x64 ENDP

END