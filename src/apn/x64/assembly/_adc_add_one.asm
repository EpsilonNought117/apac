.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> size (u64)
    ;   r9  -> val (u64)

extern apn_cpy:PROC

_adc_add_one PROC FRAME
.pushframe
    push    rbp
.pushreg    rbp
    mov     rbp, rsp
.endprolog
    
    ; at this point, rsp is aligned at 16-byte boundary
    ; because during function entry, it is at 8-byte boundary
    ; pushing rbp sets it to a 16-byte boundary
    ; as per x64 calling convention of Microsoft ABI

    xor     rax, rax    ; accumulator
    xor     r10, r10    ; counter/indexer
    mov     r11, r8     ; temp_size

    mov     rax, QWORD PTR [rdx + r10*8]
    add     rax, r9                         ; add val
    mov     QWORD PTR [rcx + r10*8], rax
    inc     r10
    dec     r11
    jz      end_of_func

propagate_carry:

    jnc     copy_remaining
    mov     rax, QWORD PTR [rdx + r10*8]
    adc     rax, 0
    mov     QWORD PTR [rcx + r10*8], rax

    inc     r10
    dec     r11
    jnz     propagate_carry
    jmp     end_of_func

copy_remaining:

    ; if this part is entered, that means there is no carry remaining
    ; rsp aligned at 16-byte boundary
    ; allocate shadow space

    sub     rsp, 32                 
    
    lea     rcx, [rcx + r10*8]
    lea     rdx, [rdx + r10*8]
    mov     r8, r11

    call    apn_cpy
    
    add     rsp, 32
    clc                 

end_of_func:

    setc    al
    
    mov     rsp, rbp
    pop     rbp
    ret
_adc_add_one ENDP

END