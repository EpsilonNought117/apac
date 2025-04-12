.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> size (u64)
    ;   r9  -> val (u64)

_adc_till_carry PROC FRAME
.pushframe
.endprolog

    xor     rax, rax
    xor     r10, r10
    mov     r11, r8

    mov     rax, QWORD PTR [rdx + r10*8]
    add     rax, r9
    mov     QWORD PTR [rcx + r10*8], rax

    inc     r10
    dec     r11
    jz      end_of_func

propagate_carry:

    jnc     end_of_func
    mov     rax, QWORD PTR [rdx + r10*8]
    adc     rax, 0
    mov     QWORD PTR [rcx + r10*8], rax

    inc     r10
    dec     r11
    jnz     propagate_carry

end_of_func:

    setc    al
    ret

_adc_till_carry ENDP

END

