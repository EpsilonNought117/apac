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
    shr     r10, 1

loop_outer:

    ; move size1 into r10 and rcx
    ; r10 = size1 / 2
    ; rcx = size1 % 2 (initially)
    ;   
    ; After the remainder loop
    ;   
    ; rcx = r10 

    xor     r11, r11    ; j
    mov     r12, rax    ; indexer for result
    mov     rdx, QWORD PTR [r8 + rax*8] ; op2[i]
    mov     rcx, r9     
    and     rcx, 1      ; rcx %= 2
    jrcxz   before_unroll

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

before_unroll:

    mov     rcx, r10

loop_inner_unroll:

    jrcxz   loop_end
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

    lea     r11, [r11 + 2]
    lea     r12, [r12 + 2]
    lea     rcx, [rcx - 1]
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