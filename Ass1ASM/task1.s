section	.rodata
LC0:
	DB	"The result is:  %s", 10, 0	; Format string

section .bss
LC1:
	RESB	32

section .text
	align 16
	global my_func
	extern printf

my_func:
	push	ebp
	mov	ebp, esp	; Entry code - set up ebp and esp
	pusha			; Save registers

	mov ecx, dword [ebp+8]	; Get argument (pointer to string)

;       Your code should be here...
	mov	eax, 0		; Initializing registers
	mov	edx, 0
	mov	ebx, 0
	mov	esi, LC1	; esi=pointer to the answer
	cmp	byte [ecx], 0	; if the entry is not empty
	jne	next
	mov 	byte [LC1], 0
	jmp	fatalaty	
	
next:				;counting the number of chars+getting to the end of the string
	inc	bx
	inc	ecx
	cmp	byte [ecx], 10	; 10 is \n
	je	start
	jmp 	next
	
start:				;moving backwards in the string
	dec	bx		;counting down the length of the string
	dec	ecx		
	mov	dl, [ecx]	;getting the current char into dl register
	sub	dl, '0'		;changing the argument to a decimal
	mov	al, dl		;this time is LSB so it is 2^0. We sum it up in al.
	cmp	bx, 0		;current binary number is less than 4 chars?
	je	meanwhile
	dec	ecx
	dec	bx
	mov	dl, [ecx]
	sub	dl, '0'
	add	al, dl		;this one is 2^1
	add	al, dl
	cmp	bx, 0
	je	meanwhile
	dec	ecx
	dec	bx
	mov	dl, [ecx]
	sub	dl, '0'
	add	al, dl		;this one is 2^2
	add	al, dl
	add	al, dl
	add	al, dl
	cmp	bx, 0
	je	meanwhile
	dec	ecx
	dec	bx
	mov	dl, [ecx]
	sub	dl, '0'
	add	al, dl		;this one is 2^3
	add	al, dl
	add	al, dl
	add	al, dl
	add	al, dl
	add	al, dl
	add	al, dl
	add	al, dl
	
meanwhile:			;We finished the sum for 1hex char(4 or less chars of the binary). 	
	cmp	al, 0
	je	zero
	cmp	al, 1
	je	one
	cmp	al, 2
	je	two
	cmp	al, 3
	je	three
	cmp	al, 4
	je	four
	cmp	al, 5
	je	five
	cmp	al, 6
	je	six
	cmp	al, 7
	je	seven
	cmp	al, 8
	je	eight
	cmp	al, 9
	je	nine
	cmp	al, 10
	je	ten
	cmp	al, 11
	je	eleven
	cmp	al, 12
	je	twelve
	cmp	al, 13
	je	thirteen
	cmp	al, 14
	je	fourteen
	cmp	al, 15
	je	fifteen

zero:				;For each case we change the value to the current ASCII char of hexdecimal number.
	mov	al, '0'
	jmp	enter_res
one:
	mov	al, '1'
	jmp	enter_res
two:
	mov	al, '2'
	jmp	enter_res
three:
	mov	al, '3'
	jmp	enter_res
four:
	mov	al, '4'
	jmp	enter_res
five:
	mov	al, '5'
	jmp	enter_res
six:
	mov	al, '6'
	jmp	enter_res
seven:
	mov	al, '7'
	jmp	enter_res
eight:
	mov	al, '8'
	jmp	enter_res
nine:
	mov	al, '9'
	jmp	enter_res
ten:
	mov	al, 'A'
	jmp	enter_res
eleven:
	mov	al, 'B'
	jmp	enter_res
twelve:
	mov	al, 'C'
	jmp	enter_res
thirteen:
	mov	al, 'D'
	jmp	enter_res
fourteen:
	mov	al, 'E'
	jmp	enter_res
fifteen:
	mov	al, 'F'
	jmp	enter_res
	
enter_res:			;Writing down the answer.
	mov	[esi], al
	inc	esi
	cmp	bx, 0
	je	set_count	;The string is over.
	jmp	start		;If the string isn't ended. Start over for the next binary number.
set_count:			;Setting up a counter and initializing the pointer(for push\pop reversing the answer).
	mov	ax, 0
	inc	esi
	mov	byte [esi], 0
	mov	esi, LC1
reverse:			;Counting the asnwer, for each hexdecimal char we push it into the stack.
	inc	ax
	mov	dx, [esi]
	push	dx
	inc	esi
	cmp	byte [esi], 0	;Finished passing on the answer.
	jne	reverse
	mov	esi, LC1	;Setting the pointer againg to put the answer.
set_in:				;Counting down(to know when to stop poping things out from the stack. :)
	dec	ax
	pop 	dx
	mov	[esi], dx	;Setting the poped char in the answer.
	inc 	esi
	cmp	ax, 0		;checking when we finished.
	jne	set_in	
	;now the answer is reversed, as it should be in the end.
	mov	byte [esi], 0	;Setting 0 as a null char for the end of the string-answer.
fatalaty:			;Finishing the programm.
	push	LC1		; Call printf with 2 arguments: pointer to str
	push	LC0		; and pointer to format string.
	call	printf
	add 	esp, 8		; Clean up stack after call

	popa			; Restore registers
	mov	esp, ebp	; Function exit code
	pop	ebp
	ret

