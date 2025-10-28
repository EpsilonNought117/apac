
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                      BASECASE MULTIPLICATION FUNCTIONS                    |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

MUL_BASECASE SEGMENT ALIGN(64) 'CODE'

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result           (apn_seg_t*)   
    ;   rdx -> op1              (const apn_seg_t*)
    ;   r8  -> op2              (const apn_seg_t*)
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

; Basecase multiplication procedure optimized for 
; AMD Zen4 Microarchitecture & unrolled 4x

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

    jmp     start_of_func

ALIGN 16
jump_table:

    QWORD offset outer_loop_end
    QWORD offset rem1
    QWORD offset rem2
    QWORD offset rem3
    QWORD offset rem4
    QWORD offset rem5
    QWORD offset rem6
    QWORD offset rem7

start_of_func:

    xchg    rbp, rcx
    xchg    rbx, rdx
    mov     r10, QWORD PTR [rsp + 88]
    mov     r11, r9
    shr     r9,  3
    mov     rax, r9
    shl     rax, 6
    and     r11, 7
    mov     r13, r11
    lea     r12, jump_table
    lea     r12, [r12 + r11*8]

outer_loop_start:

    mov     r11, QWORD PTR [rbp]
    mov     rcx, r9
    mov     rdx, QWORD PTR [r8]
    test    rcx, rcx
    jz      before_remainder

ALIGN 64
inner_loop_unrolled:

FOR i, <0, 1, 2, 3, 4, 5, 6, 7>

    mulx    rdi, rsi, QWORD PTR [rbx + i*8]
    adcx    rsi, r11
    adox    rdi, QWORD PTR [rbp + i*8 + 8]
    mov     QWORD PTR [rbp + i*8], rsi
    mov     r11, rdi

ENDM

    lea     rbx, [rbx + 64]
    lea     rbp, [rbp + 64]
    lea     rcx, [rcx - 1]
ALIGN 16
    jrcxz   before_remainder
    jmp     inner_loop_unrolled

ALIGN 32
before_remainder:

    jmp     QWORD PTR [r12]

FOR outer, <7, 6, 5, 4, 3, 2, 1>

ALIGN 32
rem&outer&:

    i = 0
    WHILE i LT outer
        mulx    rdi, rsi, QWORD PTR [rbx + i*8]
        adcx    rsi, r11
        adox    rdi, QWORD PTR [rbp + i*8 + 8]
        mov     QWORD PTR [rbp + i*8], rsi
        mov     r11, rdi
            
    i = i + 1
    ENDM
    jmp outer_loop_end
        
ENDM
  
outer_loop_end:

    adc     r11, 0
    mov     QWORD PTR [rbp + r13*8], r11
    
    add     r8,  8
    sub     rbx, rax
    sub     rbp, rax
    add     rbp, 8

    dec     r10
    jnz     outer_loop_start

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
    push    r12
.pushreg    r12
    push    r13
.pushreg    r13
.endprolog

    xchg    rbx, rdx    ; high64 in rdx
    xor     r10, r10    ; i

    ; now rbx contains op1 (const u64*)

outer_loop:

    xor     rdi, rdi        ; temp_reg
    xor     r11, r11        ; j
    xor     rdx, rdx        ; high64
    mov     r12, r10        ; r12 is indexer for result
    mov     r13, r9         ; load size1 into r13
    mov     rsi, QWORD PTR [r8 + r10*8]  ; op2[i]
    mov     rax, rsi

inner_loop:

    adc     rdi, rdx                 ; temp_reg += (CF + high64)
    mul     QWORD PTR [rbx + r11*8]  ; rdx:rax = rax * op1[j]

    add     rdi, rax                 ; temp_reg += low64
    adc     rdx, 0                   ; high64 += CF
    add     QWORD PTR [rcx + r12*8], rdi    ; result[i + j] += temp_reg

    mov     rax, rsi        ; load op2[i] once again into rax for next iter
    mov     rdi, 0          ; zero out temp_reg
    inc     r11
    inc     r12
    dec     r13
    jnz     inner_loop

loop_end:

    adc     QWORD PTR [rcx + r12*8], rdx
    inc     r10
    mov     rsi, QWORD PTR [rsp + 80]   ; load size2 into rsi
    cmp     rsi, r10        ; check if (rsi - r10) != 0 (means rsi > r10 for unsigned)
    jnz     outer_loop

end_of_func:

    pop     r13
    pop     r12
    pop     rsi
    pop     rdi
    pop     rbx
    ret     0

mul_bc_x64 ENDP

END