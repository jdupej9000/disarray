public sample_fungen_x64
public sample_fun_x64
public measure_fun_x64

.data
.code

; ---===[ CHEAT SHEET ]===---
; return        : rax
; args          : rcx, rdx, r8, r9
; must preserve : r12-15, rdi, rsi, rbx, rbp, ymm6-15
; 
; rdtsc -> edx:eax

align 8
sample_fungen_x64 proc
	
	push		rsi
	push		rdi
	push		r12				; scratch
	push		r13				; time accumulator
	push		r14				; outer counter
	push		r15

								; TODO: align the stack
	sub			rsp, 96			; 3 variables, 32B each (to fit a YMM reg)

	mov			rsi, rcx		; benchtask*
	mov			rdi, [rsi]		; fnptr
	xor			r14, r14
	
_sample_fungen_x64_outer:

	lea			rdx, [rsp]
	lea			r8, [rsp + 32]
	lea			r9, [rsp + 64]
	mov			rcx, r14
	mov			r12, [rsi + 32]	; genfnptr
	call		r12				; invoke the generator

	mov			r12, [rsi + 16]
	
	mov			rcx, [rsp]
	vmovups		ymm0, [rsp]
	mov			rdx, [rsp + 32]
	vmovups		ymm1, [rsp + 32]
	mov			r8, [rsp + 64]
	vmovups		ymm2, [rsp + 64]
	call		rdi				; call dut, to make sure its code is cached

	xor			r13, r13

_sample_fungen_x64_inner:
	vmovups		ymm0, [rsp]
	mov			r9, [rsp + 32]
	vmovups		ymm1, [rsp + 32]
	mov			r8, [rsp + 64]
	vmovups		ymm2, [rsp + 64]
	
	rdtscp
	shl			rdx, 32
	or			rdx, rax
	mov			r15, rdx

	mov			rcx, [rsp]
	mov			rdx, r9			; second GP argument (rdx)
	call		rdi

	rdtscp	
	shl			rdx, 32			
	or			rdx, rax
	xor			rcx, rcx
	sub			rdx, r15
	cmovo		rdx, rcx		
	add			r13, rdx
	
	dec			r12
	jnz			_sample_fungen_x64_inner

	mov			r12, [rsi + 8]	; benchtask->times
	mov			[r12 + 8 * r14], r13
	inc			r14
	
	mov			r12, [rsi + 24]
	cmp			r14, r12
	jb			_sample_fungen_x64_outer

	add			rsp, 96

	pop			r15
	pop			r14
	pop			r13
	pop			r12
	pop			rdi
	pop			rsi

	ret

align 8
sample_fungen_x64 endp

; --------------------------------------------------------------------------------------------


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

	xor			r8, r8
	xor			r9, r9

	rdtscp
	shl			rdx, 32
	or			rdx, rax
	mov			r15, rdx

	xor			rcx, rcx
	xor			rdx, rdx
	call		rdi

	rdtscp
	lfence
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

; --------------------------------------------------------------------------------------------

align 8
measure_fun_x64 proc

	push		rsi		; tick count accumulator
	push		rdi		; tick count in iteration
	push		r12		; iteration counter
	push		r15		; DUT proc address

	vzeroupper
	mov			r12, rdx
	mov			r15, rcx
	xor			rsi, rsi

_measure_fun_x64_1:
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
	jnz			_measure_fun_x64_1

	mov			rax, rsi		; set return value

	pop			r15
	pop			r12
	pop			rdi
	pop			rsi

	vzeroupper

	ret

align 8
measure_fun_x64 endp

end