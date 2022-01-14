#define CORO_IMPL
#include <coro.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

size_t simple_coro(coro_state *state, size_t param) {
	int x = 123;
	coro_yield(state, 42);
	coro_yield(state, x);
	return 4;
}

int main(void) {
	coro_state *state;
	coro_init(&state, simple_coro, 1024);
	printf("%zu\n", coro_yield(state, 0)); // 42
	printf("%zu\n", coro_yield(state, 0)); // 123
	printf("%zu\n", coro_yield(state, 0)); // 4
	printf("%zu\n", coro_yield(state, 0)); // assertion fail
	return 0;
}
