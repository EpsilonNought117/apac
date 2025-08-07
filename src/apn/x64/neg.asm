
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |						    NEGATION FUNCTIONS    							|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> size (u64)

; Negations (2's Complement) procedure tuned for
; AMD's Zen4 Microarchitecture with 4x unroll

neg_zen4 PROC FRAME

.pushframe
.endprolog

    mov     r11, r8
    shr     r8,  2
    and     r11, 3
    jz      unroll_loop_outer

small_loop:

    mov     rax, 0
    mov     r9, QWORD PTR [rdx]
    sbb     rax, r9
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
    
    mov     rax, 0
    mov     r9, QWORD PTR [rdx]
    sbb     rax, r9
    mov     QWORD PTR [rcx], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 8]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 8], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 16]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 16], rax

    mov     rax, 0
    mov     r9, QWORD PTR [rdx + 24]
    sbb     rax, r9
    mov     QWORD PTR [rcx + 24], rax

    lea     rdx, [rdx + 32]
    lea     rcx, [rcx + 32]
    dec     r8
    jnz     main_loop

ALIGN 16
end_of_func:

    ; discard any carry or borrow if generated

    ret

neg_zen4 ENDP

; lowest common denominator generic implementation

neg_x64 PROC FRAME
.pushframe
.endprolog

    test    r8,  r8
    jz     end_of_func

ALIGN 16
small_loop:

    mov     rax, 0
    mov     r9, QWORD PTR [rdx]
    sbb     rax, r9
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r8
    jnz     small_loop

end_of_func:

    ; no need to return the borrow as this is
    ; 2's complement negation

    ret

neg_x64 ENDP

END