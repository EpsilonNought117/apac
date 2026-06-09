
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |		         APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS   			|
;   |                                                                           |
;   O---------------------------------------------------------------------------O

ADDMUL_ONE SEGMENT ALIGN(64) 'CODE'
	
    option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result       (ap_dig_t*)   
    ;   rdx -> op1          (const ap_dig_t*)
    ;   r8  -> size         (ap_size_t)
    ;   r9  -> val          (ap_dig_t)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

addmul_one_zen4 PROC FRAME
    
    push    rbx
.pushreg    rbx
.endprolog

start_of_func:

    xchg    rax, rcx    ; empty rcx for storing counter
    xchg    rbx, rdx    ; empty rdx for mulx
    mov     rdx, r9     ; store val in rdx
    mov     rcx, r8     ; copy of size in rcx
    and     r8,  7      ; size % 8
    shr     rcx, 3      ; size / 8

    mov     r9,  rax            ; store result_ptr in r9
    mov     rax, QWORD PTR [r9] ; load result[0] into rax
    test    rcx, rcx            ; test if (size / 8) == 0
    jz      before_remainder    ; jump to rmdr if zero
    
ALIGN 64
unroll8_loop:

i = 0
WHILE i LT 8

    mulx    r11, r10, QWORD PTR [rbx + i * 8]
    adcx    r10, rax
    adox    r11, QWORD PTR [r9 + i * 8 + 8]
    mov     QWORD PTR [r9 + i * 8], r10
    mov     rax, r11

i = i + 1
ENDM

    lea     rbx, [rbx + 64]
    lea     r9,  [r9  + 64]
    lea     rcx, [rcx - 1]
    jrcxz   before_remainder
    jmp     unroll8_loop

ALIGN 16
before_remainder:

    mov     rcx, r8
    jrcxz   end_of_loop

ALIGN 64
rmdr_loop:

    mulx    r11, r10, QWORD PTR [rbx]
    adcx    r10, rax
    adox    r11, QWORD PTR [r9 + 8]
    mov     QWORD PTR [r9], r10
    mov     rax, r11

    lea     rbx, [rbx + 8]
    lea     r9,  [r9  + 8]
    loop    rmdr_loop

ALIGN 32
end_of_loop:
    
    adcx    rax, rcx            ; rcx is zero by now
    mov     QWORD PTR [r9], rax ; r9 points to &result[size]

end_of_func:

    seto    al
    movzx   rax, al

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

    xchg    r10, rcx    ; free up rcx for temp_reg
    xchg    r11, rdx    ; free up rdx for mul

    ; r10 <- result
    ; r11 <- op1
    ; r8  <- size
    
    ; rcx is temp_reg

    xor     rdx, rdx
    xor     rcx, rcx
    mov     rax, r9
    test    r8,  r8
    jz      end_of_func

main_loop:

    mul     QWORD PTR [r11] ; rdx:rax = rax * op1[idx]

    add     rcx, rax
    adc     rdx, 0
    add     QWORD PTR [r10], rcx

    mov     rcx, rdx
    mov     rax, r9         ; restore clobbered rax
    adc     rcx, 0          ; temp_reg += (CF + high64)

    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    dec     r8
    jnz     main_loop

end_of_loop:

    adc     QWORD PTR [r10], rcx

end_of_func:

    setc    al
    movzx   rax, al
    ret   

addmul_one_x64 ENDP

ADDMUL_ONE ENDS

END