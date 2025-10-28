
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                   ADD SINGLE-LIMB TO APN-ARR FUNCTIONS                    |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

ADD_N_ONE SEGMENT ALIGN(64) 'CODE'

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result       (apn_seg_t*)   
    ;   rdx -> op1          (const apn_seg_t*)
    ;   r8  -> size         (apn_size_t)
    ;   r9  -> val          (apn_seg_t)

add_n_one_zen4 PROC FRAME
.endprolog

    ; assumes r8 is at least 1
    ; via APAC_ASSERT in caller function

    mov     rax, QWORD PTR [rdx]
    add     rax, r9
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
    adc     rax, 0
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

ALIGN 64
loop_unrolled:

i = 0
WHILE i LT 4
    mov     rax, QWORD PTR [rdx + i*8]
    adc     rax, 0
    mov     QWORD PTR [rcx + i*8], rax
        
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

add_n_one_zen4 ENDP

; Generic x64 routine

add_n_one_x64 PROC FRAME
.endprolog

    mov     rax, QWORD PTR [rdx]
    add     rax, r9
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r8
    jz      end_of_func

main_loop:

    mov     rax, QWORD PTR [rdx]
    adc     rax, 0
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r8
    jnz     main_loop

end_of_func:

    setc    al
    movzx   rax, al
    ret

add_n_one_x64 ENDP

END