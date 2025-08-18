
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |		      APN-ARR BY ONE LIMB MULTIPLICATION-&-ADD FUNCTIONS			|
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

    xchg    rcx, rax    ; free up rcx for loop/jrcxz
    xchg    rdx, rbx    ; free up rdx for mulx
    xor     r12, r12
    mov     rcx, r8     ; copy of size in r10
    shr     rcx, 3      ; size /= 8    
    and     r8,  7      ; size %= 8
  
    test    rcx, rcx
    jz      before_remainder
    
    mov     rdx, r9     ; load val into rdx for mulx
    ; rax <- result
    ; rbx <- op1
    mov     r10, rax
    mov     r11, rbx
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
    jrcxz   before_remainder
    jmp     unrolled_loop

ALIGN 16
before_remainder:

    mov     rcx, r8
    jrcxz   end_of_func

ALIGN 16
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

    mov     r12, 0
    adcx    r12, QWORD PTR [r10]
    mov     QWORD PTR [r10], r12
    mov     rsi, 0
    mov     r12, 0
    adcx    r12, rsi
    adox    r12, rsi

end_of_func:

    mov     rax, r12

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
    mov     r12, r9     ; val in r12 for restoring rax after mul clobbers it

    ; rdi <- result
    ; rsi <- op1
    ; r8  <- size
    
    ; rbx is temp_reg
    
    xor     rbx, rbx
    mov     rcx, r8
    mov     rax, r9
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

end_of_func:

    setc    al
    movzx   rax, al

    pop     rsi
    pop     rdi
    pop     rbx
    ret   

addmul_one_x64 ENDP