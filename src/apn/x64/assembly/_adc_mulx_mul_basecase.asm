.code 
	
	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> op2 (const u64*)
    ;   r9  -> size1 (u64)
    ;   [rsp + 40] -> size2 (u64)

_adc_mulx_mul_basecase PROC FRAME

.pushframe
    push    rbx
.pushreg    rbx
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
    xor     rbx, rbx    ; will hold op1
    xchg    rbx, rdx    ; now rbx contains op1
    xor     r12, r12    ; temp_reg + aux_carry

loop_outer:

    xor     rsi, rsi    ; low64
    xor     rdi, rdi    ; high64
    xor     r11, r11    ; j
    mov     r13, rax    ; indexer for result
    mov     r10, [rsp + 80]     ; temp_size2
    mov     rdx, [rbx + rax*8]  ; op1[i]

main_loop:
    
    adc     r12, rdi
    mulx    rdi, rsi, QWORD PTR [r8 + r11*8]

    add     r12, rsi
    adc     rdi, 0
    add     QWORD PTR [rcx + r13*8], r12
    
    mov     r12, 0
    inc     r13
    inc     r11
    dec     r10
    jnz     main_loop

loop_end:

    adc     QWORD PTR [rcx + r13*8], rdi
    inc     rax
    cmp     r9, rax
    jnz     loop_outer

    pop     r13
    pop     r12
    pop     rdi
    pop     rsi
    pop     rbx
    ret     0

_adc_mulx_mul_basecase ENDP

END