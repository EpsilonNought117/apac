
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                   BALANCED ADDITION FUNCS (N LIMBS)                       |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

BITS 64

section .text 

global _add_n_8unroll
global _add_n_4unroll
global _add_n_2unroll

%ifdef _WIN64

    ;   Microsoft ABI 

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> op2 (const u64*)
    ;   r9  -> size (u64)

;   8x Unroll Variant

_add_n_8unroll:

    mov     r11, r9
    shr     r9,  3      ; size /= 8 
    and     r11, 7      ; size %= 8
    jz      .before_big_loop

.small_loop:

    mov     rax, QWORD [rdx]
    adc     rax, QWORD [r8]
    mov     QWORD [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8,  [r8  + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     .small_loop

.before_big_loop:

    setc    al
    test    r9, r9
    bt      ax, 0
    jz      .func_done

.big_loop:

    mov     rax, QWORD [rdx]
    adc     rax, QWORD [r8]
    mov     QWORD [rcx], rax

    mov     rax, QWORD [rdx + 8]
    adc     rax, QWORD [r8  + 8]
    mov     QWORD [rcx + 8], rax

    mov     rax, QWORD [rdx + 16]
    adc     rax, QWORD [r8  + 16]
    mov     QWORD [rcx + 16], rax

    mov     rax, QWORD [rdx + 24]
    adc     rax, QWORD [r8  + 24]
    mov     QWORD [rcx + 24], rax

    mov     rax, QWORD [rdx + 32]
    adc     rax, QWORD [r8  + 32]
    mov     QWORD [rcx + 32], rax

    mov     rax, QWORD [rdx + 40]
    adc     rax, QWORD [r8  + 40]
    mov     QWORD [rcx + 40], rax

    mov     rax, QWORD [rdx + 48]
    adc     rax, QWORD [r8  + 48]
    mov     QWORD [rcx + 48], rax

    mov     rax, QWORD [rdx + 56]
    adc     rax, QWORD [r8  + 56]
    mov     QWORD [rcx + 56], rax

    lea     rdx, [rdx + 64]
    lea     r8,  [r8  + 64]
    lea     rcx, [rcx + 64]
    dec     r9
    jnz     .big_loop

.func_done:

    setc    al
    ret     

;   4x Unroll Variant

_add_n_4unroll:

    mov     r11, r9
    shr     r9,  2      ; size /= 4 
    and     r11, 3      ; size %= 4
    jz      .before_big_loop

.small_loop:

    mov     rax, QWORD [rdx]
    adc     rax, QWORD [r8]
    mov     QWORD [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8,  [r8  + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     .small_loop

.before_big_loop:

    setc    al
    test    r9, r9
    bt      ax, 0
    jz      .func_done

.big_loop:

    mov     rax, QWORD [rdx]
    adc     rax, QWORD [r8]
    mov     QWORD [rcx], rax

    mov     rax, QWORD [rdx + 8]
    adc     rax, QWORD [r8  + 8]
    mov     QWORD [rcx + 8], rax

    mov     rax, QWORD [rdx + 16]
    adc     rax, QWORD [r8  + 16]
    mov     QWORD [rcx + 16], rax

    mov     rax, QWORD [rdx + 24]
    adc     rax, QWORD [r8  + 24]
    mov     QWORD [rcx + 24], rax

    lea     rdx, [rdx + 32]
    lea     r8,  [r8  + 32]
    lea     rcx, [rcx + 32]
    dec     r9
    jnz     .big_loop

.func_done:

    setc    al
    ret 

;   2x Unroll Variant

_add_n_2unroll:

    xor     rax, rax    ; carry holder/propagator
    mov     r11, r9
    shr     r9,  1      ; size /= 2
    and     r11, 1      ; size %= 2
    jz      .before_big_loop

.one_iter:

    mov     rax, QWORD [rdx]
    adc     rax, QWORD [r8]
    mov     QWORD [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8,  [r8  + 8]
    lea     rcx, [rcx + 8]
    dec     r11

.before_big_loop:

    setc    al
    test    r9, r9
    bt      ax, 0
    jz      .func_done

.big_loop:

    mov     rax, QWORD [rdx]
    adc     rax, QWORD [r8]
    mov     QWORD [rcx], rax

    mov     rax, QWORD [rdx + 8]
    adc     rax, QWORD [r8  + 8]
    mov     QWORD [rcx + 8], rax

    lea     rdx, [rdx + 16]
    lea     r8,  [r8  + 16]
    lea     rcx, [rcx + 16]
    dec     r9
    jnz     .big_loop

.func_done:

    setc    al
    ret 

%elifdef __linux__

    ; SystemV ABI

    ;   Function Arguments
    ;
    ;   rdi -> result (u64*)   
    ;   rsi -> op1 (const u64*)
    ;   rcx -> op2 (const u64*)
    ;   rdx -> size (u64)
    
;   8x Unroll Variant

_add_n_8unroll:

    mov     r8,  rdx
    shr     rdx, 3
    and     r8,  7
    jz      .before_big_loop

.small_loop:

    mov     rax, QWORD [rsi]
    adc     rax, QWORD [rcx]
    mov     QWORD [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rcx, [rcx + 8]
    lea     rdi, [rdi + 8]
    dec     r8
    jnz     .small_loop    

.before_big_loop:

    setc    al
    test    rdx, rdx
    bt      ax, 0
    jz      .func_done

.big_loop:

    mov     rax, QWORD [rsi]
    adc     rax, QWORD [rcx]
    mov     QWORD [rdi], rax

    mov     rax, QWORD [rsi + 8]
    adc     rax, QWORD [rcx + 8]
    mov     QWORD [rdi + 8], rax

    mov     rax, QWORD [rsi + 16]
    adc     rax, QWORD [rcx + 16]
    mov     QWORD [rdi + 16], rax

    mov     rax, QWORD [rsi + 24]
    adc     rax, QWORD [rcx + 24]
    mov     QWORD [rdi + 24], rax

    mov     rax, QWORD [rsi + 32]
    adc     rax, QWORD [rcx + 32]
    mov     QWORD [rdi + 32], rax

    mov     rax, QWORD [rsi + 40]
    adc     rax, QWORD [rcx + 40]
    mov     QWORD [rdi + 40], rax

    mov     rax, QWORD [rsi + 48]
    adc     rax, QWORD [rcx + 48]
    mov     QWORD [rdi + 48], rax

    mov     rax, QWORD [rsi + 56]
    adc     rax, QWORD [rcx + 56]
    mov     QWORD [rdi + 56], rax

    lea     rsi, [rsi + 64]
    lea     rcx, [rcx + 64]
    lea     rdi, [rdi + 64]
    dec     rdx
    jnz     .big_loop 

.func_done:

    setc    al
    ret     

_add_n_4unroll:

    mov     r8,  rdx
    shr     rdx, 2
    and     r8,  3
    jz      .before_big_loop

.small_loop:

    mov     rax, QWORD [rsi]
    adc     rax, QWORD [rcx]
    mov     QWORD [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rcx, [rcx + 8]
    lea     rdi, [rdi + 8]
    dec     r8
    jnz     .small_loop

.before_big_loop:

    setc    al
    test    rdx, rdx
    bt      ax, 0
    jz      .func_done

.big_loop:

    mov     rax, QWORD [rsi]
    adc     rax, QWORD [rcx]
    mov     QWORD [rdi], rax

    mov     rax, QWORD [rsi + 8]
    adc     rax, QWORD [rcx + 8]
    mov     QWORD [rdi + 8], rax

    mov     rax, QWORD [rsi + 16]
    adc     rax, QWORD [rcx + 16]
    mov     QWORD [rdi + 16], rax

    mov     rax, QWORD [rsi + 24]
    adc     rax, QWORD [rcx + 24]
    mov     QWORD [rdi + 24], rax

    lea     rsi, [rsi + 32]
    lea     rcx, [rcx + 32]
    lea     rdi, [rdi + 32]
    dec     rdx
    jnz     .big_loop

.func_done:

    setc    al
    ret         

_add_n_2unroll:

    mov     r8,  rdx
    shr     rdx, 1
    and     r8,  1
    jz      .before_big_loop

.one_iter:

    mov     rax, QWORD [rsi]
    adc     rax, QWORD [rcx]
    mov     QWORD [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rcx, [rcx + 8]
    lea     rdi, [rdi + 8]
    dec     r8

.before_big_loop:

    setc    al
    test    rdx, rdx
    bt      ax, 0
    jz      .func_done    

.big_loop:

    mov     rax, QWORD [rsi]
    adc     rax, QWORD [rcx]
    mov     QWORD [rdi], rax

    mov     rax, QWORD [rsi + 8]
    adc     rax, QWORD [rcx + 8]
    mov     QWORD [rdi + 8], rax

    lea     rsi, [rsi + 16]
    lea     rcx, [rcx + 16]
    lea     rdi, [rdi + 16]
    dec     rdx
    jnz     .big_loop

.func_done:

    setc    al
    ret

%endif