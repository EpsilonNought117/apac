
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                      BASECASE MULTIPLICATION FUNCTIONS                    |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

MUL_BASECASE SEGMENT ALIGN(64) 'CODE'

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result           (apn_dig_t*)   
    ;   rdx -> op1              (const apn_dig_t*)
    ;   r8  -> op2              (const apn_dig_t*)
    ;   r9  -> size1            (apn_size_t)
    ;   [rsp + 40] -> size2     (apn_size_t) (where rsp is initially at function entering)

    ;   ASSUMPTION
    ;   (size1 >= size2)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

; The fastest procedures utilizing ADX and BMI2 x64 ISA extensions

; 8x unroll adx/bmi2 variant with jump table

mul_bc_zen4 PROC FRAME

    push    rbp
.pushreg    rbp
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
.endprolog

start_of_func:

    xchg    rbp, rcx    ; rbp = result_ptr
    xchg    rbx, rdx    ; rbx = op1_ptr

    ; r10 = size2
    mov     r10, QWORD PTR [rsp + 88]

    mov     r11, r9     ; r11 = size1
    mov     r12, r9     ; r12 = size1
    mov     r13, r9     ; r13 = size1

    shr     r9,  3      ; r9  /= 8
    and     r11, 7      ; r11 %= 8
    shl     r12, 3      ; r12 *= 8

pass1_start:

    mov     rdx, QWORD PTR [r8]
    xor     rax, rax
    
    mov     rcx, r9
    test    r9,  r9
    jz      pass1_bef_rmdr

ALIGN 16
pass1_loop_unroll:

FOR i, <0, 1, 2, 3, 4, 5, 6, 7>

    mulx    rdi, rsi, QWORD PTR [rbx + i * 8]
    adc     rsi, rax
    mov     QWORD PTR [rbp + i * 8], rsi
    mov     rax, rdi

ENDM

    lea     rbx, [rbx + 64]
    lea     rbp, [rbp + 64]
    dec     rcx
    jnz     pass1_loop_unroll

pass1_bef_rmdr:

    mov     rcx, r11
    jrcxz   pass1_end

ALIGN 16
pass1_loop_rmdr:

    mulx    rdi, rsi, QWORD PTR [rbx]
    adc     rsi, rax
    mov     QWORD PTR [rbp], rsi
    mov     rax, rdi

    lea     rbx, [rbx + 8]
    lea     rbp, [rbp + 8]
    dec     rcx
    jnz     pass1_loop_rmdr

pass1_end:

    adc     rax, 0
    mov     QWORD PTR [rbp], rax

    sub     rbx, r12
    sub     rbp, r12
    add     rbp, 8
    add     r8,  8

    dec     r10
    jz      end_of_func

bef_pass2:

    dec     r13
    sub     r12, 8

    mov     r9,  r13
    mov     rax, r13
    
    shr     r9,  3
    and     rax, 7
    lea     r11, offset jump_table
    lea     r11, [r11 + rax * 8]
    
    ; now r11 contains the proper
    ; remainder size label offset
    ; from the jump table

ALIGN 16
pass2_outer_loop_start:

    mov     rax, QWORD PTR [rbp]
    mov     rdx, QWORD PTR [r8]
    mov     rcx, r9
    test    r9,  r9
    jz      pass2_bef_rmdr

ALIGN 16
pass2_inner_loop_unroll:

FOR i, <0, 1, 2, 3, 4, 5, 6, 7>

    mulx    rdi, rsi, QWORD PTR [rbx + i * 8]
    adcx    rsi, rax
    adox    rdi, QWORD PTR [rbp + i * 8 + 8]
    mov     QWORD PTR [rbp + i * 8], rsi
    mov     rax, rdi

ENDM

    lea     rbx, [rbx + 64]
    lea     rbp, [rbp + 64]
    lea     rcx, [rcx - 1]
    jrcxz   pass2_bef_rmdr
    jmp     pass2_inner_loop_unroll

ALIGN 16
pass2_bef_rmdr:

    jmp     QWORD PTR [r11]

ALIGN 16
jump_table:

    QWORD offset pass2_outer_loop_end
    QWORD offset rem1
    QWORD offset rem2
    QWORD offset rem3
    QWORD offset rem4
    QWORD offset rem5
    QWORD offset rem6
    QWORD offset rem7

FOR outer, <7, 6, 5, 4, 3, 2, 1>

ALIGN 16
rem&outer&:

i = 0
WHILE i LT outer

    mulx    rdi, rsi, QWORD PTR [rbx + i * 8]
    adcx    rsi, rax
    adox    rdi, QWORD PTR [rbp + i * 8 + 8]
    mov     QWORD PTR [rbp + i * 8], rsi
    mov     rax, rdi
            
i = i + 1
ENDM

    lea     rbx, [rbx + outer * 8]
    lea     rbp, [rbp + outer * 8]
    jmp     pass2_outer_loop_end
        
ENDM
  
pass2_outer_loop_end:

    mulx    rdi, rsi, QWORD PTR [rbx]
    adcx    rsi, rax
    adox    rdi, rcx        ; rcx is zero now

    mov     QWORD PTR [rbp], rsi
    adc     rdi, 0
    mov     QWORD PTR [rbp + 8], rdi

    sub     rbp, r12
    sub     rbx, r12
    add     rbp, 8
    add     r8,  8

    dec     r10
    jnz     pass2_outer_loop_start

end_of_func:

    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    pop     rbp
    ret

mul_bc_zen4 ENDP

;   -------------------------
;
;            MUL/ADC        
;
;   -------------------------

; Lowest common denominator x64 multiplication routine
; Not particularly optimized

mul_bc_x64 PROC FRAME

    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
.endprolog

start_of_func:

    xchg    rbx, rdx                    ; op1 now in rbx
    mov     r10, QWORD PTR [rsp + 64]   ; size2 in r10 at 5th arg position

outer_loop:

    xor     rdi, rdi    ; temp_reg
    xor     rdx, rdx    ; high64
    mov     r11, r9     ; size1 copy in r11
    mov     rsi, QWORD PTR [r8] ; op2[i]
    mov     rax, rsi

ALIGN 16
inner_loop:

    mul     QWORD PTR [rbx] ; rdx : rax = rax * op1[i]

    ; now product in rdx:rax
    ; rax = low64
    ; rdx = high64

    add     rdi, rax                ; temp_reg += low64
    adc     rdx, 0                  ; high64 += CF
    add     QWORD PTR [rcx], rdi    ; result[i + j] += temp_reg

    mov     rax, rsi                ; restore rax for next mul
    mov     rdi, rdx
    adc     rdi, 0

    lea     rbx, [rbx + 8]          ; update ptrs
    lea     rcx, [rcx + 8]          
    dec     r11
    jnz     inner_loop

outer_loop_end:

    mov     QWORD PTR [rcx], rdi

    mov     r11, r9
    shl     r11, 3
    sub     rbx, r11
    sub     rcx, r11
    add     r8,  8
    add     rcx, 8
    dec     r10
    jnz     outer_loop

end_of_func:

    pop     rsi
    pop     rdi
    pop     rbx
    ret

mul_bc_x64 ENDP

MUL_BASECASE ENDS

END