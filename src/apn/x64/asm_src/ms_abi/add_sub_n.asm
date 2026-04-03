
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |              BALANCED ADDITION/SUBTRACTION FUNCS (N DIGITS)               |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

ADD_SUB_N SEGMENT ALIGN(64) 'CODE'

    option casemap:none

    ;   Microsoft ABI 
    ;
    ;   Function Arguments
    ;
    ;   rcx -> result       (ap_dig_t*)   
    ;   rdx -> op1          (const ap_dig_t*)
    ;   r8  -> op2          (const ap_dig_t*)
    ;   r9  -> size         (ap_size_t)

GEN_ADD_SUB_N_ZEN4 MACRO OP, INSTR

OP&_n_zen4 PROC FRAME
.endprolog

    xor     rax, rax            ; clear rax, also clears CF
    mov     r11, r9             ; copy size
    shr     r9,  2              ; r9 = size / 4 (unrolled iterations)
    and     r11, 3              ; r11 = size % 4 (remainder)
    jz      before_unrolled     ; skip remainder loop if zero

small_loop:

    mov     rax, QWORD PTR [rdx] 
    INSTR   rax, QWORD PTR [r8] ; adc/sbb with carry
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8,  [r8  + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     small_loop

before_unrolled:

    setc    al                  ; save carry into AL
    test    r9,  r9             ; check if any 4-digit blocks
    bt      ax,  0              ; restore CF from AL (doesn't touch ZF)
    jz      end_of_func         ; exit if no more work

ALIGN 16
big_loop:

i = 0
WHILE i LT 4
    mov     rax, QWORD PTR [rdx + i * 8]
    INSTR   rax, QWORD PTR [r8 + i * 8]
    mov     QWORD PTR [rcx + i * 8], rax
    i = i + 1
ENDM

    lea     rdx, [rdx + 32]
    lea     r8,  [r8  + 32]
    lea     rcx, [rcx + 32]
    dec     r9
    jnz     big_loop

ALIGN 16
end_of_func:

    setc    al                  ; return carry in AL
    movzx   rax, al             ; zero-extend to RAX
    ret

OP&_n_zen4 ENDP

ENDM

GEN_ADD_SUB_N_X64 MACRO OP, INSTR

OP&_n_x64 PROC FRAME
.endprolog

    test    r9,  r9             ; if size == 0 → exit
    jz      end_of_func

main_loop:

    mov     rax, QWORD PTR [rdx] 
    INSTR   rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8,  [r8  + 8]
    lea     rcx, [rcx + 8]
    dec     r9
    jnz     main_loop

ALIGN 16
end_of_func:

    setc    al
    movzx   rax, al
    ret

OP&_n_x64 ENDP

ENDM

; ---------------------------------------------------------------------------
; Instantiations
; ---------------------------------------------------------------------------

; Zen4 optimized versions (unrolled)
GEN_ADD_SUB_N_ZEN4 add, adc
GEN_ADD_SUB_N_ZEN4 sub, sbb

; Generic x64 versions
GEN_ADD_SUB_N_X64 add, adc
GEN_ADD_SUB_N_X64 sub, sbb

ADD_SUB_N ENDS

END