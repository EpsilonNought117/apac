
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |		         APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS   			|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

ADDMUL_ONE SEGMENT ALIGN(64) 'CODE'
	
    option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result       (apn_seg_t*)   
    ;   rdx -> op1          (const apn_seg_t*)
    ;   r8  -> size         (apn_size_t)
    ;   r9  -> val          (apn_seg_t)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

addmul_one_zen4 PROC FRAME
    
    push    rbx
.pushreg    rbx
    push    rbp
.pushreg    rbp
.endprolog

jmp     start_of_func

ALIGN 16
jump_table:

    QWORD offset end_of_loop
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
    mov     rdx, r9
    mov     rcx, r8
    and     r8,  7
    shr     rcx, 3

    lea     r9,  offset jump_table
    lea     r9,  [r9 + r8 * 8]
    mov     rax, QWORD PTR [rbp]
    test    rcx, rcx
    jz      before_remainder
    
ALIGN 64
unroll8_loop:

i = 0
WHILE i LT 8

    mulx    r11, r10, QWORD PTR [rbx + i * 8]
    adcx    r10, rax
    adox    r11, QWORD PTR [rbp + i * 8 + 8]
    mov     QWORD PTR [rbp + i * 8], r10
    mov     rax, r11
        
    i = i + 1
ENDM

    lea     rbx, [rbx + 64]
    lea     rbp, [rbp + 64]
    lea     rcx, [rcx - 1]
    jrcxz   before_remainder
    jmp     unroll8_loop

ALIGN 16
before_remainder:

    jmp     QWORD PTR [r9]

FOR outer, <7, 6, 5, 4, 3, 2, 1>

ALIGN 16
rem&outer&:

i = 0
WHILE i LT outer
    mulx    r11, r10, QWORD PTR [rbx + i*8]
    adcx    r10, rax
    adox    r11, QWORD PTR [rbp + i*8 + 8]
    mov     QWORD PTR [rbp + i*8], r10
    mov     rax, r11
            
i = i + 1
ENDM

    jmp end_of_loop
        
ENDM

ALIGN 32
end_of_loop:
    
    adcx    rax, rcx
    mov     QWORD PTR [rbp + r8 * 8], rax

end_of_func:

    seto    al
    movzx   rax, al

    pop     rbp
    pop     rbx
    ret

addmul_one_zen4 ENDP

;   -------------------------
;
;            MUL/ADC        
;
;   -------------------------

addmul_one_x64 PROC FRAME
.endprolog

    xchg    r10, rcx    ; free up rcx for jrcxz/loop
    xchg    r11, rdx    ; free up rdx for mul

    ; r10 <- result
    ; r11 <- op1
    ; r8  <- size
    
    ; rbx is temp_reg

    xor     rdx, rdx
    xor     rcx, rcx
    test    r8,  r8
    jz      end_of_func

main_loop:
    
    mov     rax, r9         ; restore clobbered rax
    adc     rcx, rdx          ; temp_reg += (CF + high64)
    mul     QWORD PTR [r11] ; rdx:rax = rax * op1[idx]

    add     rcx, rax
    adc     rdx, 0
    add     QWORD PTR [r10], rcx

    mov     rcx, 0

    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    dec     r8
    jnz     main_loop

end_of_loop:

    adc     QWORD PTR [r10], rdx

end_of_func:

    setc    al
    movzx   rax, al
    ret   

addmul_one_x64 ENDP

END