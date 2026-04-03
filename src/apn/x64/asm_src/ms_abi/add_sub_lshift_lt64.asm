
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                COMBINED ADD/SUB AND LSHIFT LESS THAN 64 FUNCS             |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

ADD_SUB_LSHIFT_LT64 SEGMENT ALIGN(64) 'CODE'

    option casemap:none

    ;   Microsoft ABI 
    ;
	;   Function Arguments
    ;
    ;   rcx -> result           (ap_dig_t*)   
    ;   rdx -> op1              (const ap_dig_t*)
    ;   r8  -> op2              (const ap_dig_t*)
    ;   r9  -> size1            (ap_size_t)
    ;   [rsp + 40] -> bit_cnt   (ap_dig_t) (where rsp is initially at function entering)

    ; result = op1 +- (op2 * (2^bit_cnt))
    ; returns shift_out val +- carry/borrow

; ---------------------------------------------------------------------------
; Macro: GEN_ADD_OR_SUB_LSHIFT_LT64_ZEN4
; ---------------------------------------------------------------------------

GEN_ADD_OR_SUB_LSHIFT_LT64_ZEN4 MACRO OP, INSTR1, INSTR2

OP&_lshift_lt64_zen4 PROC FRAME

    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
    push    r12
.pushreg    r12
    push    r13
.pushreg    r13
.endprolog

    mov     rdi, QWORD PTR [rsp + 72]   ; lshift value in rdi
    mov     rsi, 64     
    sub     rsi, rdi    ; rshift val in rsi (64 - bit_cnt)
    xor     rax, rax

before_loop:

    shlx    r10, QWORD PTR [r8], rdi
    mov     r13, QWORD PTR [rdx]
    INSTR1  r13, r10
    mov     QWORD PTR [rcx], r13

    dec     r9
    jz      after_loop

    setc    al

    mov     r12, r9
    and     r12, 3
    shr     r9,  2

    bt      ax,  0
    jz      before_rmdr_loop

ALIGN 16
unroll_4x_loop:

i = 0
WHILE i LT 4

    mov     r13, QWORD PTR [rdx + i * 8 + 8]
    shrx    r10, QWORD PTR [r8 + i * 8], rsi
    shlx    r11, QWORD PTR [r8 + i * 8 + 8], rdi
    lea     r11, [r11 + r10]
    INSTR2  r13, r11
    mov     QWORD PTR [rcx + i * 8 + 8], r13

i = i + 1
ENDM

    lea     rdx, [rdx + 32]
    lea     r8, [r8 + 32]
    lea     rcx, [rcx + 32]
    dec     r9
    jnz     unroll_4x_loop

before_rmdr_loop:

    setc    al
    test    r12, r12
    bt      ax,  0
    jnz     after_loop

rmdr_loop:

    mov     r13, QWORD PTR [rdx + 8]
    shrx    r10, QWORD PTR [r8], rsi
    shlx    r11, QWORD PTR [r8 + 8], rdi
    lea     r11, [r11 + r10]              
    INSTR2  r13, r11      
    mov     QWORD PTR [rcx + 8], r13

    lea     rdx, [rdx + 8]
    lea     r8, [r8 + 8]
    lea     rcx, [rcx + 8]
    dec     r12
    jnz     rmdr_loop

after_loop:    

    shrx    rax, QWORD PTR [r8], rsi
    INSTR2  rax, 0                        

end_of_func:

    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    ret

OP&_lshift_lt64_zen4 ENDP

ENDM

; ---------------------------------------------------------------------------
; Macro: GEN_ADD_OR_SUB_LSHIFT_LT64_X64
; ---------------------------------------------------------------------------

GEN_ADD_OR_SUB_LSHIFT_LT64_X64 MACRO OP, INSTR1, INSTR2

OP&_lshift_lt64_x64 PROC FRAME

    push    rbx
.pushreg    rbx
    push    r12
.pushreg    r12
.endprolog

    xchg    rcx, rbx
    mov     rcx, QWORD PTR [rsp + 56]

before_loop:

    mov     r12, QWORD PTR [rdx]
    mov     r10, QWORD PTR [r8]
    shl     r10, cl
    xor     rax, rax

    INSTR1  r12, r10
    mov     QWORD PTR [rbx], r12

    setc    al

    dec     r9
    jnz     after_loop

main_loop:

    mov     r12, QWORD PTR [rdx + 8]
    mov     r10, QWORD PTR [r8]
    mov     r11, QWORD PTR [r8 + 8]
    shld    r11, r10, cl

    add     al,  -1
    INSTR2  r12, r11
    mov     QWORD PTR [rbx], r12
    setc    al

    add     rdx, 8
    add     r8,  8
    add     rbx, 8
    dec     r9
    jnz     main_loop

after_loop:

    mov     r10, QWORD PTR [r8]
    neg     cl
    add     cl,  64
    shr     r10, cl
    bt      ax,  0
    mov     rax, r10
    INSTR2  rax, 0

end_of_func:

    pop     r12
    pop     rbx
    ret

OP&_lshift_lt64_x64 ENDP

ENDM

; ---------------------------------------------------------------------------
; Instantiations
; ---------------------------------------------------------------------------

GEN_ADD_OR_SUB_LSHIFT_LT64_ZEN4 add, add, adc
GEN_ADD_OR_SUB_LSHIFT_LT64_ZEN4 sub, sub, sbb

GEN_ADD_OR_SUB_LSHIFT_LT64_X64  add, add, adc
GEN_ADD_OR_SUB_LSHIFT_LT64_X64  sub, sub, sbb

ADD_SUB_LSHIFT_LT64 ENDS

END