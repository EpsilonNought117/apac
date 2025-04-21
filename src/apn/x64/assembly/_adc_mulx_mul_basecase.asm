.code

	option casemap:none

	;   Function Arguments
    ;
    ;   rcx -> result (u64*)   
    ;   rdx -> op1 (const u64*)
    ;   r8  -> op2 (const u64*)
    ;   r9  -> size1 (u64)
    ;   [rsp + 40] -> size2 (u64) (where rsp is initially at function entering)

    ;   ASSUMPTION
    ;   (size1 >= size2)

_adc_mulx_mul_basecase PROC FRAME

	push	rbx
.pushreg	rbx
	push	rdi
.pushreg	rdi
	push	rsi
.pushreg	rsi
	push	r12
.pushreg	r12
	push	r13
.pushreg	r13
.pushframe
.endprolog

	xor		rax, rax	; i
	xchg	rbx, rdx	; rbx <- op1 (const u64*)

loop_outer:

	xor		r12, r12	; temp_reg		
	xor		r11, r11	; j
	xor		rsi, rsi	; low64
	xor		rdi, rdi	; high64
	mov		r10, rax	; indexer for result
	mov		r13, r9		; size1 in r13 for jnz
	mov		rdx, QWORD PTR [r8 + rax*8] ; op2[i]
	
loop_inner:
	
	adc		r12, rdi
	mulx	rdi, rsi, QWORD PTR [rbx + r11*8]	; rdi:rsi = rdx * op1[j]

	add		r12, rsi
	adc		rdi, 0
	add		QWORD PTR [rcx + r10*8], r12

	mov		r12, 0
	inc		r11
	inc		r10
	dec		r13
	jnz		loop_inner

loop_end:
	
	mov		r12, QWORD PTR [rsp + 80]	; size2 into r12 temporarily
	adc		QWORD PTR [rcx + r10*8], rdi
	inc		rax
	cmp		r12, rax
	jnz		loop_outer

end_of_func:

	pop		r13
	pop		r12
	pop		rsi
	pop		rdi
	pop		rbx
	ret		0

_adc_mulx_mul_basecase ENDP

END