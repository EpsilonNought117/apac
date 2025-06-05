
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

    xor     rax, rax
    xor     r10, r10
    mov     r11, r8
    and     r11, 7
    jz      loop_unroll_outer

small_loop:

    mov     r9, QWORD PTR [rdx + r10*8]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8], r9
    mov     rax, 0

    inc     r10
    dec     r11
    jnz     small_loop

unroll_loop_outer:

    setc    al
    mov     r11, r8
    shr     r11, 3
    bt      ax, 0
    jz      end_of_func

main_loop:

    mov     r9, QWORD PTR [rdx + r10*8]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 8]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 8], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 16]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 16], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 24]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 24], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 32]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 32], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 40]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 40], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 48]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 48], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 56]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 56], r9
    mov     rax, 0

    lea     r10, [r10 + 8]
    dec     r11
    jnz     main_loop

end_of_func:

    ret     0

_neg_8unroll ENDP

; 4x Unroll

_neg_4unroll PROC FRAME
.pushframe
.endprolog

    xor     rax, rax
    xor     r10, r10
    mov     r11, r8
    and     r11, 3
    jz      unroll_loop_outer

small_loop:

    mov     r9, QWORD PTR [rdx + r10*8]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8], r9
    mov     rax, 0

    inc     r10
    dec     r11
    jnz     small_loop

unroll_loop_outer:

    setc    al
    mov     r11, r8
    shr     r11, 2
    bt      ax, 0
    jz      end_of_func

main_loop:

    mov     r9, QWORD PTR [rdx + r10*8]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 8]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 8], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 16]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 16], r9
    mov     rax, 0

    mov     r9, QWORD PTR [rdx + r10*8 + 24]
    sbb     rax, r9
    mov     QWORD PTR [rcx + r10*8 + 24], r9
    mov     rax, 0

    lea     r10, [r10 + 4]
    dec     r11
    jnz     main_loop
 
end_of_func:

    ret     0

_neg_4unroll ENDP

END