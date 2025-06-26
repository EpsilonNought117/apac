
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   BALANCED ADDITION FUNCS (N LIMBS)                       |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

.intel_syntax noprefix
.text

    #   System-V ABI 

    #   Function Arguments
    #
    #   rdi -> result (u64*)   
    #   rsi -> op1 (const u64*)
    #   rdx -> op2 (const u64*)
    #   rcx -> size (u64)

.global add_n_8unroll
.type add_n_8unroll, @Function

add_n_8unroll:

    .cfi_startproc

    mov     r11, rcx
    shr     rcx, 3
    and     r11, 7
    jz      .before_unrolled

.small_loop:

    mov     rax, [rsi]
    adc     rax, [rdx]
    mov     [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdx, [rdx + 8]
    lea     rdi, [rdi + 8]
    dec     r11
    jnz     .small_loop

.before_unrolled:

    setc    al
    test    rcx, rcx
    bt      ax, 0       # bit-test doesn't alter zero flag
    jz      .end_of_func

.unrolled_loop:

    mov     rax, [rsi]
    adc     rax, [rdx]
    mov     [rdi], rax

    mov     rax, [rsi + 8]
    adc     rax, [rdx + 8]
    mov     [rdi + 8], rax

    mov     rax, [rsi + 16]
    adc     rax, [rdx + 16]
    mov     [rdi + 16], rax

    mov     rax, [rsi + 24]
    adc     rax, [rdx + 24]
    mov     [rdi + 24], rax

    mov     rax, [rsi + 32]
    adc     rax, [rdx + 32]
    mov     [rdi + 32], rax

    mov     rax, [rsi + 40]
    adc     rax, [rdx + 40]
    mov     [rdi + 40], rax

    mov     rax, [rsi + 48]
    adc     rax, [rdx + 48]
    mov     [rdi + 48], rax

    mov     rax, [rsi + 56]
    adc     rax, [rdx + 56]
    mov     [rdi + 56], rax

    lea     rsi, [rsi + 64]
    lea     rdx, [rdx + 64]
    lea     rdi, [rdi + 64]
    dec     rcx
    jnz     .unrolled_loop

.end_of_func:

    setc    al
    ret
    
    .cfi_endproc
