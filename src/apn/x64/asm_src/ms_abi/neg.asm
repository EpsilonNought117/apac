
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |						    NEGATION FUNCTIONS    							|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

NEGATE SEGMENT ALIGN(64) 'CODE'

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result       (apn_dig_t*)   
    ;   rdx -> op1          (const apn_dig_t*)
    ;   r8  -> size         (apn_size_t)

; Negations (2's Complement) procedure tuned for
; AMD's Zen4 Microarchitecture with 4x unroll

neg_x64 PROC FRAME
.endprolog

    mov     r11, r8
    shr     r8,  2
    and     r11, 3
    test    r11, r11
    stc     
    jz      unroll_loop_outer

small_loop:

    mov     rax, QWORD PTR [rdx]
    not     rax
    adc     rax, 0
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     small_loop

unroll_loop_outer:

    setc    al
    test    r8, r8
    bt      ax, 0
    jz      end_of_func

main_loop:
    
i = 0
    
WHILE i LT 4

    mov     rax, QWORD PTR [rdx + i * 8]
    not     rax
    adc     rax, 0
    mov     QWORD PTR [rcx + i * 8], rax
        
    i = i + 1
ENDM

    lea     rdx, [rdx + 32]
    lea     rcx, [rcx + 32]
    dec     r8
    jnz     main_loop

ALIGN 16
end_of_func:

    ; discard any carry or borrow if generated

    ret

neg_x64 ENDP

NEGATE ENDS

END