bits 64

global coro_yield
global coro_return
extern coro_error

%define state rdi
%define value rsi

section .text

coro_yield:
	; yield value
	mov rax, value

	; save my registers
	push rbp
	push rbx
	push r12
	push r13
	push r14
	push r15

	; restore other stack
	xchg qword [state], rsp

	; restore other registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp

	ret


coro_return:
	mov state, rsp
	mov value, rax
	sub rsp, 8; keep the stack aligned to 16 bytes
	call coro_yield
	jmp coro_error
