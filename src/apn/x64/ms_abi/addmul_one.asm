
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |		         APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS   			|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (apn_seg*)   
    ;   rdx -> op1 (const apn_seg*)
    ;   r8  -> size (apn_size)
    ;   r9  -> val (apn_seg)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

addmul_one_zen4 PROC FRAME

    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
    push    r12
.pushreg    r12
.endprolog

    xor     rax, rax
    xor     rbx, rbx

    ; will need these zero'd out regs later

    xchg    rcx, r10    ; free up rcx for loop/jrcxz
    xchg    rdx, r11    ; free up rdx for mulx

    ; r10 <- result
    ; r11 <- op1

    xor     r12, r12    ; temporary store register
    mov     rcx, r8     ; copy of size in r10
    shr     rcx, 3      ; size /= 8    
    and     r8,  7      ; size %= 8
    mov     rdx, r9     ; load val into rdx for mulx

    test    rcx, rcx
    jz      before_remainder

ALIGN 16
unrolled_loop:

    mulx    rdi, rsi, QWORD PTR [r11]
    adcx    rsi, QWORD PTR [r10]
    adox    rdi, QWORD PTR [r10 + 8]
    mov     QWORD PTR [r10], rsi
    mov     r12, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 8]
    adcx    rsi, r12
    adox    rdi, QWORD PTR [r10 + 16]
    mov     QWORD PTR [r10 + 8], rsi
    mov     r12, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 16]
    adcx    rsi, r12
    adox    rdi, QWORD PTR [r10 + 24]
    mov     QWORD PTR [r10 + 16], rsi
    mov     r12, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 24]
    adcx    rsi, r12
    adox    rdi, QWORD PTR [r10 + 32]
    mov     QWORD PTR [r10 + 24], rsi
    mov     r12, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 32]
    adcx    rsi, r12
    adox    rdi, QWORD PTR [r10 + 40]
    mov     QWORD PTR [r10 + 32], rsi
    mov     r12, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 40]
    adcx    rsi, r12
    adox    rdi, QWORD PTR [r10 + 48]
    mov     QWORD PTR [r10 + 40], rsi
    mov     r12, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 48]
    adcx    rsi, r12
    adox    rdi, QWORD PTR [r10 + 56]
    mov     QWORD PTR [r10 + 48], rsi
    mov     r12, rdi

    mulx    rdi, rsi, QWORD PTR [r11 + 56]
    adcx    rsi, r12
    adox    rdi, QWORD PTR [r10 + 64]
    mov     QWORD PTR [r10 + 56], rsi
    mov     QWORD PTR [r10 + 64], rdi

    lea     r10, [r10 + 64]
    lea     r11, [r11 + 64]
    lea     rcx, [rcx - 1]

ALIGN 16
    jrcxz   before_remainder
    jmp     unrolled_loop

ALIGN 16
before_remainder:

    mov     rcx, r8
    jrcxz   end_of_func

remainder_loop:

    mulx    rdi, rsi, QWORD PTR [r11]
    adcx    rsi, QWORD PTR [r10]
    adox    rdi, QWORD PTR [r10 + 8]
    mov     QWORD PTR [r10], rsi
    mov     QWORD PTR [r10 + 8], rdi
    
    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    loop    remainder_loop

end_of_loop:

    adcx    rbx, QWORD PTR [r10]
    mov     QWORD PTR [r10], rbx
    adox    rax, rax    

end_of_func:

    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    ret
    
addmul_one_zen4 ENDP

;   -------------------------
;
;            MUL/ADC        
;
;   -------------------------

addmul_one_x64 PROC FRAME

    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
.endprolog

    xchg    rdi, rcx    ; free up rcx for jrcxz/loop
    xchg    rsi, rdx    ; free up rdx for mul

    ; rdi <- result
    ; rsi <- op1
    ; r8  <- size
    
    ; rbx is temp_reg

    xor     rdx, rdx
    xor     rbx, rbx
    mov     rcx, r8
    mov     r10, rdi
    mov     r11, rsi
    test    rcx, rcx
    jz      end_of_func

main_loop:
    
    mov     rax, r9         ; restore clobbered rax
    adc     rbx, rdx        ; temp_reg += (CF + high64)
    mul     QWORD PTR [r11] ; rdx:rax = rax * op1[idx]

    add     rbx, rax
    adc     rdx, 0
    add     QWORD PTR [r10], rbx

    mov     rbx, 0

    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    loop    main_loop

end_of_loop:

    adc     QWORD PTR [r10], rdx
    setc    al
    movzx   rax, al

end_of_func:

    pop     rsi
    pop     rdi
    pop     rbx
    ret   

addmul_one_x64 ENDP

END