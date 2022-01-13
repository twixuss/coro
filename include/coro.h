#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct {
	void *resume;
	void *sp;
	void *stack_base;
	void *mark;
} coro_state;

void coro_init(coro_state **_state, size_t (*coroutine)(coro_state *, size_t), unsigned long long stack_size);
size_t coro_yield(coro_state *state, size_t value);
void coro_error();

