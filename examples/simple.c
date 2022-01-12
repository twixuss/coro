#include <stdio.h>
#include <stdint.h>
#include <coro.h>

size_t simple_coro(coro_state *state, size_t param) {
	coro_yield(state, 42);
	coro_yield(state, 123);
	return 4;
}

int main(void) {
	coro_state *state;
	coro_init(&state, simple_coro, 1024);
	printf("%zu\n", coro_yield(state, 0)); // 42
	printf("%zu\n", coro_yield(state, 0)); // 123
	printf("%zu\n", coro_yield(state, 0)); // 4
	return 0;
}
