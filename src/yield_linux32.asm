bits 32

global _coro_yield
global _coro_return
extern _coro_error

coro_state:
.resume equ 0x00
.sp     equ 0x04

section .text

_coro_yield:
	mov ecx, [esp + 4]

	; yield value
	mov eax, [esp + 8]

	; get other resume address
	mov edx, dword [ecx + coro_state.resume]

	; save my resume address
	pop dword [ecx + coro_state.resume]

	; save my registers
	push ebp
	push ebx
	push esi
	push edi

	; restore other stack
	xchg dword [ecx + coro_state.sp], esp

	; restore other registers
	pop edi
	pop esi
	pop ebx
	pop ebp

	jmp edx


_coro_return:
	lea ecx, [esp + 8]
	push eax
	push ecx
	call _coro_yield
	jmp _coro_error

