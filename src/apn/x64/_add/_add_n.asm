
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                   BALANCED ADDITION FUNCS (N LIMBS)                       |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> op2 (const u64*)
    ;   r9  -> size (u64)

; 8x unroll version

_add_n_8unroll PROC FRAME

.pushframe
.endprolog

    xor     rax, rax
    xor     r10, r10
    mov     r11, r9
    and     r11, 7      ; handle remaining limbs first
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
    shr     r11, 3      ; r11 /= 4
    bt      ax, 0       ; restore carry flag if destroyed by shr
    jz      end_of_func ; bt (bit-test) doesn't affect zero flag

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
    
    mov     rax, QWORD PTR [rdx + r10*8 + 32]
    adc     rax, QWORD PTR [r8  + r10*8 + 32]
    mov     QWORD PTR [rcx + r10*8 + 32], rax

    mov     rax, QWORD PTR [rdx + r10*8 + 40]
    adc     rax, QWORD PTR [r8  + r10*8 + 40]
    mov     QWORD PTR [rcx + r10*8 + 40], rax

    mov     rax, QWORD PTR [rdx + r10*8 + 48]
    adc     rax, QWORD PTR [r8  + r10*8 + 48]
    mov     QWORD PTR [rcx + r10*8 + 48], rax

    mov     rax, QWORD PTR [rdx + r10*8 + 56]
    adc     rax, QWORD PTR [r8  + r10*8 + 56]
    mov     QWORD PTR [rcx + r10*8 + 56], rax

    lea     r10, [r10 + 8]
    dec     r11
    jnz     main_loop

end_of_func:

    setc    al
    ret     0

_add_n_8unroll ENDP

; 4x unroll version

_add_n_4unroll PROC FRAME

.pushframe
.endprolog

    xor     rax, rax        ; carry (borrow) store and accumulator for adc
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
    ret     0

_add_n_4unroll ENDP

; 2x unroll version

_add_n_2unroll PROC FRAME

.pushframe
.endprolog

    xor     rax, rax
    xor     r10, r10
    mov     r11, r9
    and     r11, 1
    jz      unroll_loop_outer

one_iter:

    mov     rax, QWORD PTR [rdx + r10*8]
    adc     rax, QWORD PTR [r8  + r10*8]
    mov     QWORD PTR [rcx + r10*8], rax

    inc     r10
    dec     r11

unroll_loop_outer:

    setc    al
    mov     r11, r9
    shr     r11, 1
    bt      ax, 0
    
    ; same here as 4x and 8x unroll

    jz      end_of_func

main_loop:

    mov     rax, QWORD PTR [rdx + r10*8]
    adc     rax, QWORD PTR [r8  + r10*8]
    mov     QWORD PTR [rcx + r10*8], rax

    mov     rax, QWORD PTR [rdx + r10*8 + 8]
    adc     rax, QWORD PTR [r8  + r10*8 + 8]
    mov     QWORD PTR [rcx + r10*8 + 8], rax

    lea     r10, [r10 + 2]
    dec     r11
    jnz     main_loop

end_of_func:

    setc    al
    ret     0

_add_n_2unroll ENDP

END