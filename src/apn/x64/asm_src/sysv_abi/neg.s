
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |						    NEGATION FUNCTIONS    							|
#   |                                                                           |
#   O---------------------------------------------------------------------------O
    
    #   Function Arguments
    #
    #   rdi -> result       (apn_dig_t*)
    #   rsi -> op1          (const apn_dig_t*)
    #   rdx -> size         (apn_size_t)

.intel_syntax noprefix
.text
.globl neg_x64, neg_x64
.type  neg_x64, @function
.type  neg_x64, @function

# Negations (2's Complement) procedure tuned for
# AMD's Zen4 Microarchitecture with 4x unroll

neg_x64:
.cfi_startproc

    mov     r11, rdx
    shr     rdx, 2
    and     r11, 3

    test    r11, r11
    stc    
    jz      .Lx64_before_unroll_loop

.Lx64_rmdr_loop:

    mov     rax, QWORD PTR [rsi]
    not     rax
    adc     rax, 0
    mov     QWORD PTR [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 8]
    dec     r11
    jnz     .Lx64_rmdr_loop

.Lx64_before_unroll_loop:
    
    setc    al
    test    rdx, rdx
    bt      ax, 0
    jz      .Lx64_end_of_func

.Lx64_unroll_loop:

.set i, 0
.rept 4

    mov     rax, QWORD PTR [rsi + i*8]
    not     rax
    adc     rax, 0
    mov     QWORD PTR [rdi + i*8], rax

.set i, i + 1
.endr

    lea     rsi, [rsi + 32]
    lea     rdi, [rdi + 32]
    dec     rdx
    jnz     .Lx64_unroll_loop

.p2align 4
.Lx64_end_of_func:

    # discard any carry or borrow if generated
    ret

.cfi_endproc
.size neg_x64, .-neg_x64

.section .note.GNU-stack,"",@progbits
