
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |		         APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS   			|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

SUBMUL_ONE SEGMENT ALIGN(64) 'CODE'
	
    option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result       (apn_seg_t*)   
    ;   rdx -> op1          (const apn_seg_t*)
    ;   r8  -> size         (apn_size_t)
    ;   r9  -> val          (apn_seg_t)

;   -------------------------
;
;          MULX/SBB        
;
;   -------------------------

submul_one_zen4 PROC FRAME

    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
.endprolog

start_of_func:

    xor     r9,  r9
    xchg    r10, rcx    ; r10 <- result_ptr
    xchg    r11, rdx    ; r11 <- op1_ptr
    mov     rcx, r8     ; rcx <- size
    mov     rdx, r9     ; rdx <- val
    and     r8,  7      ; size %= 8
    shr     rcx, 3      ; size /= 8
    xor     r9,  r9     ; temp_reg = 0
    test    rcx, rcx    
    jz      before_remainder

ALIGN 64
unrolled_loop:

i = 0
WHILE i LT 8

    mulx    rdi, rsi, QWORD PTR [r11 + i * 8]
    add     r9,  rsi
    adc     rdi, 0
    sub     QWORD PTR [r10 + i * 8], r9
    mov     r9,  rdi

i = i + 1
ENDM
     
     lea    r11, [r11 + 64]
     lea    r10, [r10 + 64]
     dec    rcx
     jnz    unrolled_loop

before_remainder:

    mov     rcx, r8
    jrcxz   end_of_loop

remainder_loop:

    mulx    rdi, rsi, QWORD PTR [r11]
    add     r9,  rsi
    adc     rdi, 0
    sub     QWORD PTR [r10], r9
    mov     r9,  rdi

    lea     r11, [r11 + 8]
    lea     r10, [r10 + 8]
    dec     rcx
    jnz     remainder_loop

end_of_loop:

    sbb     QWORD PTR [rbp], r9

end_of_func:

    setc    al
    movzx   rax, al
    
    pop     rsi
    pop     rdi
    ret

submul_one_zen4 ENDP

;   -------------------------
;
;           MUL/SBB        
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

start_of_func:

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
    sbb     rbx, 0          ; temp_reg -= borrow
    mul     QWORD PTR [r11] ; rdx:rax = rax * op1[idx]
    
    ; mul clobbers the original Carry Flag value

    add     rbx, rax                ; temp_reg += low64
    adc     rdx, 0                  ; high64 += CF
    sub     QWORD PTR [r10], rbx    ; result[i] -= temp_reg
    
    mov     rbx, rdx        ; temp_reg = high64

    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    loop    main_loop

end_of_loop:

    sbb     QWORD PTR [rbp], rbx

end_of_func:

    setc    al
    movzx   rax, al

    pop     rsi
    pop     rdi
    pop     rbx
    ret

submul_one_x64 ENDP

END