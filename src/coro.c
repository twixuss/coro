#include <coro.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void coro_init(coro_state **_state, size_t (*coroutine)(coro_state *, size_t), size_t stack_size) {
	char *stack_base = malloc(stack_size + sizeof(coro_state));
	coro_state *state = (coro_state *)(stack_base + stack_size);
	memset(state, 0, sizeof(*state));

	state->stack_base = stack_base;

	state->mark = (void *)(size_t)0xdeadc0de;

#ifdef _WIN64

	//
	// Windows 64-bit
	//

	const int saved_registers_count = 8; // rbp rbx rsi rdi r12 r13 r14 r15
	const int register_size = 8;

	state->sp =
		(char *)state->stack_base + stack_size
		- saved_registers_count * register_size
		- register_size // return address
		- 16; // shadow space
	state->resume = coroutine;
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 0)) = coro_return; // return address
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 1)) = 0; // rbp

#elif defined(_WIN32)

	//
	// Windows 32-bit
	//

	const int saved_registers_count = 4; // ebp ebx esi edi
	const int register_size = 4;

	state->sp =
		(char *)state->stack_base + stack_size
		- saved_registers_count * register_size
		- 8 // state pointer + parameter
		- register_size; // return address
	state->resume = coroutine;
	*(void **)((char *)state->sp + register_size * (saved_registers_count + 2)) = 0; // parameter
	*(void **)((char *)state->sp + register_size * (saved_registers_count + 1)) = state; // state pointer
	*(void **)((char *)state->sp + register_size * (saved_registers_count + 0)) = coro_return; // return address
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 1)) = 0; // ebp

#elif defined(__linux__)
#if __SIZE_MAX__ == 0xffffffff

	//
	// Linux 32-bit
	//

	const int saved_registers_count = 4; // ebp ebx esi edi
	const int register_size = 4;

	state->sp =
		(char *)state->stack_base + stack_size
		- saved_registers_count * register_size
		- 8 // state pointer + parameter
		- register_size; // return address
	state->resume = coroutine;
	*(void **)((char *)state->sp + register_size * (saved_registers_count + 2)) = 0; // parameter
	*(void **)((char *)state->sp + register_size * (saved_registers_count + 1)) = state; // state pointer
	*(void **)((char *)state->sp + register_size * (saved_registers_count + 0)) = coro_return; // return address
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 1)) = 0; // ebp

#else

	//
	// Linux 64-bit
	//

	const int saved_registers_count = 6; // rbp rbx r12 r13 r14 r15
	const int register_size = 8;

	state->sp =
		(char *)state->stack_base + stack_size
		- saved_registers_count * register_size
		- register_size; // return address
	state->resume = coroutine;
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 0)) = coro_return; // return address
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 1)) = 0; // rbp

#endif
#else

#error coro.c: invalid platform

#endif
	*_state = state;
}

void coro_error(void) {
	assert(!"Attempt to run a coroutine that has already finished.");
}
