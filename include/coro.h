#pragma once
#include <stddef.h>

typedef struct {
	void *resume;
	void *sp;
	void *stack_base;
	void *mark;
} coro_state;

void coro_init(coro_state **_state, size_t (*coroutine)(coro_state *, size_t), size_t stack_size);
size_t coro_yield(coro_state *state, size_t value);
void coro_return(void);
void coro_error(void);

