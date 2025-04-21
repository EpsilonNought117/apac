.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (u64*)
    ;   r8  -> op2 (u64*)
    ;   r9  -> size (u64)

_adc_add_n PROC FRAME
.pushframe
.endprolog

    xor     rax, rax        ; carry store and accumulator for adc
    xor     r10, r10        ; indexing variable
    mov     r11, r9         
    and     r11, 3          ; handle remaining limbs case first
    jz      unroll_loop_outer

small_loop:
    
    mov     rax, QWORD PTR [rdx + r10*8]
    adc     rax, QWORD PTR [r8  + r10*8]
    mov     QWORD PTR [rcx + r10*8], rax

    inc     r10
    dec     r11
    jnz     small_loop

unroll_loop_outer:
     
    setc    al
    mov     r11, r9
    shr     r11, 2      ; r11 = r11 / 4 (for handling 4 chunks at once)
    bt      ax, 0       ; restore carry flag if destroyed by shr
    
    ; bt (bit-test) doesn't affect zero flag

    jz      end_of_func

main_loop:
    
    mov     rax, QWORD PTR [rdx + r10*8]
    adc     rax, QWORD PTR [r8  + r10*8]
    mov     QWORD PTR [rcx + r10*8], rax

    mov     rax, QWORD PTR [rdx + r10*8 + 8]
    adc     rax, QWORD PTR [r8  + r10*8 + 8]
    mov     QWORD PTR [rcx + r10*8 + 8], rax

    mov     rax, QWORD PTR [rdx + r10*8 + 16]
    adc     rax, QWORD PTR [r8  + r10*8 + 16]
    mov     QWORD PTR [rcx + r10*8 + 16], rax

    mov     rax, QWORD PTR [rdx + r10*8 + 24]
    adc     rax, QWORD PTR [r8  + r10*8 + 24]
    mov     QWORD PTR [rcx + r10*8 + 24], rax

    lea     r10, [r10 + 4]
    dec     r11
    jnz     main_loop

end_of_func:

    setc    al
    ret
_adc_add_n ENDP

END