
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |               ADD/SUB SINGLE-DIGIT TO AP-DIG-ARR FUNCTIONS                |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

ADD_SUB_ONE SEGMENT ALIGN(64) 'CODE'

    option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result       (apn_dig_t*)   
    ;   rdx -> op1          (const apn_dig_t*)
    ;   r8  -> size         (apn_size_t)
    ;   r9  -> val          (apn_dig_t)

; ---------------------------------------------------------------------------
; Zen4 / Unrolled Macro
; ---------------------------------------------------------------------------

GEN_ADD_SUB_ONE_ZEN4 MACRO OP, INSTR1, INSTR2

OP&_one_zen4 PROC FRAME
.endprolog

    ; assumes r8 is at least 1
    ; via APAC_ASSERT in caller function

    mov     rax, QWORD PTR [rdx]
    INSTR1  rax, r9
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    setc    al
    dec     r8

    mov     r11, r8
    shr     r8,  2
    and     r11, 3
    bt      ax,  0          ; doesn't modify zero flag
    jz      before_unroll

small_loop:

    mov     rax, QWORD PTR [rdx]
    INSTR2  rax, 0
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     small_loop

before_unroll:

    setc    al
    test    r8,  r8
    bt      ax,  0      ; same here       
    jz      end_of_func

ALIGN 16
loop_unrolled:

i = 0
WHILE i LT 4

    mov     rax, QWORD PTR [rdx + i * 8]
    INSTR2  rax, 0
    mov     QWORD PTR [rcx + i * 8], rax
        
i = i + 1
ENDM

    lea     rdx, [rdx + 32]
    lea     rcx, [rcx + 32]
    dec     r8
    jnz     loop_unrolled

end_of_func:

    setc    al
    movzx   rax, al
    ret

OP&_one_zen4 ENDP

ENDM

; ---------------------------------------------------------------------------
; Generic x64 Macro
; ---------------------------------------------------------------------------

GEN_ADD_SUB_ONE_X64 MACRO OP, INSTR1, INSTR2

OP&_one_x64 PROC FRAME
.endprolog

    mov     rax, QWORD PTR [rdx]
    INSTR1  rax, r9
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r8
    jz      end_of_func

main_loop:

    mov     rax, QWORD PTR [rdx]
    INSTR2  rax, 0
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r8
    jnz     main_loop

end_of_func:

    setc    al
    movzx   rax, al
    ret

OP&_one_x64 ENDP

ENDM

; ---------------------------------------------------------------------------
; Instantiations
; ---------------------------------------------------------------------------

; Zen4 versions
GEN_ADD_SUB_ONE_ZEN4 add, add, adc
GEN_ADD_SUB_ONE_ZEN4 sub, sub, sbb

; Generic x64 versions
GEN_ADD_SUB_ONE_X64 add, add, adc
GEN_ADD_SUB_ONE_X64 sub, sub, sbb

ADD_SUB_ONE ENDS

END