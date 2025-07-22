public measure_read_inner_x64
public measure_write_inner_x64

.data
.code

align 8
measure_write_inner_x64 proc
; rcx: ptr
; rdx: num_bytes

	mov			r8, 128
	vxorps		ymm0, ymm0, ymm0

_loop:
	vmovntps	ymmword ptr [rcx], ymm0
	vmovntps	ymmword ptr [rcx + 32], ymm0
	vmovntps	ymmword ptr [rcx + 64], ymm0
	vmovntps	ymmword ptr [rcx + 96], ymm0

	add			rcx, r8
	sub			rdx, r8
	test		rdx, rdx
	jg			_loop

	ret

align 8
measure_write_inner_x64 endp


align 8
measure_read_inner_x64 proc
; rcx: ptr
; rdx: num_bytes

	mov			r8, 128

_loop:
	vmovntdqa	ymm0, ymmword ptr [rcx]
	vmovntdqa	ymm1, ymmword ptr [rcx + 32]
	vmovntdqa	ymm2, ymmword ptr [rcx + 64]
	vmovntdqa	ymm3, ymmword ptr [rcx + 96]

	add			rcx, r8
	sub			rdx, r8
	test		rdx, rdx
	jg			_loop

	ret

align 8
measure_read_inner_x64 endp

end