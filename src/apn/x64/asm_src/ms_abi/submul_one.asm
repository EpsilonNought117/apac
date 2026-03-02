
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |		         APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS   			|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

SUBMUL_ONE SEGMENT ALIGN(64) 'CODE'
	
    option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result       (ap_dig_t*)   
    ;   rdx -> op1          (const ap_dig_t*)
    ;   r8  -> size         (ap_size_t)
    ;   r9  -> val          (ap_dig_t)

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

    xchg    r10, rcx    ; r10 <- result_ptr
    xchg    r11, rdx    ; r11 <- op1_ptr
    mov     rcx, r8     ; rcx <- size
    mov     rdx, r9     ; rdx <- val
    and     r8,  3      ; size %= 4
    shr     rcx, 2      ; size /= 4
    xor     r9,  r9     ; temp_reg1 = 0
    test    rcx, rcx
    stc                 ; set carry flag
    jz      before_remainder

ALIGN 64
unrolled_loop:

i = 0
WHILE i LT 4

    mulx    rdi, rsi, QWORD PTR [r11 + i * 8]
    adox    rsi, r9
    not     rsi
    adcx    rsi, QWORD PTR [r10 + i * 8]
    mov     QWORD PTR [r10 + i * 8], rsi
    mov     r9,  rdi

i = i + 1
ENDM
     
    lea     r11, [r11 + 32]
    lea     r10, [r10 + 32]
    lea     rcx, [rcx - 1]
    jrcxz   before_remainder
    jmp     unrolled_loop

ALIGN 16
before_remainder:

    mov     rcx, r8
    jrcxz   end_of_loop

remainder_loop:

    mulx    rdi, rsi, QWORD PTR [r11]
    adox    rsi, r9
    not     rsi
    adcx    rsi, QWORD PTR [r10]
    mov     QWORD PTR [r10], rsi
    mov     r9,  rdi

    lea     r11, [r11 + 8]
    lea     r10, [r10 + 8]
    loop    remainder_loop

end_of_loop:

    adox    r9,  rcx
    not     r9
    adcx    r9,  QWORD PTR [r10]
    mov     QWORD PTR [r10], r9

end_of_func:

    setnc   al
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
.endprolog

start_of_func:

    xchg    r10, rcx    ; free up rcx for loop counter
    xchg    r11, rdx    ; free up rdx for mul
    
    ; r10 <- result
    ; r11 <- op1
    ; r8  <- size
    
    ; rbx is temp_reg
    
    xor     rdx, rdx
    xor     rcx, rcx
    mov     rax, r9
    test    r8,  r8
    jz      end_of_func

main_loop:

    mul     QWORD PTR [r11] ; rdx:rax = rax * op1[idx]
    
    ; mul clobbers the original Carry Flag value

    add     rcx, rax                ; temp_reg += low64
    adc     rdx, 0                  ; high64 += CF
    sub     QWORD PTR [r10], rcx    ; result[i] -= temp_reg
    
    mov     rcx, rdx        ; temp_reg = high64
    mov     rax, r9         ; restore clobbered rax
    adc     rcx, 0          ; temp_reg += borrow

    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    dec     r8
    jnz     main_loop

end_of_loop:

    sbb     QWORD PTR [r10], rcx

end_of_func:

    setc    al
    movzx   rax, al

    ret

submul_one_x64 ENDP

END