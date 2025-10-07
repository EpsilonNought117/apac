
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                      BASECASE MULTIPLICATION FUNCTIONS                    |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (apn_seg*)   
    ;   rdx -> op1 (const apn_seg*)
    ;   r8  -> op2 (const apn_seg*)
    ;   r9  -> size1 (apn_size)
    ;   [rsp + 40] -> size2 (apn_size) (where rsp is initially at function entering)

    ;   ASSUMPTION
    ;   (size1 >= size2)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

; The fastest procedures utilizing ADX and BMI2 instruction set extensions

; Basecase multiplication procedure optimized for 
; AMD Zen4 Microarchitecture & unrolled 4x

mul_bc_zen4 PROC FRAME

    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
    push    r12
.pushreg    r12
    push    rbp
.pushreg    rbp
.endprolog

    xchg    rbp, rcx        ; result in rbp
    xchg    rbx, rdx        ; op1 in rbx
    mov     r10, QWORD PTR [rsp + 80] ; size2 in r10
    mov     rax, r9
    shl     rax, 3
    mov     r12, r9
    shr     r9,  2          ; size1 /= 4
    and     r12, 3          ; size1 %= 4

loop_outer:

    mov     rcx, r9
    mov     rdx, QWORD PTR [r8] ; load op2[i] into rdx
    test    rcx, rcx
    jz      before_remainder
    mov     r11, QWORD PTR [rbp]

ALIGN 16
inner_unrolled:

    mulx    rdi, rsi, QWORD PTR [rbx]
    adcx    rsi, r11
    adox    rdi, QWORD PTR [rbp + 8]
    mov     QWORD PTR [rbp], rsi
    mov     r11, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 8]
    adcx    rsi, r11
    adox    rdi, QWORD PTR [rbp + 16]
    mov     QWORD PTR [rbp + 8], rsi
    mov     r11, rdi
    
    mulx    rdi, rsi, QWORD PTR [rbx + 16]
    adcx    rsi, r11
    adox    rdi, QWORD PTR [rbp + 24]
    mov     QWORD PTR [rbp + 16], rsi
    mov     r11, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 24]
    adcx    rsi, r11
    adox    rdi, QWORD PTR [rbp + 32]
    mov     QWORD PTR [rbp + 24], rsi
    mov     r11, rdi

    lea     rbx, [rbx + 32]
    lea     rbp, [rbp + 32]
    loop    inner_unrolled
    mov     QWORD PTR [rbp], r11

ALIGN 16
before_remainder:

    mov     rcx, r12
    jrcxz   outer_loop_end
    mov     r11, QWORD PTR [rbp]

remainder:

    mulx    rdi, rsi, QWORD PTR [rbx]
    adcx    rsi, r11
    adox    rdi, QWORD PTR [rbp + 8]
    mov     QWORD PTR [rbp], rsi
    mov     r11, rdi

    lea     rbx, [rbx + 8]
    lea     rbp, [rbp + 8]
    loop    remainder
    mov     QWORD PTR [rbp], r11

outer_loop_end:
    
    adc     QWORD PTR [rbp], 0
    
    add     r8, 8
    sub     rbx, rax
    sub     rbp, rax
    add     rbp, 8
    dec     r10
    jnz     loop_outer

end_of_func:

    pop     rbp
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