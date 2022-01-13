#include <coro.h>
inline void coro_platform_init(coro_state *state, size_t (*coroutine)(coro_state *, size_t), size_t stack_size) {
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
}
