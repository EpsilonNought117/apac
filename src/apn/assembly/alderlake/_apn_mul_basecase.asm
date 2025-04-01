.code 
	
	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> op2 (const u64*)
    ;   r9  -> size1 (u64)
    ;   [rsp + 40] -> size2 (u64) (eventually in r10)

_apn_mul_basecase PROC FRAME

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
    
    push    r14
.pushreg    r14

    push    r15
.pushreg    r15
.endprolog

    mov     r10, QWORD PTR [rsp + 88] ; contains size2

    ; 11 * 8 = 88 bytes
    ; 6 pushed registers (48-bytes total) +
    ; 1 return address (8-bytes) + 
    ; 4-element shadow space (32-bytes)
    ; 5th Argument (8-bytes) -> value to mov

    xor     rax, rax
    xor     rbx, rbx

    xchg    rbx, rdx        ; use rdx for the mulx ops
    xor     r13, r13
    xor     r14, r14
    
    ; now rbx contains op1 

single_loop_begin:

    xor     rsi, rsi            ; temp_reg and aux
    xor     rdi, rdi            ; high64
    xor     r12, r12            ; low64    
    xor     r11, r11            ; j
    xor     r15, r15            

    mov     r13, r10                        ; r13 = size2
    mov     rdx, QWORD PTR [rbx + rax*8]    ; rdx = op1[i]
    
single_loop:
    
    ; hot loop

    adc     rsi, rdi                                ; temp_reg = high64 + CF
    mulx    r14, r12, QWORD PTR [r8 + r11*8]        ; r14:r12 = op1[i] * op2[j] (implicit rdx)

    add     rsi, r12                                ; temp_reg += low64
    adc     rdi, r14                                ; high64 += CF
    lea     r14, [rax + r11]                        ; r14 = i + j
    add     QWORD PTR [rcx + r14*8], rsi            ; result[i + j] += temp_reg

    mov     rsi, r15
    inc     r11                 ; j++
    dec     r13                 ; for checking loop termination
    jnz     single_loop

single_loop_end:
    
    lea     r14, [r10 + rax]                ; r14 = size2 + i
    adc     QWORD PTR [rcx + r14*8], rdi    ; result[i + size2] = (most recent) high64 + CF
    inc     rax                             ; i++
    cmp     r9, rax                         ; check if i < size1
    jnz     single_loop_begin

end_of_func:

    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rdi
    pop     rsi
    pop     rbx
	ret     0

_apn_mul_basecase ENDP

END