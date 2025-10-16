
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                   BALANCED ADDITION FUNCS (N LIMBS)                       |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

    option casemap:none

    ;   Microsoft ABI 

    ;   Function Arguments
    ;
    ;   rcx -> result (apn_seg_t*)   
    ;   rdx -> op1 (const apn_seg_t*)
    ;   r8  -> op2 (const apn_seg_t*)
    ;   r9  -> size (apn_size_t)

; Procedure tuned for optimal performance
; on AMD Zen4 microarchitecture

add_n_zen4 PROC FRAME
.endprolog

    xor     rax, rax
    mov     r11, r9
    shr     r9,  2  ; size /= 4
    and     r11, 3  ; size %= 4
    jz      before_unrolled

    ; runs at most thrice

small_loop:

    mov     rax, QWORD PTR [rdx] 
    adc     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8, [r8 + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     small_loop
    
before_unrolled:

    setc    al          ; for carry propagation
    test    r9,  r9     ; test if unrolled size is zero
    bt      ax,  0      ; bit-test doesn't modify zero flag
    jz      end_of_func     

ALIGN 16
big_loop:

    mov     rax, QWORD PTR [rdx]
    adc     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    mov     rax, QWORD PTR [rdx + 8]
    adc     rax, QWORD PTR [r8 + 8]
    mov     QWORD PTR [rcx + 8], rax

    mov     rax, QWORD PTR [rdx + 16]
    adc     rax, QWORD PTR [r8 + 16]
    mov     QWORD PTR [rcx + 16], rax

    mov     rax, QWORD PTR [rdx + 24]
    adc     rax, QWORD PTR [r8 + 24]
    mov     QWORD PTR [rcx + 24], rax

    lea     rdx, [rdx + 32]
    lea     r8, [r8 + 32]
    lea     rcx, [rcx + 32]
    dec     r9
    jnz     big_loop

ALIGN 16
end_of_func:

    setc    al
    movzx   rax, al
    ret

add_n_zen4 ENDP

; generic lowest common denominator x64 
; procedure for balanced addition

add_n_x64 PROC FRAME
.endprolog

main_loop:

    mov     rax, QWORD PTR [rdx] 
    adc     rax, QWORD PTR [r8]
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     r8, [r8 + 8]
    lea     rcx, [rcx + 8]
    dec     r9
    jnz     main_loop

ALIGN 16
end_of_func:

    setc    al
    movzx   rax, al
    ret

add_n_x64 ENDP

END