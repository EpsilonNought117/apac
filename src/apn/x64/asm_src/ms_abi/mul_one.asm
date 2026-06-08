
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |		         APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS   			|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

MUL_ONE SEGMENT ALIGN(64) 'CODE'

    option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result       (ap_dig_t*)   
    ;   rdx -> op1          (const ap_dig_t*)
    ;   r8  -> size         (ap_size_t)
    ;   r9  -> val          (ap_dig_t)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

mul_one_zen4 PROC FRAME
    
    push    rbx
.pushreg    rbx
.endprolog

start_of_func:

    xchg    rbx, rdx
    mov     rdx, r9
    mov     r11, r8
    and     r11, 7
    shr     r8,  3

    lea     r9,  offset jump_table
    lea     r9,  [r9 + r11 * 8]
    mov     rax, 0
    test    r8,  r8
    jz      before_remainder

ALIGN 64
unroll8_loop:

i = 0
WHILE i LT 8

    mulx    r11, r10, QWORD PTR [rbx + i * 8]
    adc     r10, rax
    mov     QWORD PTR [rcx + i * 8], r10
    mov     rax, r11
        
    i = i + 1
ENDM

    lea     rbx, [rbx + 64]
    lea     rcx, [rcx + 64]
    dec     r8
    jnz     unroll8_loop

before_remainder:

    jmp     QWORD PTR [r9]



end_of_func:

    pop     rbp
    pop     rbx
    ret

mul_one_zen4 ENDP

MUL_ONE ENDS

END