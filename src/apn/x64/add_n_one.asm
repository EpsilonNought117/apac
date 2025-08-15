
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                   UNBALANCED ADDITION HELPER FUNCTION                     |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (apn_seg*)   
    ;   rdx -> op1 (const apn_seg*)
    ;   r8  -> size (apn_size)
    ;   r9  -> val (apn_seg)

; apn_cpy needed for copying the rest of the limbs as is once carry becomes zero

extern apn_cpy:PROC

; This function is not a performance bottleneck usually in practice.
; Therefore only one common x64 implementation suffices for now.

add_n_one_x64 PROC FRAME
.pushframe          
    push    rbp
.pushreg    rbp     
    mov     rbp, rsp
.setframe   rbp, 0  
.endprolog

    ; at this point, rsp is aligned at 16-byte boundary
    ; because during function entry, it is at 8-byte boundary
    ; pushing rbp sets it to a 16-byte boundary
    ; as per x64 calling convention of Microsoft ABI

    mov     r11, r8     ; temp_size

    mov     rax, QWORD PTR [rdx]
    add     rax, r9                         ; add val
    mov     QWORD PTR [rcx], rax
    
    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jz      end_of_func

propagate_carry:

    jnc     copy_remaining
    mov     rax, QWORD PTR [rdx]
    adc     rax, 0
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     propagate_carry
    jmp     end_of_func

copy_remaining:

    ; if this part is entered, that means there is no carry remaining
    ; rsp aligned at 16-byte boundary
    ; allocate shadow space and call the apn_cpy func

    sub     rsp, 32  
    mov     r8, r11
    call    apn_cpy     ; call the apn_cpy func
    add     rsp, 32

ALIGN 16
end_of_func:

    setc    al
    movzx   rax, al
    mov     rsp, rbp
    pop     rbp
    ret

add_n_one_x64 ENDP

END