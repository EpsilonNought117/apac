
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                   ADD SINGLE-LIMB TO APN-ARR FUNCTIONS                    |
#   |                                                                           |
#   O---------------------------------------------------------------------------O
    
    #   Function Arguments
    #
    #   rdi -> result   (apn_seg_t*)
    #   rsi -> op1      (const apn_seg_t*)
    #   rdx -> size     (apn_size_t)
    #   rcx -> val      (apn_seg_t)

.intel_syntax noprefix
.text
.globl add_one_x64, add_one_zen4
.type  add_one_x64, @function
.type  add_one_zen4, @function

add_one_zen4:
.cfi_startproc

    mov     rax, QWORD PTR [rsi]
    add     rax, rcx
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    setc    al
    dec     rdx

    mov     r11, rdx
    shr     rdx, 2
    and     r11, 3
    bt      ax,  0          # doesn't modify zero flag
    jz      2f

1:
    mov     rax, QWORD PTR [rsi]
    adc     rax, 0
    mov     QWORD PTR [rdi], rax

    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     r11
    jnz     1b

2:
    setc    r11b
    test    rdx, rdx
    bt      r11w, 0
    jz      4f

3:
.set i, 0
.rept 4

    mov     rax, QWORD PTR [rsi + i * 8]
    adc     rax, 0
    mov     QWORD PTR [rdi + i * 8], rax

.set i, i + 1
.endr
    
    lea     rdi, [rdi + 32]
    lea     rsi, [rsi + 32]
    dec     rdx
    jnz     3b

4:
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size add_one_zen4, .-add_one_zen4

add_one_x64:
.cfi_startproc
    
    mov     rax, QWORD PTR [rsi]
    add     rax, rcx
    mov     QWORD PTR [rdi], rax
    
    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     rdx
    jz      2f

1:
    mov     rax, QWORD PTR [rsi]
    adc     rax, 0
    mov     QWORD PTR [rdi], rax
    lea     rdi, [rdi + 8]
    lea     rsi, [rsi + 8]
    dec     rdx
    jnz     1b

2:
    setc    al
    movzx   rax, al
    ret

.cfi_endproc
.size add_one_x64, .-add_one_x64

