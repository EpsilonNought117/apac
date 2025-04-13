.code

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> op2 (const u64*)
    ;   r9  -> size1 (u64)
    ;   [rsp + 40] -> size2 (u64)

_adc_mul_basecase PROC FRAME

.pushframe
    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    r12
.pushreg    r12
    push    r13
.pushreg    r13
.endprolog

    xor     rax, rax    ; low64 and op1[i]
    xor     rbx, rbx    
    xchg    rbx, rdx    
    xor     r10, r10    ; i

    ; rbx <- op1
    ; rdx <- free for mul

outer_loop:

    xor     rdi, rdi        ; carry + temp_reg
    xor     r11, r11        ; j
    mov     r12, r10                     ; r12 is indexer for result
    mov     r13, QWORD PTR [rsp + 72]    ; load size2 into 13
    mov     rax, QWORD PTR [rbx + r10*8] ; op1[i]

inner_loop:

    adc     rdi, rdx
    mul     QWORD PTR [r8 + r11*8]  ; rdx:rax = rax * op2[j]

    add     rdi, rax
    adc     rdx, 0
    add     QWORD PTR [rcx + r12*8], rdi

    mov     rax, QWORD PTR [rbx + r10*8]    ; op1[i]
    mov     rdi, 0
    inc     r11
    inc     r12
    dec     r13
    jnz     inner_loop

loop_end:

    adc     QWORD PTR [r8 + r12*8], rdx
    inc     r10
    cmp     r9, r10
    jnz     outer_loop

end_of_func:

    pop     r13
    pop     r12
    pop     rdi
    pop     rbx
    ret

_adc_mul_basecase ENDP

END