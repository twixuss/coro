bits 64

global coro_yield
global coro_return
extern coro_error

coro_state:
.resume equ 0x00
.sp     equ 0x08

%define state rdi
%define value rsi

section .text

coro_yield:
	; yield value
	mov rax, value

	; get other resume address
	mov r8, qword [state + coro_state.resume]

	; save my resume address
	pop qword [state + coro_state.resume]

	; save my registers
	push rbp
	push rbx
	push r12
	push r13
	push r14
	push r15

	; restore other stack
	xchg qword [state + coro_state.sp], rsp

	; restore other registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp

	jmp r8


coro_return:
	mov state, rsp
	mov value, rax
	call coro_yield
	jmp coro_error
