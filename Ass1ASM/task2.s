section	.rodata
LC0:
	DB	"%d", 10, 0	; Format string
LC1:
	DB	"x is out of range", 10, 0	; Wrong input string
	
section .text
	align 16
	global calc_func
	extern printf
	extern check

calc_func:
	push	ebp
	mov	ebp, esp	; Entry code - set up ebp and esp
	pusha			; Save registers

	mov ecx, dword [ebp+8]	; Get argument (pointer to string)

	;*Program starts*
	push 	ecx		; setting the input number to check function.
	call	check
	pop 	ecx		; resetting the input number.
	cmp	eax, 0		; checking the argument we recieved from check function.
	je	wrong_input
	mov	ebx, ecx	; intializing for doing powers.
	mov	eax, 2
	dec	ebx
	cmp	ebx, 0		; in case the input is 1, we don't need to power it up so we jump forward.
	je	divide
	
powers:				; 2^x. eax will have the answer.
	mov	edx, 2
	mul	edx
	dec	ebx
	cmp	ebx, 0
	jne	powers
divide:				; now eax=2^x, we divide it with the input.
	mov	edx, 0
	mov	ebx, ecx
	div	ebx		; eax=final answer=(2^x)/x rounded down.
	;Setting the ouput and calling the fprint function.
	push	eax		; the result.
	push	LC0		; and pointer to format string.
	call	printf
	add 	esp, 8		; Clean up stack after call
	jmp	clean_up	; passing on the wrong input section.
	
wrong_input:			; if we got wrong input(by check), we will print the wanted string.
	push	LC1
	call	printf
	add	esp, 4		; cleaning the stack after call.

clean_up:	
	popa			; Restore registers
	mov	esp, ebp	; Function exit code
	pop	ebp
	ret

