
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |						    NEGATION FUNCTIONS    							|
#   |                                                                           |
#   O---------------------------------------------------------------------------O
    
    #   Function Arguments
    #
    #   rdi -> result       (apn_seg_t*)
    #   rsi -> op1          (const apn_seg_t*)
    #   rdx -> size         (apn_size_t)

.intel_syntax noprefix
.text
.globl neg_x64, neg_zen4
.type  neg_x64, @function
.type  neg_zen4, @function

# Negations (2's Complement) procedure tuned for
# AMD's Zen4 Microarchitecture with 4x unroll

neg_zen4:
.cfi_startproc

    mov     r11, rdx
    shr     rdx, 2
    and     r11, 3
    jz      .Lzen4_before_unroll_loop

.Lzen4_rmdr_loop:

    mov     rax, 0
    mov     r9, QWORD PTR [rsi]
    sbb     rax, r9
    mov     QWORD PTR [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 8]
    dec     r11
    jnz     .Lzen4_rmdr_loop

.Lzen4_before_unroll_loop:
    
    setc    al
    test    rdx, rdx
    bt      ax, 0
    jz      .Lzen4_end_of_func

.Lzen4_unroll_loop:

.set i, 0
.rept 4

    mov     rax, 0
    mov     r9, QWORD PTR [rsi + i*8]
    sbb     rax, r9
    mov     QWORD PTR [rdi + i*8], rax

.set i, i + 1
.endr

    lea     rsi, [rsi + 32]
    lea     rdi, [rdi + 32]
    dec     rdx
    jnz     .Lzen4_unroll_loop

.p2align 4
.Lzen4_end_of_func:

    # discard any carry or borrow if generated
    ret

.cfi_endproc
.size neg_zen4, .-neg_zen4

# lowest common denominator generic implementation

neg_x64:
.cfi_startproc

    test    rdx, rdx
    jz      .Lx64_end_of_func

.Lx64_main_loop:

    mov     rax, 0
    mov     r9, QWORD PTR [rsi]
    sbb     rax, r9
    mov     QWORD PTR [rdi], rax

    lea     rsi, [rsi + 8]
    lea     rdi, [rdi + 8]
    dec     rdx
    jnz     .Lx64_main_loop

.Lx64_end_of_func:

    # no need to return the borrow as this is
    # 2's complement negation
    ret

.cfi_endproc
.size neg_x64, .-neg_x64

