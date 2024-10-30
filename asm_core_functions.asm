.code

SetOutputOffset proc
; rcx - screen_buffer
; rdx - OutputPos
; rax - result(offset)
	
	mov eax, edx
	shr eax, 16 ; ax = y_pos

	mov rbx, rdx
	shr rbx, 32 ; bx = screen_width

	imul ax, bx ; ax = y_pos * screen_width
	add ax, dx ; ax = y_pos * screen_width + x_pos

	shl ax, 2
	
	mov rdi, rcx
	add rdi, rax

	ret

SetOutputOffset endp

DrawLineHorizontal proc
;void DrawLineHorizontal(CHAR_INFO *screen_buffer, OutputPos pos, CHAR_INFO symbol_info);
;rcx - screen_buffer
;rdx - pos
;r8 - symbol_info
	
	push rax
	push rbx
	push rcx
	push rdi

	call SetOutputOffset

	mov eax, r8d ; eax = symbol_info
	mov rcx, rdx
	shr rcx, 48 ; rcx = len
	
	rep stosd

	pop rdi
	pop rcx
	pop rbx
	pop rax
	ret

DrawLineHorizontal endp

DrawLineVertical proc
;void DrawLineVertical(CHAR_INFO *screen_buffer, OutputPos pos, CHAR_INFO symbol_info);
;rcx - screen_buffer
;rdx - pos
;r8 - symbol_info
	
	push rax
	push rbx
	push rcx
	push rdi
	push r10

	call SetOutputOffset

	mov eax, r8d ; eax = symbol_info
	mov rcx, rdx
	shr rcx, 48 ; rcx = len
	
	mov r10, rdx
	shr r10, 32
	movzx r10, r10w
	dec r10
	shl r10, 2 ; r10 = screen_width in bytes - 1

printing_loop:
	
	stosd
	add rdi, r10

	loop printing_loop

	pop r10
	pop rdi
	pop rcx
	pop rbx
	pop rax
	ret

DrawLineVertical endp

PrintColorPalette proc
;rcx - screen_buffer
;rdx - pos
;r8 - symbol_info
	
	push rax
	push rbx
	push rcx
	push rdi
	push r9
	push r10

	call SetOutputOffset

	mov r9, rdi

	mov r10, rdx
	shr r10, 32
	movzx r10, r10w
	shl r10, 2 ; r10 = screen_width in bytes

	mov eax, r8d ; eax = symbol_info
	and rax, 0ffffh ; rax = ax = symbol

	mov rbx, 16

outer_loop:

	mov rcx, 16
	add r9, r10
	mov rdi, r9

inner_loop:

	stosd
	add eax, 010000h
	
	loop inner_loop

	dec rbx
	jnz outer_loop

	pop r10
	pop r9
	pop rdi
	pop rcx
	pop rbx
	pop rax
	ret

PrintColorPalette endp

end