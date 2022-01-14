#define CORO_IMPL
#include <coro.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

size_t simple_coro(coro_state *state, size_t param) {
	int x = 4;
	int y = 5;
	int z = 6;
	// Note that param will always have the value of the first call to coro_yield!
	// To get new data from the caller, use the return value of coro_yield.
	printf("coro got %zu\n", param);                // 1
	printf("coro got %zu\n", coro_yield(state, x)); // 2
	printf("coro got %zu\n", coro_yield(state, y)); // 3
	return z;
}

int main(void) {
	coro_state *state;
	coro_init(&state, simple_coro, 1024*4);
	printf("main got %zu\n", coro_yield(state, 1)); // 4
	printf("main got %zu\n", coro_yield(state, 2)); // 5
	printf("main got %zu\n", coro_yield(state, 3)); // 6
	// printf("main got %zu\n", coro_yield(state, 0)); // this will assert as the coroutine has already finished.
	coro_free(&state);
	return 0;
}
