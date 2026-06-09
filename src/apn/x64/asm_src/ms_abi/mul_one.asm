
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
;           MULX/ADC       
;
;   -------------------------

mul_one_zen4 PROC FRAME
    
    push    rbx
.pushreg    rbx
.endprolog

start_of_func:

    xchg    rbx, rdx    ; rdx now has op1
    mov     rdx, r9     ; load val into rdx
    mov     r9,  r8     ; store copy of size in r9    
    and     r9,  7      ; r9 = size % 8
    shr     r8,  3      ; r8 = size / 8

    xor     rax, rax
    test    r8,  r8     ; test if (size / 8) == 0
    jz      bef_rmdr    ; jump to before rmdr loop

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

    setc    r8b
    test    r9,  r9
    bt      r8b, 0
    jz      end_of_func

rmdr_loop:

    mulx    r11, r10, QWORD PTR [rbx]
    adc     r10, rax
    mov     QWORD PTR [rcx], r10
    mov     rax, r11

    lea     rbx, [rbx + 8]
    lea     rcx, [rcx + 8]
    dec     r9
    jnz     rmdr_loop

end_of_func:
    
    adc     rax, 0
    mov     QWORD PTR [rcx], rax

    pop     rbx
    ret

mul_one_zen4 ENDP

;   -------------------------
;
;           MUL/ADC       
;
;   -------------------------

mul_one_x64 PROC FRAME
.endprolog

    xchg    r10, rcx    ; free up rcx for jrcxz/loop
    xchg    r11, rdx    ; free up rdx for mul

    ; r10 <- result
    ; r11 <- op1
    ; r8  <- size
    
    ; rcx is temp_reg

    xor     rdx, rdx
    xor     rcx, rcx
    mov     rax, r9     ; val in rax for mul
    test    r8,  r8
    jz      end_of_func

main_loop:

    mul     QWORD PTR [r11] ; rdx:rax = rax * op1[idx]

    add     rcx, rax
    adc     rdx, 0
    mov     QWORD PTR [r10], rcx

    mov     rcx, rdx
    mov     rax, r9         ; restore clobbered rax

    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    dec     r8
    jnz     main_loop

end_of_loop:

    mov     QWORD PTR [r10], rcx

end_of_func:

    ret   

mul_one_x64 ENDP

MUL_ONE ENDS

END