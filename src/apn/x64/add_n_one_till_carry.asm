
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                   UNBALANCED ADDITION HELPER FUNCTION                     |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> size (u64)
    ;   r9  -> val (u64)

; This function is not a performance bottleneck usually in practice.
; Therefore only one common x64 implementation suffices for now.

add_n_one_till_carry_x64 PROC FRAME
.endprolog

    mov     r11, r8

    mov     rax, QWORD PTR [rdx]
    add     rax, r9
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jz      end_of_func

propagate_carry:

    jnc     end_of_func
    mov     rax, QWORD PTR [rdx]
    adc     rax, 0
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     propagate_carry

ALIGN 16
end_of_func:

    setc    al
    movzx   rax, al
    ret

add_n_one_till_carry_x64 ENDP

END