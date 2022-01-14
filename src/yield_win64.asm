bits 64

global coro_yield
global coro_return
extern coro_error

section .text

coro_yield:
	; yield value
	mov rax, rdx

	; save my registers
	push rbp
	push rbx
	push rsi
	push rdi
	push r12
	push r13
	push r14
	push r15

	; restore other stack
	xchg [rcx], rsp

	; restore other registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop rdi
	pop rsi
	pop rbx
	pop rbp

	ret


coro_return:
	lea rcx, [rsp + 32]
	mov rdx, rax
	call coro_yield
	sub rsp, 8
	jmp coro_error
