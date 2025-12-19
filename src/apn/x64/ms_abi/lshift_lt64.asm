
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |						LEFT BITWISE SHIFT FUNCTIONS            			|
;   |                                                                           |
;   O---------------------------------------------------------------------------O


LEFT_SHIFT_LT64 SEGMENT ALIGN(64) 'CODE'

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result       (apn_seg_t*)   
    ;   rdx -> op1          (const apn_seg_t*)
    ;   r8  -> size         (apn_size_t)
    ;   r9  -> bit_cnt      (apn_seg_t)

    ; The wrapper calling this will assert that (bit_cnt < 64 && bit_cnt > 0)
    ; also asserts that size >= 1

; optimized routine for zen4 microarchitecture

lshift_lt64_zen4 PROC FRAME
    
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
.endprolog

    mov     rsi, 64
    mov     rdi, r9     ; lshift val in rdi
    sub     rsi, rdi    ; rshift val in rsi (64 - bit_cnt)
    
before_loop:

    lea     rdx, [rdx + r8 * 8 - 8]
    lea     rcx, [rcx + r8 * 8 - 8]

    shrx    rax, QWORD PTR [rdx], rsi
    dec     r8
    jz      after_loop

    mov     r9,  r8     
    and     r9,  3      ; size %= 4
    shr     r8,  2      ; size /= 4
    test    r8,  r8
    jz      before_rmdr_loop

ALIGN 64
inner_loop_4x_unroll:

i = 0
WHILE i LT 4

    shlx    r10, QWORD PTR [rdx - i * 8], rdi
    shrx    r11, QWORD PTR [rdx - i * 8 - 8], rsi
    or      r11, r10
    mov     QWORD PTR [rcx - i * 8], r11

i = i + 1
ENDM

    sub     rdx, 32
    sub     rcx, 32
    dec     r8
    jnz     inner_loop_4x_unroll

before_rmdr_loop:

    test    r9,  r9
    jz      after_loop   

inner_loop_remainder:

    shlx    r10, QWORD PTR [rdx], rdi
    shrx    r11, QWORD PTR [rdx - 8], rsi
    or      r11, r10
    mov     QWORD PTR [rcx], r11

    sub     rdx, 8
    sub     rcx, 8
    dec     r9
    jnz     inner_loop_remainder

after_loop:

    shlx    r11, QWORD PTR [rdx], rdi
    mov     QWORD PTR [rcx], r11

end_of_func:

    pop     rsi
    pop     rdi
    ret

lshift_lt64_zen4 ENDP

; Lowest common denominator routine

lshift_lt64_x64 PROC FRAME
    push    rbx
.pushreg    rbx
.endprolog

    xchg    rcx, rbx
    xor     rax, rax
    mov     rcx, r9 
    
before_loop:   

    lea     rdx, [rdx + r8 * 8 - 8]
    lea     rbx, [rbx + r8 * 8 - 8]

    mov     r11, QWORD PTR [rdx]
    shld    rax, r11, cl
    dec     r8
    jz      after_loop

    ; loop never entered if size is less than 2

main_loop:

    mov     r10, QWORD PTR [rdx - 8]
    mov     r11, QWORD PTR [rdx]
    shld    r11, r10, cl
    mov     QWORD PTR [rbx], r11

    sub     rdx, 8
    sub     rbx, 8
    dec     r8
    jnz     main_loop

after_loop:

    mov     r11, QWORD PTR [rdx]
    shl     r11, cl
    mov     QWORD PTR [rbx], r11

end_of_func:

    pop     rbx
    ret

lshift_lt64_x64 ENDP

END