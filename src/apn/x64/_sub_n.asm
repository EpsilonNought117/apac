
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                 BALANCED SUBTRACTION FUNCS (N LIMBS)                      |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

    option casemap:none

    ;   Microsoft ABI 

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> op2 (const u64*)
    ;   r9  -> size (u64)

;   8x Unroll Variant

_sub_n_8unroll PROC FRAME

.pushframe
.endprolog

    mov     r11, r9
    shr     r9,  3      ; size /= 8 
    and     r11, 7      ; size %= 8
    jz      before_big_loop

small_loop:

    mov     rax, QWORD PTR [rdx]
    sbb     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8,  [r8  + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     small_loop

before_big_loop:

    setc    al
    test    r9, r9
    bt      ax, 0
    jz      func_done

big_loop:

    mov     rax, QWORD PTR [rdx]
    sbb     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    mov     rax, QWORD PTR [rdx + 8]
    sbb     rax, QWORD PTR [r8  + 8]
    mov     QWORD PTR [rcx + 8], rax

    mov     rax, QWORD PTR [rdx + 16]
    sbb     rax, QWORD PTR [r8  + 16]
    mov     QWORD PTR [rcx + 16], rax

    mov     rax, QWORD PTR [rdx + 24]
    sbb     rax, QWORD PTR [r8  + 24]
    mov     QWORD PTR [rcx + 24], rax

    mov     rax, QWORD PTR [rdx + 32]
    sbb     rax, QWORD PTR [r8  + 32]
    mov     QWORD PTR [rcx + 32], rax

    mov     rax, QWORD PTR [rdx + 40]
    sbb     rax, QWORD PTR [r8  + 40]
    mov     QWORD PTR [rcx + 40], rax

    mov     rax, QWORD PTR [rdx + 48]
    sbb     rax, QWORD PTR [r8  + 48]
    mov     QWORD PTR [rcx + 48], rax

    mov     rax, QWORD PTR [rdx + 56]
    sbb     rax, QWORD PTR [r8  + 56]
    mov     QWORD PTR [rcx + 56], rax

    lea     rdx, [rdx + 64]
    lea     r8,  [r8  + 64]
    lea     rcx, [rcx + 64]
    dec     r9
    jnz     big_loop

func_done:

    setc    al
    ret     

_sub_n_8unroll ENDP

;   4x Unroll Variant

_sub_n_4unroll PROC FRAME

.pushframe
.endprolog

    mov     r11, r9
    shr     r9,  2      ; size /= 4 
    and     r11, 3      ; size %= 4
    jz      before_big_loop

small_loop:

    mov     rax, QWORD PTR [rdx]
    sbb     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8,  [r8  + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     small_loop

before_big_loop:

    setc    al
    test    r9, r9
    bt      ax, 0
    jz      func_done

big_loop:

    mov     rax, QWORD PTR [rdx]
    sbb     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    mov     rax, QWORD PTR [rdx + 8]
    sbb     rax, QWORD PTR [r8  + 8]
    mov     QWORD PTR [rcx + 8], rax

    mov     rax, QWORD PTR [rdx + 16]
    sbb     rax, QWORD PTR [r8  + 16]
    mov     QWORD PTR [rcx + 16], rax

    mov     rax, QWORD PTR [rdx + 24]
    sbb     rax, QWORD PTR [r8  + 24]
    mov     QWORD PTR [rcx + 24], rax

    lea     rdx, [rdx + 32]
    lea     r8,  [r8  + 32]
    lea     rcx, [rcx + 32]
    dec     r9
    jnz     big_loop

func_done:

    setc    al
    ret 

_sub_n_4unroll ENDP

;   2x Unroll Variant

_sub_n_2unroll PROC FRAME

.pushframe
.endprolog

    xor     rax, rax    ; carry holder/propagator
    mov     r11, r9
    shr     r9,  1      ; size /= 2
    and     r11, 1      ; size %= 2
    jz      before_big_loop

one_iter:

    mov     rax, QWORD PTR [rdx]
    sbb     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8,  [r8  + 8]
    lea     rcx, [rcx + 8]
    dec     r11

before_big_loop:

    setc    al
    test    r9, r9
    bt      ax, 0
    jz      func_done

big_loop:

    mov     rax, QWORD PTR [rdx]
    sbb     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    mov     rax, QWORD PTR [rdx + 8]
    sbb     rax, QWORD PTR [r8  + 8]
    mov     QWORD PTR [rcx + 8], rax

    lea     rdx, [rdx + 16]
    lea     r8,  [r8  + 16]
    lea     rcx, [rcx + 16]
    dec     r9
    jnz     big_loop

func_done:

    setc    al
    ret 

_sub_n_2unroll ENDP

END