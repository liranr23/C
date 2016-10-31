section .rodata
  string: 		db "calc: ", 0	; Format string
  number:		db "%X",  0
  newline: 		db "", 10, 0
  error_over_flow: 	db "Error : Stack is over flow" , 10, 0
  error_no_args: 	db "Error: Insufficient argument count in stack" ,10, 0
  number_of_operators:  db "Number of operators: ", 0
  LC0: 			DB	"The result is:  %s", 10, 0	; Format string
section .bss
buffer:
	RESB 	80
stack: 
	RESB 	20

section .data
    node_size: 	 EQU   	 5
    stack_index: DD	 0
    counter: 	 DD	 0
    buffer_iter: DD	 0   
    first_or_second: DD  1
    ones_counter:    DD  0
    y_counter:	DD	0
    
section .text
	align 16
	global main
	extern printf
	extern malloc
	extern gets

 
%macro  macro_start 0
        push	ebp
	mov	ebp, esp	
	pushad		
%endmacro
 
 
%macro  macro_ret 0-1 0      
      	popad
        mov esp, ebp
        pop ebp
        ret %1 * 4
%endmacro


%macro  macro_process_operator 0
inc	byte[counter]
	
	cmp 	bl, '+'
	je 	call_add
		
	cmp 	bl, 'p'
	je 	call_pop_and_print
	
	cmp 	bl, 'd'
	je 	call_duplicate
	
	cmp 	bl, 'n'
	je 	call_num_of_bits
	
	cmp	bl, '^'
	je	call_power
	
	dec	byte[counter]
	
	cmp 	bl, 'q'
	je 	quit
%endmacro


%macro  macro_process_d 1 ;get char in bl, where to jmp when end
	cmp 	bl , 'a'
	jge 	%%low_to_num
	
	cmp 	bl , 'A'
	jge 	%%up_to_num
	
	jmp 	%%num_to_num
	      
	      
      %%up_to_num:
	      sub	bl, 'A'
	      add	bl, 10
	      jmp	%1

      %%low_to_num:
	      sub	bl, 'a'
	      add	bl, 10
	      jmp	%1
	      
      %%num_to_num:
	      cmp	bl, 0
	      je	push_to_stack
	      sub	bl, '0'
%endmacro

%macro macro_creat_node 2
	push 	edx
	push 	ecx		; keep the value of ecx because of malloc
	push 	5 		;push amount of bytes malloc should allocate    
	call 	malloc 		;call malloc
	add  	esp,4
	pop 	ecx
	pop 	edx
	
	shl 	ebx, %1; 4=num
	add 	ebx, %2; ecx=to add
	
	mov 	[eax], ebx

	mov 	dword [eax+ 4], edx
	mov 	edx, eax 		;keep the node address in edx
	
	inc 	byte[buffer_iter]
	jmp 	two_chars
%endmacro
	
main:
	macro_start
start:
	mov 	dword[buffer_iter], 0
	push	string		; and pointer to format string.
	call	printf
	add 	esp, 4		; Clean up stack after call
	push 	buffer
	call 	gets 		;read from user and inserts to buffer
	add 	esp, 4
	mov 	edi, 0
	mov 	edx, 0		; keep address of the node
	
read_buffer:	
check_len: 			;checks whether the buffer size is odd or even
	mov eax ,0
	mov cl, 1
loop:
	mov 	bl, byte[buffer+eax]
	not 	cl
	add 	eax, 1
	cmp 	bl, 0
	jne 	loop
	
	cmp 	eax, 0
	je 	start
	
	cmp 	cl, 1
	je 	one_char
	jmp 	two_chars
		
one_char:
	mov 	ebx, 0
	mov 	ecx, 0
	mov 	bl, byte[buffer+edi]	; Read a single byte from the string. Store into a byte size register
	
	;checking if it's a operator	
	macro_process_operator
	macro_process_d continue

continue:	
	;create node
	mov	ecx,0
	macro_creat_node 0,ecx

two_chars:
	mov 	ecx, 0
	mov	ebx, 0
	mov 	edi, [buffer_iter]
	mov 	bl, byte[buffer+edi]	; Read a single byte from the string. Store into a byte size register
	
	inc 	byte[buffer_iter]
	mov 	cl, 0
	mov 	edi, [buffer_iter]
	mov 	cl, byte[buffer+edi]	; Read a single byte from the string. Store into a byte size register

  macro_process_d continue1

continue1:		
	;second char
	push	ebx
	mov	bl, cl
	macro_process_d continue2
continue2:
	;create node
	mov	cl, bl
	pop 	ebx
	macro_creat_node 4,ecx
	
push_to_stack:
	mov 	eax, [stack_index]
	cmp 	eax, 20
	jge 	print_over_flow

	mov 	[stack+eax] ,edx
	add 	word[stack_index], 4

	jmp 	start

call_add:
	call 	add
	jmp 	start
	
call_pop_and_print:
	call 	pop_and_print
	jmp start

call_duplicate:
	call 	duplicate
	jmp start
	
call_num_of_bits:
	call 	num_of_bits
	jmp start
call_power:
	call	power
	jmp start
	
add:
	macro_start 
	cmp 	dword[stack_index], 8
	jl  	print_error_no_args
	sub 	word[stack_index], 4

	mov 	ecx, [stack_index]
	mov 	ebx, [stack+ecx]	;eax= value of the top element in the stack
	sub 	ecx, 4
	mov 	eax, [stack+ecx]	
      
	mov 	ecx, [ebx]
	mov 	edx, [eax]
    
bb:
	jmp   	add_carry
     
next_node:
	mov    	edi, dword [eax]
	mov    	edx, [edi]
	mov    	eax, edi
     
	mov    	edi, dword [ebx]
	mov    	ecx, [edi]
	mov    	ebx, edi
         
add_carry:
	adc 	edx, ecx
	mov 	[eax], edx    ;put the sum in the node

	add 	ebx, 4
	cmp 	dword [ebx], 0 ;
	je  	end_add

	add 	eax, 4
	cmp 	dword [eax], 0 ;
	jne 	next_node
    
	mov 	ebx, [ebx]	
	mov 	[eax], ebx
 
end_add:
      macro_ret  
	
pop_and_print:
	macro_start 
	cmp 	dword[stack_index], 0
	je  	print_error_no_args
	sub 	word[stack_index], 4
again:
	mov 	ecx, [stack_index]
	mov 	eax, [stack+ecx]	;eax= value of the top element in the stack
	jmp 	a
find_end:
	mov 	edx, eax
	mov 	eax, [eax]
a:
	add 	eax, 4
	cmp 	dword[eax], 0
	jne 	find_end     ; eax= address of 0 in last node, edx= address of previous node
	    
	sub 	eax, 4
	pusha	;save registers
	push 	dword[eax]			;print
	push 	number
	call 	printf
	add 	esp, 8
	popa
	    
	cmp 	edx, 0
	je 	new_line
	mov 	dword[edx], 0		;make pointer to 0
	    
	cmp 	eax, [stack+ecx]
	jne 	again
new_line:
	push 	newline
	call 	printf
	add 	esp, 4
	macro_ret 
	
duplicate:
	macro_start
	mov 	eax, [stack_index]
	cmp 	eax, 20
	jge 	print_over_flow
	sub 	word[stack_index], 4
		
	mov 	eax, [stack_index]
	mov 	edx, [stack+eax]
		
	add 	word[stack_index], 4   ;edx= address of top element

        push 	edx
        push 	5
	call 	malloc
	add 	esp, 4
	pop 	edx
		
	push 	eax 	;keep for end
	jmp 	d_j
d_loop:
	push 	ecx
        push 	edx
        push 	5
	call 	malloc
	add 	esp, 4
	pop 	edx
	pop 	ecx
	
	mov	[ecx], eax
	mov 	edx, [edx]
	
d_j: 
	mov 	ebx ,[edx]
	mov 	[eax], ebx
    
	add 	eax, 4
	mov 	ecx, eax
	
	add 	edx, 4
	
	cmp 	dword[edx], 0
	jne 	d_loop
	
d_continue:
	pop 	edx
	mov 	eax, [stack_index]
	mov 	[stack+eax] ,edx
	add 	word[stack_index], 4

	macro_ret

power:
	macro_start 
	cmp 	dword[stack_index], 8
	jl  	print_error_no_args
	sub 	word[stack_index], 4
	mov	esi, 0

	mov 	ecx, [stack_index]
	sub 	ecx, 4
	mov 	eax, [stack+ecx]	;eax= next to TOS = y
	
	mov	dword[y_counter], 1
again1:
	mov 	ecx, [stack_index]
	sub	ecx, 4
	mov 	eax, [stack+ecx]	;eax= value of the top element in the stack
	jmp 	a1
find_end1:
	inc 	dword[y_counter]
	mov 	edx, eax
	mov 	eax, [eax]
a1:
	add 	eax, 4
	cmp 	dword[eax], 0
	jne 	find_end1     ; eax= address of 0 in last node, edx= address of previous node
	    
	sub 	eax, 4
	cmp	dword[y_counter], 0
	je	end_pow
	cmp	byte[y_counter], 1
	je	rishon
	cmp	byte[y_counter], 2
	je	sheni
	cmp	byte[y_counter], 3
	je	shlishi
	jmp	end_pow
rishon:
	add	esi, [eax]
	jmp	abba
sheni:
	push	edx
	mov	eax, [eax]
	mov	bx, 100
	mul	bx
	add 	esi, eax
	pop 	edx
	jmp 	abba
shlishi:
	push	edx
	mov	eax, [eax]
	mov	bx, 10000
	mul	bx
	add 	esi, eax
	pop 	edx
	    
abba:
	cmp 	edx, 0
	je 	end_pow
	mov 	dword[edx], 0		;make pointer to 0
	dec	dword[y_counter]
	    
	cmp 	eax, [stack+ecx]
	jne 	again1
end_pow:
	mov	eax, esi
	mov	dx, 0
	mov	bx, 8
	div	bx
	mov	esi, eax	;the div 8
	mov	edi, edx	;the modulo
	cmp	esi, 0
	je	just_shift

again2:
	mov 	ecx, [stack_index]
	mov 	eax, [stack+ecx]	;eax= value of the top element in the stack = x
	jmp 	a2
find_end2:
	mov 	edx, eax
	mov 	eax, [eax]
a2:
	add 	eax, 4
	cmp 	dword[eax], 0
	jne 	find_end2     ; eax= address of 0 in last node, edx= address of previous node
	;eax=last_node->next
	mov	ecx, eax
shift_left:
	cmp	esi, 0
	je	just_shift
	push	ecx
	push 	5 		;push amount of bytes malloc should allocate    
	call 	malloc 		;call malloc
	add  	esp,4
	pop 	ecx
	
	mov 	[ecx], eax	
	mov	ebx, 0
	mov 	[eax], ebx	;2 bytes of zeroes
	add 	eax, 4		;node->next
	mov 	ecx, eax 	;keep the node address in ecx
	mov	word[ecx], 0
	dec 	esi
	jmp	shift_left
just_shift:
	mov 	ecx, [stack_index]
	mov 	eax, [stack+ecx]
	cmp	edi, 0
	je	to_the_node
	mov	ebx, [eax]
shift_it:
	cmp	edi, 0
	je	to_the_node
	shl 	ebx, 1
	dec	edi
	jmp	shift_it
to_the_node:
	mov 	ecx, [stack_index]
	sub	ecx, 4
	mov 	edx, [stack+ecx]
	mov 	[edx], ebx
	
finish_pow:
	macro_ret 
	
num_of_bits:
	macro_start
	cmp 	dword[stack_index], 0
	je  	print_error_no_args
	sub 	word[stack_index], 4

	mov 	ecx, [stack_index]
	mov 	eax, [stack+ecx]	;eax= value of the top element in the stack
count_again:
	mov 	edx, 1
	mov 	ebx, dword[eax]
	jmp 	count_ones
	
num_continue:
	add 	eax, 4
	cmp 	dword[eax], 0
	je 	finish_count
	
	mov 	eax, [eax]
	jmp 	count_again	

count_ones:
	mov 	edx, 1
	cmp 	ebx, 0
	je 	num_continue
	and 	edx, ebx
	shr 	ebx, 1
	cmp 	edx, 0
	je 	count_ones
	inc 	dword[ones_counter]
	jmp 	count_ones
		
finish_count:
	mov 	eax, dword[ones_counter]
	push 	eax
	push 	number
	call 	printf
	add 	esp, 8
	mov 	dword[ones_counter], 0
	push 	newline
	call 	printf
	add 	esp, 4
	
	macro_ret
		
print_over_flow:
	push	error_over_flow		
	call	printf
	add 	esp, 4
	macro_ret
		
print_error_no_args:
	push	error_no_args		
	call	printf
	add 	esp, 4
	macro_ret

print_counter:
	macro_start
	push	number_of_operators	
	call 	printf
	add 	esp, 4
	push 	dword[counter]
	push 	number
	call	printf
	add 	esp, 8
	push 	newline
	call 	printf
	add 	esp, 4
	macro_ret		

quit:
	call 	print_counter
	macro_ret