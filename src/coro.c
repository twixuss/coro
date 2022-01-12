#include <coro.h>

void coro_return();

void coro_init(coro_state **_state, size_t (*coroutine)(coro_state *, size_t), unsigned long long stack_size) {
	char *stack_base = malloc(stack_size + sizeof(coro_state));
	coro_state *state = (coro_state *)(stack_base + stack_size);
	memset(state, 0, sizeof(*state));

	state->stack_base = stack_base;

	state->mark = 0xdeadc0de;

#ifdef _WIN64
	const int coroutine_saved_registers_count = 8; // rbp rbx rsi rdi r12 r13 r14 r15
	const int register_size = 8;

	state->sp =
		(char *)state->stack_base + stack_size
		- coroutine_saved_registers_count * register_size
		- register_size // return address
		- 16; // shadow space
	state->resume = coroutine;
	*(void **)((char *)state->sp + register_size * (coroutine_saved_registers_count - 0)) = coro_return; // return address
	*(void **)((char *)state->sp + register_size * (coroutine_saved_registers_count - 1)) = 0; // rbp
#elif defined(_WIN32)
	const int coroutine_saved_registers_count = 4; // ebp ebx esi edi
	const int register_size = 4;

	state->sp =
		(char *)state->stack_base + stack_size
		- coroutine_saved_registers_count * register_size
		- 8 // state pointer + parameter
		- register_size; // return address
	state->resume = coroutine;
	*(void **)((char *)state->sp + register_size * (coroutine_saved_registers_count + 2)) = 0; // parameter
	*(void **)((char *)state->sp + register_size * (coroutine_saved_registers_count + 1)) = state; // state pointer
	*(void **)((char *)state->sp + register_size * (coroutine_saved_registers_count + 0)) = coro_return; // return address
	*(void **)((char *)state->sp + register_size * (coroutine_saved_registers_count - 1)) = 0; // ebp
#else
#error coro.h does not support this platform
#endif

	*_state = state;
}

void coro_error() {
	assert(!"Attempt to run a coroutine that has already finished.");
}
