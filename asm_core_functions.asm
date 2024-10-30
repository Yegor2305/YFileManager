.code

Get_Sum proc
;Get_Sum(int first, int second)
;rcx - first
;rdx - second
	
	push rcx
	push rdx

	mov eax, ecx
	add eax, edx

	pop rdx
	pop rcx
	ret

Get_Sum endp

end