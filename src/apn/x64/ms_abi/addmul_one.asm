
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |		         APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS   			|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

ADDMUL_ONE SEGMENT ALIGN(64) 'CODE'
	
    option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result       (apn_seg_t*)   
    ;   rdx -> op1          (const apn_seg_t*)
    ;   r8  -> size         (apn_size_t)
    ;   r9  -> val          (apn_seg_t)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

addmul_one_zen4 PROC FRAME
    
    push    rbx
.pushreg    rbx
    push    rbp
.pushreg    rbp
.endprolog

    xchg    rbp, rcx
    xchg    rbx, rdx
    mov     rcx, r8
    and     r8,  7
    shr     rcx, 3

    mov     rax, QWORD PTR [rbp]
    mov     rdx, r9
    test    rcx, rcx
    jz      before_remainder
    
ALIGN 64
unroll8_loop:

i = 0
WHILE i LT 8

    mulx    r11, r10, QWORD PTR [rbx + i*8]
    adcx    r10, rax
    adox    r11, QWORD PTR [rbp + i*8 + 8]
    mov     QWORD PTR [rbp + i*8], r10
    mov     rax, r11
        
    i = i + 1
ENDM

    lea     rbx, [rbx + 64]
    lea     rbp, [rbp + 64]
    lea     rcx, [rcx - 1]
    jrcxz   before_remainder
ALIGN 64
    jmp     unroll8_loop

ALIGN 32
before_remainder:

    mov     rcx, r8
    mov     r8,  0
    jrcxz   end_of_loop

ALIGN 64
remainder_loop:

    mulx    r11, r10, QWORD PTR [rbx]
    adcx    r10, rax
    adox    r11, QWORD PTR [rbp + 8]
    mov     QWORD PTR [rbp], r10
    mov     rax, r11

    lea     rbx, [rbx + 8]
    lea     rbp, [rbp + 8]
    loop    remainder_loop

end_of_loop:

    adcx    rax, r8
    mov     QWORD PTR [rbp], rax
    seto    al
    movzx   rax, al

end_of_func:

    pop     rbp
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