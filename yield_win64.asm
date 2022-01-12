bits 64

global tl_coroutine_yield

coro_state:
.resume equ 0x00
.sp     equ 0x08

section .text

t_coro_yield:
	; yield value
	mov rax, rdx

	; get other resume address
	mov r8, qword [rcx + coro_state.resume]

	; save my resume address
	pop qword [rcx + coro_state.resume]

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
	xchg qword [rcx + coro_state.sp], rsp

	; restore other registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop rdi
	pop rsi
	pop rbx
	pop rbp

	jmp r8
