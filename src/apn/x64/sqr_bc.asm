
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                        BASECASE SQUARING FUNCTIONS                        |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

.code

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (apn_seg*)   
    ;   rdx -> op1 (const apn_seg*)
    ;   r8  -> size (apn_size)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------


sqr_bc_zen4 PROC FRAME

    ret

sqr_bc_zen4 ENDP

;   -------------------------
;
;           MUL/ADC        
;
;   -------------------------

; generic x64 fallback routine

sqr_bc_x64 PROC FRAME

    push    rbx
.pushreg    rbx
    push    rsi
.pushreg    rsi
    push    rdi
.pushreg    rdi
    push    r12
.pushreg    r12    
    push    r13
.pushreg    r13
    push    r14
.pushreg    r14
    push    r15
.pushreg    r15
.pushframe
.endprolog

    ;   5-limb bignum squaring matrix example with the 
    ;   symmetric non-diagonal terms marked as DUP
    ;
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |     |    a0     |    a1     |    a2     |    a3     |    a4     |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   | a0  |   a0*a0   |    DUP    |    DUP    |    DUP    |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   | a1  |  2*a1*a0  |   a1*a1   |    DUP    |    DUP    |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   | a2  |  2*a2*a0  |  2*a2*a1  |   a2*a2   |    DUP    |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   | a3  |  2*a3*a0  |  2*a3*a1  |  2*a3*a2  |   a3*a3   |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   | a4  |  2*a4*a0  |  2*a4*a1  |  2*a4*a2  |  2*a4*a3  |   a4*a4   |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+

    ; the whole function works in 3 Passes

    ; Pass 1: Accumulate non-diagonal products in the result
    ;       
    ;   For an (n * n) sized squaring operation, there will be exactly (n * (n - 1)) / 2
    ;   number of non-diagonal multiplications since those products occur twice in the 
    ;   multiplication matrix. In pass 1, add all those sums once
    ;
    ; Pass 2: Shift left the accumulated non-diag prods by 1 bit
    ;   
    ;   Shift the accumulated products left by 1 bit, essentially multiplying 
    ;   the whole lower-triangular part of the matrix by 2.
    ;
    ; Pass 3: Accumulate the diagonal products (squares) in the result
    ;
    ;   These squares only need to be added once as shown along the diagonal.

    xchg    rdi, rcx        ; exchange rcx and rdi, freeing up rcx for jrcxz/loop
    xchg    rsi, rdx        ; exchange rdx and rsi, freeing up rdx for mul
    mov     r9,  r8         ; copy of size in r9
    dec     r9              ; lower triangular matrix elems start with
                            ; (n - 1) -> (n - 2) -> ... -> 2 -> 1 -> 0
    test    r9,  r9         ; test if the basecase sqr size is 1, then no double prods
    jz      before_pass3    ; do the only diagonal sqr prod

    ; PASS-1 (O(n^2) step)

    ; rdx:rax for accumulating the product via mul
    ; r10 <- result (copy for loop)
    ; r11 <- op1    (copy for loop)
    ; r9 <- (size - 1) (copy for loop)

outer_loop_pass1:

    mov     rcx,  r9    ; rcx is inner loop counter
    xor     rbx, rbx    ; temp_reg
    mov     r12, 

inner_loop_pass1:

end_of_pass1:


    ; PASS-2 (O(n) step)

    ; PASS-3 (O(n) step)

ALIGN 16
end_of_func:

    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rdi
    pop     rsi
    pop     rbx
    ret

sqr_bc_x64 ENDP

END