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
    push    rbp
.pushreg    rbp
.pushframe
.endprolog

    xor     rax, rax    ; i
    xchg    rbp, rcx    ; result in rbp
    xchg    rbx, rdx    ; op1 in rbx
    mov     r10, QWORD PTR [rsp + 104] ; size2 in r10
    mov     r12, r9
    shr     r9, 2       ; size1 /= 4
    and     r12, 3      ; size1 %= 4

loop_outer:

    xor     r11, r11    ; j
    mov     rcx, r9     ; size1/4 in rcx
    mov     r13, rax    ; indexer for result
    mov     rdx, QWORD PTR [r8 + rax*8]     ; op2[i]

    xor     rsi, rsi    ; clear flags from previous iterations

    mov     rdi, jump_table
    jmp     QWORD PTR [rdi + r12*8]

rem1:

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]
    mov     r14, QWORD PTR [rbp + r13*8]
    mov     r15, QWORD PTR [rbp + r13*8 + 8]

    adcx    r14, rsi
    adox    r15, rdi

    mov     QWORD PTR [rbp + r13*8], r14
    mov     QWORD PTR [rbp + r13*8 + 8], r15

    lea     r11, [r11 + 1]
    lea     r13, [r13 + 1]
    jmp     loop_unrolled_4

rem2:

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]
    mov     r14, QWORD PTR [rbp + r13*8]
    mov     r15, QWORD PTR [rbp + r13*8 + 8]

    adcx    r14, rsi
    adox    r15, rdi

    mov     QWORD PTR [rbp + r13*8], r14

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 8]
    mov     r14, QWORD PTR [rbp + r13*8 + 16]

    adcx    r15, rsi
    adox    r14, rdi

    mov     QWORD PTR [rbp + r13*8 + 8], r15
    mov     QWORD PTR [rbp + r13*8 + 16], r14

    lea     r11, [r11 + 2]
    lea     r13, [r13 + 2]
    jmp     loop_unrolled_4

rem3:

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]
    mov     r14, QWORD PTR [rbp + r13*8]
    mov     r15, QWORD PTR [rbp + r13*8 + 8]

    adcx    r14, rsi
    adox    r15, rdi

    mov     QWORD PTR [rbp + r13*8], r14

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 8]
    mov     r14, QWORD PTR [rbp + r13*8 + 16]

    adcx    r15, rsi
    adox    r14, rdi

    mov     QWORD PTR [rbp + r13*8 + 8], r15

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 16]
    mov     r15, QWORD PTR [rbp + r13*8 + 24]

    adcx    r14, rsi
    adox    r15, rdi

    mov     QWORD PTR [rbp + r13*8 + 16], r14
    mov     QWORD PTR [rbp + r13*8 + 24], r15

    lea     r11, [r11 + 3]
    lea     r13, [r13 + 3]
    jmp     loop_unrolled_4

loop_end:

    mov     r14, 0
    adcx    r14, QWORD PTR [rbp + r13*8]
    mov     QWORD PTR [rbp + r13*8], r14

    inc     rax
    cmp     r10, rax
    jz      end_of_func
    jmp     loop_outer
 
ALIGN 16
loop_unrolled_4:

    jrcxz   loop_end

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]
    mov     r14, QWORD PTR [rbp + r13*8]
    mov     r15, QWORD PTR [rbp + r13*8 + 8]

    adcx    r14, rsi
    adox    r15, rdi

    mov     QWORD PTR [rbp + r13*8], r14

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 8]
    mov     r14, QWORD PTR [rbp + r13*8 + 16]

    adcx    r15, rsi
    adox    r14, rdi

    mov     QWORD PTR [rbp + r13*8 + 8], r15

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 16]
    mov     r15, QWORD PTR [rbp + r13*8 + 24]

    adcx    r14, rsi
    adox    r15, rdi

    mov     QWORD PTR [rbp + r13*8 + 16], r14

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 24]
    mov     r14, QWORD PTR [rbp + r13*8 + 32]

    adcx    r15, rsi
    adox    r14, rdi

    mov     QWORD PTR [rbp + r13*8 + 24], r15
    mov     QWORD PTR [rbp + r13*8 + 32], r14

    lea     r11, [r11 + 4]
    lea     r13, [r13 + 4]
    lea     rcx, [rcx - 1]
    jmp     loop_unrolled_4

jump_table:
            dq offset loop_unrolled_4
            dq offset rem1
            dq offset rem2
            dq offset rem3
    
end_of_func:

    pop     rbp
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