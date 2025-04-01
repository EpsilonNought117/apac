.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (u64*)
    ;   r8  -> op2 (u64*)
    ;   r9  -> size (u64)

_apn_add_n PROC FRAME

    ; testing 5x loop unroll on Golden Cove UARCH (Alder Lake P-Core)

.pushframe
    push    rdi
.pushreg    rdi
.endprolog

    xor     rdi,    rdi
    mov     rax,    r9
    xchg    rdi,    rdx

    mov     r10,    3
    div     r10
    mov     r11,    rdx
    mov     r9,     rax

    xor     r10,    r10
    xchg    rdx,    rdi

    test    r11,    r11
    jz      unroll_loop_outer

small_loop:
    
    mov     rax,    QWORD PTR [rdx + r10*8]
    adc     rax,    QWORD PTR [r8  + r10*8]
    mov     QWORD PTR [rcx + r10*8],    rax

    inc     r10
    dec     r11
    jnz     small_loop

unroll_loop_outer:
     
    setc   dil          ; set dil to contain carry
    test   r9,  r9
    jz     end_of_func
    bt     di,  0       ; propagate carry between small and unrolled loop

main_loop:
    
    mov     rax,    QWORD PTR [rdx + r10*8     ]
    adc     rax,    QWORD PTR [r8  + r10*8     ]
    mov     QWORD PTR [rcx + r10*8     ],    rax
    
    mov     rax,    QWORD PTR [rdx + r10*8 +  8]
    adc     rax,    QWORD PTR [r8  + r10*8 +  8]
    mov     QWORD PTR [rcx + r10*8 +  8],    rax

    mov     rax,    QWORD PTR [rdx + r10*8 + 16]
    adc     rax,    QWORD PTR [r8  + r10*8 + 16]
    mov     QWORD PTR [rcx + r10*8 + 16],    rax

    lea     r10,    [r10 + 3]
    dec     r9
    jnz     main_loop

end_of_func:

    movzx   rax, dil

    pop     rdi
    ret

_apn_add_n ENDP

END