
.code

    option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (ap_seg_t*)   
    ;   rdx -> op1 (ap_seg_t*)
    ;   r8  -> op2 (ap_seg_t*)
    ;   r9  -> size (ap_size_t)
    
_adc_apn_add_n PROC

.pushframe
    push    r12
.pushreg    r12

    push    r13
.pushreg    r13

    push    rsi
.pushreg    rsi

    push    rdi
.pushreg    rdi
.endprolog

    xor     rax, rax                ; pass the carry-out here
    xor     r10, r10                ; for array indexing
    mov     r11, r9
    and     r11, 3                  ; remainder_size = size % 4
    jz      unroll_four_times       ; If remainder is zero, skip small loop

small_loop:

    mov     rsi, [rdx + r10*8]
    adc     rsi, [r8  + r10*8]
    mov     [rcx + r10*8], rsi

    inc     r10
    dec     r11
    jnz     small_loop

unroll_four_times:

    setc    al
    shr     r9, 2                    ; Divide size by 4 (handling in chunks)
    jz      end_of_func              ; If no full chunks, end
    bt      ax, 0                    ; set carry for unrolled loop

main_loop:

    mov     rsi, [rdx + r10*8]
    mov     rdi, [rdx + r10*8 + 8]
    mov     r13, [rdx + r10*8 + 16]
    mov     r12, [rdx + r10*8 + 24] 

    adc     rsi, [r8 + r10*8]
    adc     rdi, [r8 + r10*8 + 8]
    adc     r13, [r8 + r10*8 + 16]
    adc     r12, [r8 + r10*8 + 24]

    mov     [rcx + r10*8], rsi
    mov     [rcx + r10*8 + 8], rdi
    mov     [rcx + r10*8 + 16], r13
    mov     [rcx + r10*8 + 24], r12

    lea     r10, [r10 + 4]
    dec     r9
    jnz     main_loop

end_of_func:
    setc    al

    pop     rdi
    pop     rsi
    pop     r13
    pop     r12
    ret

_adc_apn_add_n ENDP

END