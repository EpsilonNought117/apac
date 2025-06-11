
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                 UNBALANCED SUBTRACTION HELPER FUNCTION                    |
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

_sub_n_one_till_borrow PROC FRAME
.pushframe
.endprolog

    mov     r11, r8

    mov     rax, QWORD PTR [rdx]
    sub     rax, r9
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jz      end_of_func

propagate_borrow:

    jnc     end_of_func
    mov     rax, QWORD PTR [rdx]
    sbb     rax, 0
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     propagate_borrow

end_of_func:

    setc    al
    ret

_sub_n_one_till_borrow ENDP

END