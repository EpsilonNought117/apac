
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                 BALANCED SUBTRACTION FUNCS (N LIMBS)                      |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

SUB_N SEGMENT ALIGN(64) 'CODE'

    option casemap:none

    ;   Microsoft ABI 

    ;   Function Arguments
    ;
    ;   rcx -> result       (ap_seg_t*)   
    ;   rdx -> op1          (const ap_seg_t*)
    ;   r8  -> op2          (const ap_seg_t*)
    ;   r9  -> size         (ap_size_t)

; Procedure tuned for optimal performance
; on AMD Zen4 microarchitecture

sub_n_zen4 PROC FRAME
.endprolog

    xor     rax, rax
    mov     r11, r9
    shr     r9,  2  ; size /= 4
    and     r11, 3  ; size %= 4
    jz      before_unrolled

    ; runs at most thrice

small_loop:

    mov     rax, QWORD PTR [rdx]
    sbb     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8,  [r8  + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     small_loop

before_unrolled:
    
    setc    al          ; for borrow propagation
    test    r9,  r9     ; test if unrolled size is zero
    bt      ax,  0      ; set carry (borrow) bit via a bit-test of the LSB of ax
    jz      end_of_func     

ALIGN 16
big_loop:

i = 0
WHILE i LT 4
    mov     rax, QWORD PTR [rdx + i * 8]
    sbb     rax, QWORD PTR [r8  + i * 8]
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

    setc    al
    movzx   rax, al
    ret

sub_n_zen4 ENDP

; generic lowest common denominator x64 
; procedure for balanced subtraction

sub_n_x64 PROC FRAME
.endprolog

    test    r9,  r9
    jz      end_of_func

main_loop:

    mov     rax, QWORD PTR [rdx]
    sbb     rax, QWORD PTR [r8]
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

sub_n_x64 ENDP

END