.code 
	
	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> op2 (const u64*)
    ;   r9  -> size1 (u64)
    ;   [rsp + 40] -> size2 (u64)

    ; ASSUMES ALL ZERO'D LIMBS IN RESULT

_apn_mul_basecase PROC FRAME
.pushframe

    push    rbx
.pushreg    rbx

    push    rbp
.pushreg    rbp
    
    push    rsi
.pushreg    rsi
    
    push    rdi
.pushreg    rdi

    push    r12
.pushreg    r12
    
    push    r13
.pushreg    r13
.endprolog

    xor     rax, rax    ; i
    xor     rbx, rbx    
    xor     rbp, rbp

    xchg    rbx, rdx    ; rbx <- op1
    xchg    rbp, rcx    ; rbp <- result
   
loop_outer:
    
    xor     rsi, rsi    ; low64
    xor     rdi, rdi    ; high64

    ; r10 = result[i + j] (for adcx)
    ; r12 = result[i + j + 1] (for adox)
    ; no need to zero them out as they get over-written by mov's

    mov     r13, rax    ; indexer for result

    xor     r11, r11    ; j
    mov     rcx, QWORD PTR [rsp + 88]       ; size2 (for jrcxz)
    mov     rdx, QWORD PTR [rbx + rax*8]    ; op1[i]

    test    rcx, 1
    jz      loop_inner_twice

one_time_iter:

    mulx    rdi, rsi, QWORD PTR [r8 + r11*8]

    mov     r10, QWORD PTR [rbp + r13*8]
    adcx    r10, rsi
    mov     QWORD PTR [rbp + r13*8], r10

    mov     r12, QWORD PTR [rbp + r13*8 + 8]
    adox    r12, rdi
    mov     QWORD PTR [rbp + r13*8 + 8], r12
    
    lea     r11, [r11 + 1]
    lea     r13, [r13 + 1]
    lea     rcx, [rcx - 1]

loop_inner_twice:

    mulx    rdi, rsi, QWORD PTR [r8 + r11*8]

    mov     r10, QWORD PTR [rbp + r13*8]
    adcx    r10, rsi
    mov     QWORD PTR [rbp + r13*8], r10

    mov     r12, QWORD PTR [rbp + r13*8 + 8]
    adox    r12, rdi
    mov     QWORD PTR [rbp + r13*8 + 8], r12

    mulx    rdi, rsi, QWORD PTR [r8 + r11*8 + 8]

    mov     r10, QWORD PTR [rbp + r13*8 + 8]
    adcx    r10, rsi
    mov     QWORD PTR [rbp + r13*8 + 8], r10

    mov     r12, QWORD PTR [rbp + r13*8 + 16]
    adox    r12, rdi
    mov     QWORD PTR [rbp + r13*8 + 16], r12

    lea     r11, [r11 + 2]
    lea     r13, [r13 + 2]
    lea     rcx, [rcx - 2]
    jrcxz   loop_end
    jmp     loop_inner_twice

loop_end:
    
    mov     r12, 0
    mov     r10, QWORD PTR [rbp + r13*8]
    adcx    r10, r12
    mov     QWORD PTR [rbp + r13*8], r10

    inc     rax
    cmp     r9, rax
    jnz     loop_outer

    pop     r13
    pop     r12
    pop     rdi
    pop     rsi
    pop     rbp
    pop     rbx
    ret     0
_apn_mul_basecase ENDP

END