public measure_fun2_x64
public sample_fun_x64

.data
.code

; ---===[ CHEAT SHEET ]===---
; return        : rax
; args          : rcx, rdx, r8, r9
; must preserve : r12-15, rdi, rsi, rbx, rbp, ymm6-15
; 
; rdtsc -> edx:eax


align 8
sample_fun_x64 proc
	
	push		rsi
	push		rdi
	push		r12				; scratch
	push		r13				; time accumulator
	push		r14				; outer counter
	push		r15

	mov			rsi, rcx		; benchtask*
	mov			rdi, [rsi]		; fnptr
	xor			r14, r14
	
_sample_fun_x64_outer:
	mov			r12, [rsi + 16]
	mov			rcx, r12
	mov			rdx, r14
	call		rdi

	xor			r13, r13

_sample_fun_x64_inner:
	mov			rcx, r12

	rdtsc
	shl			rdx, 32
	or			rdx, rax
	mov			r15, rdx

	mov			rdx, r14
	call		rdi

	rdtsc						
	shl			rdx, 32			
	or			rdx, rax
	xor			rcx, rcx
	sub			rdx, r15
	cmovo		rdx, rcx		
	add			r13, rdx
	
	dec			r12
	jnz			_sample_fun_x64_inner

	mov			r12, [rsi + 8]	; benchtask->times
	mov			[r12 + 8 * r14], r13
	inc			r14
	
	mov			r12, [rsi + 24]
	cmp			r14, r12
	jb			_sample_fun_x64_outer

	pop			r15
	pop			r14
	pop			r13
	pop			r12
	pop			rdi
	pop			rsi

	ret

align 8
sample_fun_x64 endp


;;;

align 8
measure_fun2_x64 proc

	push		rsi		; tick count accumulator
	push		rdi		; tick count in iteration
	push		r12		; iteration counter
	push		r15		; DUT proc address

	vzeroupper
	mov			r12, rcx
	mov			r15, rdx
	xor			rsi, rsi

_measure_fun2_x64_1:
	mov			rcx, r12		; DUT 1st arg. -> rcx

	rdtsc						; time -> rdi
	shl			rdx, 32
	or			rdx, rax
	mov			rdi, rdx

	mov			rdx, r12		; DUT 2nd arg -> rdx
	call		r15				; call DUT

	rdtsc						; measure time again, subtract previous value
	shl			rdx, 32			; accumulate positive values into rsi
	or			rdx, rax
	xor			rcx, rcx
	sub			rdx, rdi
	cmovo		rdx, rcx		; clamp negative values to zero
	add			rsi, rdx

	dec			r12				; decrement counter and repeat if needed
	jnz			_measure_fun2_x64_1

	mov			rax, rsi		; set return value

	pop			r15
	pop			r12
	pop			rdi
	pop			rsi

	vzeroupper

	ret

align 8
measure_fun2_x64 endp

end