
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                        BASECASE SQUARING FUNCTIONS                        |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> size (u64)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

; The fastest basecase sqr procedures with 2x, 4x and 8x unrolled variants
; Compatible on processors supporting ADX and BMI2 ISA Extensions

;   8x Unroll Variant

_sqr_bc_mulx_adx_8unroll PROC FRAME

    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
    push    r12
.pushreg    r12
    push    r13
.pushreg    r13
    push    r14
.pushreg    r14
    push    r15
.pushreg    r15
    push    rbp
.pushreg    rbp
.pushframe
.endprolog

    xor     rax, rax
    xchg    rbp, rcx    ; result
    xchg    rbx, rdx    ; op1
    mov     r11, rbx
    mov     r13, rbp
    mov     r10, r9     ; store size for outer loop
    mov     r12, r9     ; for inner unrolled loop
    shr     r9,  3      ; 8x unroll
    and     r12, 7      ; for handling remainder
    xor     r15, r15

outer_loop:

    mov     rcx, r9
    mov     rdx, QWORD PTR [r11]
    lea     rbp, [r13]
    lea     rbx, [r11]
    add     r11, 8
    add     r13, 8

    test    rcx, rcx
    jz      before_remainder

inner_unrolled:

    mulx    rdi, rsi, QWORD PTR [r]

end_of_func:

    pop     rbp
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    ret     

_sqr_bc_mulx_adx_8unroll ENDP