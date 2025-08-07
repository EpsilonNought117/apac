
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                 UNBALANCED SUBTRACTION HELPER FUNCTION                    |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

    ;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> size (u64)
    ;   r9  -> val (u64)

extern apn_cpy:PROC

sub_n_one_x64 PROC FRAME

    push    rbp
.pushreg    rbp
    mov     rbp, rsp
.pushframe
.endprolog
    
    ; at this point, rsp is aligned at 16-byte boundary
    ; because during function entry, it is at 8-byte boundary
    ; pushing rbp sets it to a 16-byte boundary
    ; as per x64 calling convention of Microsoft ABI

    xor     r10, r10    ; counter/indexer
    mov     r11, r8     ; temp_size

    mov     rax, QWORD PTR [rdx]
    sub     rax, r9                         ; sub val
    mov     QWORD PTR [rcx], rax
    
    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jz      end_of_func

propagate_carry:

    jnc     copy_remaining
    mov     rax, QWORD PTR [rdx]
    sbb     rax, 0
    mov     QWORD PTR [rcx], rax

    lea     rdx, [rdx + 8]
    lea     rcx, [rcx + 8]
    dec     r11
    jnz     propagate_carry
    jmp     end_of_func

copy_remaining:

    ; if this part is entered, that means there is no carry remaining
    ; rsp aligned at 16-byte boundary
    ; allocate shadow space

    sub     rsp, 32
    mov     r8, r11

    call    apn_cpy
    
    add     rsp, 32                

ALIGN 16
end_of_func:

    setc    al
    movzx   rax, al    
    mov     rsp, rbp
    pop     rbp
    ret

sub_n_one_x64 ENDP

END