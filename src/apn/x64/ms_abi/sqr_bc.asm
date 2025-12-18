
;   O---------------------------------------------------------------------------O
;   |                                                                           |
;   |                        BASECASE SQUARING FUNCTIONS                        |
;   |                                                                           |
;   O---------------------------------------------------------------------------O

SQR_BASECASE SEGMENT ALIGN(64) 'CODE'

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result       (apn_seg_t*)   
    ;   rdx -> op1          (const apn_seg_t*)
    ;   r8  -> size         (apn_size_t)

;   -------------------------
;
;        MULX/ADOX/ADCX        
;
;   -------------------------

;   Optimized routine for AMD Zen 4

sqr_bc_zen4 PROC FRAME

    push    rbp
.pushreg    rbp
    push    rbx
.pushreg    rbx
    push    rdi
.pushreg    rdi
    push    rsi
.pushreg    rsi
    push    r13
.pushreg    r13
.endprolog

    jmp     start_of_func

jump_table_pass1:

    QWORD offset outer_loop_end_pass1
    QWORD offset inner_pass1_rem1
    QWORD offset inner_pass1_rem2
    QWORD offset inner_pass1_rem3
    QWORD offset inner_pass1_rem4
    QWORD offset inner_pass1_rem5
    QWORD offset inner_pass1_rem6
    QWORD offset inner_pass1_rem7

start_of_func:

    xchg    rbp, rcx        ; free up rcx for jrcxz/loop
    xchg    rbx, rdx        ; free up rdx for mul
    mov     r11, r8         ; copy of size in r11 to later restore from
    dec     r11             ; curr_size = size - 1
    jz      pass3

    mov     r10, r11
    shl     r10, 3          ; curr_size * sizeof(apn_seg_t)
    add     rbp, 8

outer_loop_pass1:

    mov     rdx, QWORD PTR [rbx]  
    mov     rax, QWORD PTR [rbp]   
    mov     rcx, r11
    mov     r9,  r11                ; for later finding the jump_table label
    shr     rcx, 3                  ; curr_size /= 8 (for 8x unrolled loop)
    and     r9,  7                  ; curr_size %= 8
    lea     r13, jump_table_pass1
    lea     r13, [r13 + r9 * 8]
    test    rcx, rcx
    jz      bef_inner_rmdr_pass1

ALIGN 64
inner_loop_unroll_pass1:

i = 0
WHILE i LT 8

    mulx    rdi, rsi, QWORD PTR [rbx + i * 8 + 8]
    adcx    rsi, rax
    adox    rdi, QWORD PTR [rbp + i * 8 + 8]
    mov     QWORD PTR [rbp + i * 8], rsi
    mov     rax, rdi
        
    i = i + 1
ENDM

    lea     rbx, [rbx + 64]
    lea     rbp, [rbp + 64]
    lea     rcx, [rcx - 1]
ALIGN 32
    jrcxz   bef_inner_rmdr_pass1
    jmp     inner_loop_unroll_pass1

ALIGN 32
bef_inner_rmdr_pass1:

    jmp     QWORD PTR [r13]

FOR i, <7, 6, 5, 4, 3, 2, 1>

ALIGN 32
inner_pass1_rem&i&:

j = 0
WHILE j LT i

    mulx    rdi, rsi, QWORD PTR [rbx + j * 8 + 8]
    adcx    rsi, rax
    adox    rdi, QWORD PTR [rbp + j * 8 + 8]
    mov     QWORD PTR [rbp + j * 8], rsi
    mov     rax, rdi

j = j + 1    
ENDM

    lea     rbp, [rbp + i * 8]
    lea     rbx, [rbx + i * 8]
    jmp     outer_loop_end_pass1

ENDM

outer_loop_end_pass1:

    adc     QWORD PTR [rbp], rax
    sub     rbx, r10
    sub     rbp, r10
    add     rbx, 8
    add     rbp, 16
    sub     r10, 8
    dec     r11
    jnz     outer_loop_pass1

pass2_start:

    mov     r11, r8
    dec     r11
    shl     r11, 3
    sub     rbx, r11

    ; now op1 ptr is back to it's original address
    ; from the start of the function

    shr     r11, 2
    mov     r9,  r11
    mov     rcx, r11
    and     r9,  3
    shr     rcx, 2
    test    rcx, rcx
    jz      before_rmdr_pass2

ALIGN 64
loop_unroll_pass2:

i = 0
WHILE i LT 4
    
    mov     r10, QWORD PTR [rbp - i * 8 - 8]
    shld    QWORD PTR [rbp - i * 8], r10, 1
        
    i = i + 1
ENDM

    lea     rbp, [rbp - 32]
    dec     rcx
    jnz     loop_unroll_pass2

ALIGN 16
before_rmdr_pass2:

    mov     rcx, r9
    jrcxz   pass2_end

ALIGN 32
loop_rmdr_pass2:

    mov     r10, QWORD PTR [rbp - 8]
    shld    QWORD PTR [rbp], r10, 1
    
    lea     rbp, [rbp - 8]
    dec     rcx
    jnz     loop_rmdr_pass2
    
pass2_end:

    shl     QWORD PTR [rbp], 1
    sub     rbp, 8

    ; now rbp is pointing to first seg of result_arr
    ; same with rbx pointing to first seg of op1

pass3:

    mov     rcx, r8
    mov     r9,  r8
    shr     rcx, 2
    and     r9,  3
    test    rcx, rcx
    jz      before_rmdr_pass3

ALIGN 16
loop_pass3:

i = 0    
WHILE i LT 4

    mov     rdx, QWORD PTR [rbx + i * 8]
    mulx    rdi, rsi, rdx
    adc     QWORD PTR [rbp + i * 16], rsi
    adc     QWORD PTR [rbp + i * 16 + 8], rdi
        
    i = i + 1
ENDM

    lea     rbp, [rbp + 64]
    lea     rbx, [rbx + 32]
    dec     rcx
    jnz     loop_pass3

ALIGN 16
before_rmdr_pass3:

    mov     rcx, r9
    jrcxz   end_of_func

rmdr_loop_pass3:

    mov     rdx, QWORD PTR [rbx]
    mulx    rdi, rsi, rdx
    adc     QWORD PTR [rbp], rsi
    adc     QWORD PTR [rbp + 8], rdi

    lea     rbp, [rbp + 16]
    lea     rbx, [rbx + 8]
    dec     rcx
    jnz     rmdr_loop_pass3

end_of_func:

    pop     r13
    pop     rsi
    pop     rdi
    pop     rbx
    pop     rbp
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
.endprolog

    ;   5-limb bignum squaring matrix example with the 
    ;   symmetric non-diagonal terms marked as DUP
    ;
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |     |    a0     |    a1     |    a2     |    a3     |    a4     |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a0 |   a0*a0   |    DUP    |    DUP    |    DUP    |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a1 |  2*a1*a0  |   a1*a1   |    DUP    |    DUP    |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a2 |  2*a2*a0  |  2*a2*a1  |   a2*a2   |    DUP    |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a3 |  2*a3*a0  |  2*a3*a1  |  2*a3*a2  |   a3*a3   |    DUP    |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+
    ;   |  a4 |  2*a4*a0  |  2*a4*a1  |  2*a4*a2  |  2*a4*a3  |   a4*a4   |
    ;   +-----+-----------+-----------+-----------+-----------+-----------+

    ; the whole function works in 3 Passes

    ; Pass 1: Accumulate non-diagonal products in the result
    ;       
    ;   For an (n * n) sized squaring operation, there will be exactly (n * (n - 1)) / 2
    ;   number of non-diagonal multiplications since those products occur twice in the 
    ;   multiplication matrix. In pass 1, add all those sums once.
    ;
    ; Pass 2: Shift left the accumulated non-diag prods by 1 bit
    ;   
    ;   Shift the accumulated products left by 1 bit, essentially multiplying 
    ;   the whole lower-triangular part of the matrix by 2, getting the desired value.
    ;
    ; Pass 3: Accumulate the diagonal products (squares) in the result
    ;
    ;   These squares only need to be added once as shown along the diagonal.

    xchg    rdi, rcx        ; exchange rcx and rdi, freeing up rcx for jrcxz/loop
    xchg    rsi, rdx        ; exchange rdx and rsi, freeing up rdx for mul

    ; op1 in rsi, result in rdi

    mov     r9,  r8         ; copy of size in r9
    dec     r9              ; lower triangular matrix elems start with
                            ; (n - 1) -> (n - 2) -> ... -> 2 -> 1 -> 0
    test    r9,  r9         ; test if the basecase sqr size is 1, then no double sum prods
    jz      before_pass3    ; do the single diagonal sqr prod

    ; PASS-1 (O(n^2) step)

    xor     r12, r12    ; counter (starts at 0)
    xor     r13, r13    ; to restore rax after it has been clobbered by mul
    
outer_loop_pass1:

    ; rdx:rax for accumulating the product via mul
    ; r10 <- result (copy for loop)
    ; r11 <- op1    (copy for loop)
    ; r9 <- (size - 1) (copy for loop)

    mov     rcx,  r9    ; rcx is inner loop counter
    xor     rbx, rbx    ; temp_reg
    xor     rdx, rdx    ; high64 = 0
    mov     r13, QWORD PTR [rsi + r12 * 1]
    lea     r10, [rdi + r12 * 2 + 8]      
    lea     r11, [rsi + r12 * 1 + 8]      

ALIGN 16
inner_loop_pass1:

    mov     rax, r13            ; restore clobbered rax or load for the first time
    adc     rbx, rdx            ; temp_reg += (CF + high64)
    mul     QWORD PTR [r11]     ; rdx:rax = rax * op1[counter + 1]

    add     rbx, rax                ; temp_reg += low64
    adc     rdx, 0                  ; high64 += CF
    add     QWORD PTR [r10], rbx    ; result[counter + 1] += temp_reg

    mov     rbx, 0
    lea     r10, [r10 + 8]
    lea     r11, [r11 + 8]
    loop    inner_loop_pass1

outer_loop_end_pass1:

    adc     QWORD PTR [r10], rdx
    add     r12, 8
    dec     r9
    jnz     outer_loop_pass1

    ; PASS-2 (O(n) step)

before_pass2:

    lea     r10, [rdi + 8]
    mov     rcx, r8
    sub     rcx, 1
    
    ; 2 * (n - 1) limbs in result need to be shifted left by 1
    ; the first and last limb don't contain any values
    ; limbs (inclusive) of result[1:2n-2]
    ; Process two limbs at once for each decrement in rcx

    xor     r11, r11    ; zeroes out the carry flag
                        ; before the rcl
loop_pass2:

    rcl     QWORD PTR [r10], 1
    rcl     QWORD PTR [r10 + 8], 1
    
    lea     r10, [r10 + 16]
    loop    loop_pass2

    ; PASS-3 (O(n) step)

before_pass3:

    mov     r10, rdi
    mov     r11, rsi
    mov     rcx, r8

    xor     r12, r12            ; to clear CF/OF

loop_pass3:

    adc     r12, 0                  ; accumulate CF from last iter as mul clobbers both 
                                    ; OF & CF, does nothing in first iter
    mov     rax, QWORD PTR [r11]
    mul     rax                     ; rdx:rax = rax * rax (rax contains op1[i])
    add     r12, rax                ; mul can set both OF/CF, to take care of that
                                    ; add rax to an empty r12                 
    add     QWORD PTR [r10], r12
    adc     QWORD PTR [r10 + 8], rdx    
    mov     r12, 0

    lea     r11, [r11 + 8]
    lea     r10, [r10 + 16]
    loop    loop_pass3
    
end_of_func:

    pop     r13
    pop     r12
    pop     rdi
    pop     rsi
    pop     rbx
    ret

sqr_bc_x64 ENDP

END