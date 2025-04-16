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

_adx_mulx_mul_basecase PROC FRAME

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
.pushframe
.endprolog

    xor     rax, rax    ; i
    xchg    rbx, rdx    ; now rbx contains op1 (const u64*)
    xchg    rcx, r15    ; now r15 contains result (u64*)
    mov     r10, r9     
    shr     r10, 2

loop_outer:

    ; move size1 into r10 and rcx
    ; r10 = size1 / 4
    ; rcx = size1 % 4 (initially)
    ;   
    ; After the remainder loop
    ;   
    ; rcx = r10 

    mov     r12, rax    ; indexer for result
    mov     rcx, r9     
    mov     rdx, QWORD PTR [r8 + rax*8] ; op2[i]
    and     rcx, 3      ; rcx %= 4
    xor     rsi, rsi    ; low64
    xor     rdi, rdi    ; high64
    xor     r11, r11    ; j

loop_inner_rmdr:
    
    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]

    mov     r14, QWORD PTR [r15 + r12*8]
    adcx    r14, rsi
    mov     QWORD PTR [r15 + r12*8], r14
    
    mov     r14, QWORD PTR [r15 + r12*8 + 8]
    adox    r14, rdi
    mov     QWORD PTR [r15 + r12*8 + 8], r14

    lea     r11, [r11 + 1]
    lea     r12, [r12 + 1]
    lea     rcx, [rcx - 1]
    
    jrcxz   before_unroll  
    jmp     loop_inner_rmdr

before_unroll:

    mov     rcx, r10

loop_inner_unroll:

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]

    mov     r14, QWORD PTR [r15 + r12*8]
    adcx    r14, rsi
    mov     QWORD PTR [r15 + r12*8], r14

    mov     r14, QWORD PTR [r15 + r12*8 + 8]
    adox    r14, rdi
    mov     QWORD PTR [r15 + r12*8 + 8], r14

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 8]

    mov     r14, QWORD PTR [r15 + r12*8 + 8]
    adcx    r14, rsi
    mov     QWORD PTR [r15 + r12*8 + 8], r14

    mov     r14, QWORD PTR [r15 + r12*8 + 16]
    adox    r14, rdi
    mov     QWORD PTR [r15 + r12*8 + 16], r14

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 16]

    mov     r14, QWORD PTR [r15 + r12*8 + 16]
    adcx    r14, rsi
    mov     QWORD PTR [r15 + r12*8 + 16], r14

    mov     r14, QWORD PTR [r15 + r12*8 + 24]
    adox    r14, rdi
    mov     QWORD PTR [r15 + r12*8 + 24], r14
    
    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 24]

    mov     r14, QWORD PTR [r15 + r12*8 + 24]
    adcx    r14, rsi
    mov     QWORD PTR [r15 + r12*8 + 24], r14

    mov     r14, QWORD PTR [r15 + r12*8 + 32]
    adox    r14, rdi
    mov     QWORD PTR [r15 + r12*8 + 32], r14

    lea     r11, [r11 + 4]
    lea     r12, [r12 + 4]
    lea     rcx, [rcx - 1]
    jrcxz   loop_end
    jmp     loop_inner_unroll

loop_end:

    mov     r14, QWORD PTR [rsp + 96] ; size2 in r14 for cmp later
    mov     r11, 0
    adcx    r11, QWORD PTR [r15 + r12*8]
    mov     QWORD PTR [r15 + r12*8], r11
      
    inc     rax
    cmp     r14, rax
    jnz     loop_outer
    
end_of_func:

    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    ret     0

_adx_mulx_mul_basecase ENDP

END