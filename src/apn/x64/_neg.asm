
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |						    NEGATION FUNCTIONS    							|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> size (u64)

; 8x unroll version

_neg_8unroll PROC FRAME

.pushframe
.endprolog

    mov     r11, r8
    shr     r8,  3
    and     r11, 7
    jz      unroll_loop_outer

small_loop:

    mov     rax, 0
    mov     r9, QWORD PTR [rdx]
    sbb     rax, r9
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     small_loop

unroll_loop_outer:

    setc    al
    test    r8, r8
    bt      ax, 0
    jz      end_of_func

main_loop:

    mov     rax, 0
    mov     r9, QWORD PTR [rdx]
    sbb     rax, r9
    mov     QWORD PTR [rcx], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 8]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 8], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 16]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 16], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 24]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 24], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 32]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 32], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 40]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 40], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 48]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 48], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 56]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 56], rax

    lea     rdx, [rdx + 64]
    lea     rcx, [rcx + 64]
    dec     r8
    jnz     main_loop

end_of_func:

    ret

_neg_8unroll ENDP

; 4x unroll version

_neg_4unroll PROC FRAME

.pushframe
.endprolog

    mov     r11, r8
    shr     r8,  2
    and     r11, 3
    jz      unroll_loop_outer

small_loop:

    mov     rax, 0
    mov     r9, QWORD PTR [rdx]
    sbb     rax, r9
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     small_loop

unroll_loop_outer:

    setc    al
    test    r8, r8
    bt      ax, 0
    jz      end_of_func

main_loop:
    
    mov     rax, 0
    mov     r9, QWORD PTR [rdx]
    sbb     rax, r9
    mov     QWORD PTR [rcx], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 8]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 8], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 16]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 16], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 24]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 24], rax

    lea     rdx, [rdx + 32]
    lea     rcx, [rcx + 32]
    dec     r8
    jnz     main_loop
 
end_of_func:

    ret

_neg_4unroll ENDP

END