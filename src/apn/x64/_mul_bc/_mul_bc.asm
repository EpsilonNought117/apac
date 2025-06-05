
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                 BASECASE MULTIPLICATION FUNC (ADC/MUL)                    |
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

_mul_bc PROC FRAME
    
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
.pushframe
.endprolog

    xchg    rbx, rdx    ; high64 in rdx
    xor     r10, r10    ; i

    ; now rbx contains op1 (const u64*)

outer_loop:

    xor     rdi, rdi        ; temp_reg
    xor     r11, r11        ; j
    xor     rdx, rdx        ; high64
    mov     r12, r10        ; r12 is indexer for result
    mov     r13, r9         ; load size1 into r13
    mov     rsi, QWORD PTR [r8 + r10*8]  ; op2[i]
    mov     rax, rsi

inner_loop:

    adc     rdi, rdx                 ; temp_reg += CF + high64
    mul     QWORD PTR [rbx + r11*8]  ; rdx:rax = rax * op1[j]

    add     rdi, rax                 ; temp_reg += low64
    adc     rdx, 0                   ; high64 += CF
    add     QWORD PTR [rcx + r12*8], rdi    ; result[i + j] += temp_reg

    mov     rax, rsi        ; load op2[i] once again into rax for next iter
    mov     rdi, 0          ; zero out temp_reg
    inc     r11
    inc     r12
    dec     r13
    jnz     inner_loop

loop_end:

    adc     QWORD PTR [rcx + r12*8], rdx
    inc     r10
    mov     rsi, QWORD PTR [rsp + 80]   ; load size2 into rsi
    cmp     rsi, r10        ; check if (rsi - r10) != 0 (means rsi > r10 for unsigned)
    jnz     outer_loop

end_of_func:

    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    ret     0

_mul_bc ENDP

END