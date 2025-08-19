
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |		      APN-ARR BY ONE LIMB MULTIPLICATION-&-SUB FUNCTIONS			|
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
;           MULX/SBB                 
;
;   -------------------------

submul_one_zen4 PROC FRAME

    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
.endprolog

    xchg    rcx, rax
    xchg    rdx, rbx    
    mov     rcx, r8
    shr     rcx, 2
    and     r8,  3

    mov     rdx, r9
    mov     r10, rax
    mov     r11, rbx
    test    rcx, rcx
    jz      before_remainder

unrolled_loop:

    mov     rax, QWORD PTR [r10 + 8]
    mulx    rdi, rsi, QWORD PTR [r11]
    sbb     QWORD PTR [r10], rsi
    sbb     rax, rdi
    mov     rbx, rax

    mov     rax, QWORD PTR [r10 + 16]
    mulx    rdi, rsi, QWORD PTR [r11 + 8]
    sbb     rbx, rsi
    sbb     rax, rdi
    mov     QWORD PTR [r10 + 8], rbx
    mov     rbx, rax

    mov     rax, QWORD PTR [r10 + 24]
    mulx    rdi, rsi, QWORD PTR [r11 + 16]
    sbb     rbx, rsi
    sbb     rax, rdi
    mov     QWORD PTR [r10 + 16], rbx
    mov     rbx, rax

    mov     rax, QWORD PTR [r10 + 32]
    mulx    rdi, rsi, QWORD PTR [r11 + 24]
    sbb     rbx, rsi
    sbb     rax, rdi
    mov     QWORD PTR [r10 + 24], rbx
    mov     QWORD PTR [r10 + 32], rax

    lea     r10, [r10 + 32]
    lea     r11, [r11 + 32]
    lea     rcx, [rcx - 1]
    jrcxz   before_remainder
    jmp     unrolled_loop

before_remainder:

    mov     rcx, r8
    jrcxz   end_of_func

remainder_loop:

    mulx    rdi, rsi, QWORD PTR [r11]
    sbb     QWORD PTR [r10], rsi
    sbb     QWORD PTR [r10 + 8], rdi

    lea     r10, [r10 + 32]
    lea     r11, [r11 + 32]
    loop    remainder_loop

end_of_func:

    setc    al
    movzx   rax, al

    pop     rsi
    pop     rdi
    pop     rbx
    ret

submul_one_zen4 ENDP

;   -------------------------
;
;         MUL/SBB/ADC                 
;
;   -------------------------

submul_one_x64 PROC FRAME

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

    mov     rax, r9
    adc     rbx, rdx
    mul     QWORD PTR [r11]

    add     rbx, rax
    adc     rdx, 0
    sub     QWORD PTR [r10], rbx

    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    loop    main_loop

end_of_loop:

    sbb     QWORD PTR [r10], rdx

end_of_func:

    setc    al
    movzx   rax, al

    pop     rsi
    pop     rdi
    pop     rbx
    ret   

submul_one_x64 ENDP