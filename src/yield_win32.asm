bits 32

global _coro_yield
global _coro_return
extern _coro_error

section .text

_coro_yield:
	; state pointer
	mov ecx, [esp + 4]

	; yield value
	mov eax, [esp + 8]

	; save my registers
	push ebp
	push ebx
	push esi
	push edi

	; restore other stack
	xchg [ecx], esp

	; restore other registers
	pop edi
	pop esi
	pop ebx
	pop ebp

	mov edx, [ecx - 4]
	test edx, edx
	jnz .ret
	; set up parameters for the first call to coroutine
	mov byte [ecx - 4], 1
	mov [esp + 12], eax
	mov [esp + 8], ecx
.ret:
	ret


_coro_return:
	lea ecx, [esp + 12]
	push eax
	push ecx
	call _coro_yield
	jmp _coro_error
