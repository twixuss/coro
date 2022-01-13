#include <coro.h>
inline void coro_platform_init(coro_state *state, size_t (*coroutine)(coro_state *, size_t), size_t stack_size) {
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
}
