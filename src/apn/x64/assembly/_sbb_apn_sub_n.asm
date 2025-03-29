.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (u64*)
    ;   r8  -> op2 (u64*)
    ;   r9  -> size (u64)

_sbb_apn_sub_n PROC FRAME

.pushframe
    push    r12
.pushreg    r12

    push    r13
.pushreg    r13

    push    rsi
.pushreg    rsi

    push    rdi
.pushreg    rdi
.endprolog

    xor     rax,    rax
    xor     r10,    r10
    mov     r11,    r9
    and     r11,    3
    jz      unroll_loop_outer

small_loop:
    
    mov     rsi,    QWORD PTR [rdx + r10*8]
    sbb     rsi,    QWORD PTR [r8  + r10*8]
    mov     QWORD PTR [rcx + r10*8],    rsi

    inc     r10
    dec     r11
    jnz     small_loop

unroll_loop_outer:
     
    setc   al
    shr    r9,  2
    jz     end_of_func
    bt     ax,  0

main_loop:
    
    mov     rsi,    QWORD PTR [rdx + r10*8     ]
    mov     rdi,    QWORD PTR [rdx + r10*8 +  8]
    mov     r13,    QWORD PTR [rdx + r10*8 + 16]
    mov     r12,    QWORD PTR [rdx + r10*8 + 24] 

    sbb     rsi,    QWORD PTR [r8 + r10*8     ]
    sbb     rdi,    QWORD PTR [r8 + r10*8 +  8]
    sbb     r13,    QWORD PTR [r8 + r10*8 + 16]
    sbb     r12,    QWORD PTR [r8 + r10*8 + 24]

    mov     QWORD PTR [rcx + r10*8     ],   rsi
    mov     QWORD PTR [rcx + r10*8 +  8],   rdi
    mov     QWORD PTR [rcx + r10*8 + 16],   r13
    mov     QWORD PTR [rcx + r10*8 + 24],   r12

    lea     r10,    QWORD PTR [r10 + 4]
    dec     r9
    jnz     main_loop

end_of_func:

    setc    al

    pop     rdi
    pop     rsi
    pop     r13
    pop     r12
    ret

_sbb_apn_sub_n ENDP

END