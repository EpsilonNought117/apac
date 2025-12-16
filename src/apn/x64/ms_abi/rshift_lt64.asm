
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |					   RIGHT BITWISE SHIFT FUNCTIONS            			|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

RIGHT_SHIFT_LT64 SEGMENT ALIGN(64) 'CODE'

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

rshift_lt64_zen4 PROC FRAME
    push    rbx
.pushreg    rbx
.endprolog

    xchg    rcx, rbx
    xor     rax, rax
    mov     rcx, r9
    xor     r9,  r9

before_loop:

    mov     r11, QWORD PTR [rdx]
    shrd    rax, r11, cl
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

    mov     r10, QWORD PTR [rdx + i * 8 + 8]
    mov     r11, QWORD PTR [rdx + i * 8]
    shrd    r11, r10, cl
    mov     QWORD PTR [rbx + i * 8], r11

i = i + 1
ENDM

    add     rdx, 32
    add     rbx, 32
    dec     r8
    jnz     inner_loop_4x_unroll

before_rmdr_loop:

    test    r9,  r9
    jz      after_loop

inner_loop_remainder:

    mov     r10, QWORD PTR [rdx + 8]
    mov     r11, QWORD PTR [rdx]
    shrd    r11, r10, cl
    mov     QWORD PTR [rbx], r11

    add     rdx, 8
    add     rbx, 8
    dec     r9
    jnz     inner_loop_remainder

after_loop:

    mov     r11, QWORD PTR [rdx]
    shr     r11, cl
    mov     QWORD PTR [rbx], r11

end_of_func:

    pop     rbx
    ret

rshift_lt64_zen4 ENDP

; lowest common denominator x64 routine

rshift_lt64_x64 PROC FRAME
    push    rbx
.pushreg    rbx
.endprolog
    
    xchg    rbx, rcx
    xor     rax, rax
    mov     rcx, r9 

before_loop:

    mov     r11, QWORD PTR [rdx]
    shrd    rax, r11, cl
    dec     r8
    jz      after_loop

main_loop:

    mov     r10, QWORD PTR [rdx + 8]
    mov     r11, QWORD PTR [rdx]
    shrd    r11, r10, cl
    mov     QWORD PTR [rbx], r11    

    add     rdx, 8
    add     rbx, 8
    dec     r8
    jnz     main_loop

after_loop:

    mov     r11, QWORD PTR [rdx]
    shr     r11, cl
    mov     QWORD PTR [rbx], r11

end_of_func:

    pop     rbx
    ret

rshift_lt64_x64 ENDP

END