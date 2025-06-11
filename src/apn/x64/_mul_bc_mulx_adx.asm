
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                 BASECASE MULTIPLICATION FUNC (ADX/MULX)                   |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

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

; 8x unroll version

_mul_bc_mulx_adx_8unroll PROC FRAME

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
    shr     r9,  3      ; size1 /= 8
    and     r12, 7      ; size1 %= 8
    mov     r13, rbp    ; store a copy of result in r11
    mov     r11, rbx    ; store a copy of op1 in r13

loop_outer:

    mov     rcx, r12
    mov     rdx, QWORD PTR [r8  + rax*8] ; load op2[i] into rdx for implicit muls
    lea     rbp, [r13 + rax*8]           
    lea     rbx, [r11]

inner_small:

    jrcxz   before_inner_unrolled

    mulx    rdi, rsi, QWORD PTR [rbx]

    adcx    rsi, QWORD PTR [rbp]
    adox    rdi, QWORD PTR [rbp + 8]

    mov     QWORD PTR [rbp], rsi
    mov     QWORD PTR [rbp + 8], rdi    

    lea     rbx, [rbx + 8]
    lea     rbp, [rbp + 8]
    lea     rcx, [rcx - 1]
    jmp     inner_small

before_inner_unrolled:

    mov     rcx, r9
    jmp     inner_unrolled

outer_loop_end:

    adcx    r15, QWORD PTR [rbp]
    mov     QWORD PTR [rbp], r15

    inc     rax
    cmp     r10, rax
    jz      end_of_func
    jmp     loop_outer

ALIGN 16
inner_unrolled:

    jrcxz   outer_loop_end

    mulx    rdi, rsi, QWORD PTR [rbx]
    adcx    rsi, QWORD PTR [rbp]
    adox    rdi, QWORD PTR [rbp + 8]
    mov     QWORD PTR [rbp], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 8]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 16]
    mov     QWORD PTR [rbp + 8], rsi
    mov     r14, rdi
    
    mulx    rdi, rsi, QWORD PTR [rbx + 16]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 24]
    mov     QWORD PTR [rbp + 16], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 24]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 32]
    mov     QWORD PTR [rbp + 24], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 32]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 40]
    mov     QWORD PTR [rbp + 32], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 40]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 48]
    mov     QWORD PTR [rbp + 40], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 48]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 56]
    mov     QWORD PTR [rbp + 48], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + 56]
    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + 64]
    mov     QWORD PTR [rbp + 56], rsi
    mov     QWORD PTR [rbp + 64], rdi

    lea     rbx, [rbx + 64]
    lea     rbp, [rbp + 64]
    lea     rcx, [rcx - 1]
    jmp     inner_unrolled

end_of_func:

    pop     rbp
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    ret     

_mul_bc_mulx_adx_8unroll ENDP

; 4x unroll version

_mul_bc_mulx_adx_4unroll PROC FRAME

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
    shr     r9,  2      ; size1 /= 4
    and     r12, 3      ; size1 %= 4
    mov     r13, rbp    ; store a copy of result in r11
    mov     r11, rbx    ; store a copy of op1 in r13
    mov     rcx, r9

loop_outer:

    mov     rdx, QWORD PTR [r8  + rax*8] ; load op2[i] into rdx for implicit muls
    lea     rbp, [r13 + rax*8]           
    lea     rbx, [r11]

    xor     r15, r15    ; clear any flags from previous iterations

    mov     rdi, jump_table
    jmp     QWORD PTR [rdi + r12*8]

rem1:

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]

    adcx    rsi, QWORD PTR [rbp + r13*8]
    adox    rdi, QWORD PTR [rbp + r13*8 + 8]

    mov     QWORD PTR [rbp + r13*8], rsi
    mov     QWORD PTR [rbp + r13*8 + 8], rdi

    lea     r11, [r11 + 1]
    lea     r13, [r13 + 1]
    jmp     loop_unrolled_4

rem2:

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]

    adcx    rsi, QWORD PTR [rbp + r13*8]
    adox    rdi, QWORD PTR [rbp + r13*8 + 8]

    mov     QWORD PTR [rbp + r13*8], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 8]

    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + r13*8 + 16]

    mov     QWORD PTR [rbp + r13*8 + 8], rsi
    mov     QWORD PTR [rbp + r13*8 + 16], rdi

    lea     r11, [r11 + 2]
    lea     r13, [r13 + 2]
    jmp     loop_unrolled_4

rem3:

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]
    
    adcx    rsi, QWORD PTR [rbp + r13*8]
    adox    rdi, QWORD PTR [rbp + r13*8 + 8]

    mov     QWORD PTR [rbp + r13*8], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 8]

    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + r13*8 + 16]

    mov     QWORD PTR [rbp + r13*8 + 8], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 16]

    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + r13*8 + 24]

    mov     QWORD PTR [rbp + r13*8 + 16], rsi
    mov     QWORD PTR [rbp + r13*8 + 24], rdi

    lea     r11, [r11 + 3]
    lea     r13, [r13 + 3]
    jmp     loop_unrolled_4

loop_end:

    adcx    r15, QWORD PTR [rbp + r13*8]
    mov     QWORD PTR [rbp + r13*8], r15

    inc     rax
    cmp     r10, rax
    jz      end_of_func
    jmp     loop_outer

ALIGN 16
loop_unrolled_4:

    jrcxz   loop_end

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]

    adcx    rsi, QWORD PTR [rbp + r13*8]
    adox    rdi, QWORD PTR [rbp + r13*8 + 8]

    mov     QWORD PTR [rbp + r13*8], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 8]

    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + r13*8 + 16]

    mov     QWORD PTR [rbp + r13*8 + 8], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 16]

    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + r13*8 + 24]

    mov     QWORD PTR [rbp + r13*8 + 16], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 24]

    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + r13*8 + 32]

    mov     QWORD PTR [rbp + r13*8 + 24], rsi
    mov     QWORD PTR [rbp + r13*8 + 32], rdi

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
    ret     

_mul_bc_mulx_adx_4unroll ENDP

; 2x unroll version

_mul_bc_mulx_adx_2unroll PROC FRAME

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
    shr     r9, 1       ; size1 /= 2
    and     r12, 1      ; size1 %= 2

loop_outer:

    xor     r11, r11    ; j
    mov     rcx, r9     ; (size1) / 2 -> rcx
    mov     r13, rax    ; indexer for result
    mov     rdx, QWORD PTR [r8 + rax*8]     ; op2[i]

    xor     r15, r15    ; clear any flags from previous iterations
    test    r12, r12
    jz      loop_unrolled_2

one_iter:

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]

    adcx    rsi, QWORD PTR [rbp + r13*8]
    adox    rdi, QWORD PTR [rbp + r13*8 + 8]

    mov     QWORD PTR [rbp + r13*8], rsi
    mov     QWORD PTR [rbp + r13*8 + 8], rdi

    lea     r11, [r11 + 1]
    lea     r13, [r13 + 1]
    jmp     loop_unrolled_2

loop_end:

    adcx    r15, QWORD PTR [rbp + r13*8]
    mov     QWORD PTR [rbp + r13*8], r15

    inc     rax
    cmp     r10, rax
    jz      end_of_func
    jmp     loop_outer

loop_unrolled_2:

    jrcxz   loop_end

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8]

    adcx    rsi, QWORD PTR [rbp + r13*8]
    adox    rdi, QWORD PTR [rbp + r13*8 + 8]

    mov     QWORD PTR [rbp + r13*8], rsi
    mov     r14, rdi

    mulx    rdi, rsi, QWORD PTR [rbx + r11*8 + 8]

    adcx    rsi, r14
    adox    rdi, QWORD PTR [rbp + r13*8 + 16]

    mov     QWORD PTR [rbp + r13*8 + 8], rsi
    mov     QWORD PTR [rbp + r13*8 + 16], rdi

    lea     r11, [r11 + 2]
    lea     r13, [r13 + 2]
    lea     rcx, [rcx - 1]
    jmp     loop_unrolled_2

end_of_func:

    pop     rbp
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    ret     

_mul_bc_mulx_adx_2unroll ENDP

END