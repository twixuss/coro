#define CORO_IMPL
#include <coro.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

size_t simple_coro(coro_state *state, size_t param) {
	// Note that param will always have the value of the first call to coro_yield!
	// To get new data from the caller, use the return value of coro_yield.

	size_t yield_value  = 999;
	size_t return_value = 777;

	printf("coro: starting with '%zu'\n", param);

	printf("coro: yielding '%zu'\n", yield_value);

	size_t value_from_main = coro_yield(state, yield_value);

	printf("coro: got '%zu' from main\n", value_from_main);

	printf("coro: param is still '%zu'\n", param);

	printf("coro: returning '%zu'\n", return_value);
	return return_value;
}

int main(void) {
	coro_state *state;
	coro_init(&state, simple_coro, 1024*1024);


	printf("main: starting coroutine with argument '123'\n");

	size_t result = coro_yield(state, 123);

	printf("main: coroutine yielded '%zu'\n", result);


	printf("main: calling coroutine the last time with '456'...\n");

	result = coro_yield(state, 456);

	printf("main: coroutine finished with '%zu'\n", result);

	coro_yield(state, 0); // excessive call to coro_yield will result in assertion failure.

	coro_free(&state);
	return 0;
}
