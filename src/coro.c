#include <coro.h>
#include <string.h>
#include <coro_platform.h>

void coro_return();

void coro_init(coro_state **_state, size_t (*coroutine)(coro_state *, size_t), size_t stack_size) {
	char *stack_base = malloc(stack_size + sizeof(coro_state));
	coro_state *state = (coro_state *)(stack_base + stack_size);
	memset(state, 0, sizeof(*state));

	state->stack_base = stack_base;

	state->mark = (void *)0xdeadc0de;

	coro_platform_init(state, coroutine, stack_size);
	*_state = state;
}

void coro_error() {
	assert(!"Attempt to run a coroutine that has already finished.");
}
