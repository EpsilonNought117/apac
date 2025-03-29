
.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (u64*)
    ;   r8  -> op2 (u64*)
    ;   r9  -> size (u64)

_adc_apn_add_n PROC FRAME

.pushframe
    push    r12
.pushreg    r12

    push    rbx
.pushreg    rbx
.endprolog

    xor     rax, rax

    ; saved regs
    xor     rbx, rbx
    xor     r12, r12
    
    xor     r10, r10
    mov     r11, r9
    and     r11, 3
    jz      unroll_loop_outer

small_loop:
    
    mov     rax, [rdx + rbx*8]
    adc     rax, [r8 + rbx*8]
    mov     [rcx + rbx*8], rax

    inc     rbx
    dec     r11
    jnz     small_loop

unroll_loop_outer:
     
    setc    al
    shr     r9, 2
    jz      end_of_func

    lea     r10, [rdx + rbx*8]
    lea     r11, [r8  + rbx*8]
    lea     r12, [rcx + rbx*8]
    xor     rbx, rbx
    bt      ax, 0

main_loop:
    
    mov     rax, [r10]
    adc     rax, [r11]
    mov     [r12], rsi

    mov     rax, [r10 + 8]
    adc     rax, [r11 + 8]
    mov     [r12 + 8], rax

    mov     rax, [r10 + 16]
    adc     rax, [r11 + 16]
    mov     [r12 + 16], rax

    mov     rax, [r10 + 24]
    adc     rax, [r11 + 24]
    mov     [r12 + 24], rax

    lea     r10, [r10 + 32]
    lea     r11, [r11 + 32]
    lea     r12, [r12 + 32]
    dec     r9
    jnz     main_loop

end_of_func:
    setc    bl
    xor     rax, rax
    mov     al, bl

    pop     rbx
    pop     r12
    ret

_adc_apn_add_n ENDP

END