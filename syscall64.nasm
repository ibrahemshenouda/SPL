global main

SECTION .data
msg: db "Hello World from x86_64 machin", 0Ah, 0h
len_msg: equ $ - msg

SECTION .text
main:
	mov rax, 1
	
	mov rdi, 1
	mov rsi, msg
	mov rdx, len_msg
	syscall 		;write(1, msg, len_msg);
	
	mov r15, rax
	mov rax, 60
	mov rdi, r15
	syscall 		;exit(write_len)
