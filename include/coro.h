#pragma once
#include <stddef.h>

typedef struct {
	void *resume;
	void *sp;
	void *buffer_base;
	void *mark;
} coro_state;

typedef size_t (*coroutine_t)(coro_state *, size_t);

void coro_init(coro_state **_state, coroutine_t coroutine, size_t stack_size);
size_t coro_yield(coro_state *state, size_t value);
void coro_return(void);
void coro_error(void);

size_t *g_buffer_base;
size_t *g_buffer_end;
coro_state *g_coro_state;
size_t *g_debug_start;
size_t *g_debug_end;
size_t g_buffer_size;

#ifdef CORO_IMPL

#ifdef CORO_MALLOC
#ifndef CORO_FREE
#error CORO_MALLOC was defined, but CORO_FREE was not.
#endif
#else
#define CORO_MALLOC(x) _aligned_malloc(x, 16)
#define CORO_FREE(x)   _aligned_free(x)
#endif

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifndef CORO_DEBUG
#define CORO_DEBUG 1
#endif

#if CORO_DEBUG
#define CORO_DEBUG_SIZE 128
#else
#define CORO_DEBUG_SIZE 0
#endif


#ifdef _WIN64

	//
	// Windows 64-bit
	//

static const size_t coro_extra_stack_space = 104;

static void coro_init_platform(coro_state *state, coroutine_t coroutine) {
	const int saved_registers_count = 8; // rbp rbx rsi rdi r12 r13 r14 r15
	const int register_size = 8;

	state->sp = (char *)state
		- saved_registers_count * register_size
		- register_size // return address
		- 32; // shadow space
	state->resume = coroutine;
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 0)) = coro_return; // return address
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 1)) = 0; // rbp
}

#elif defined(_WIN32)

	//
	// Windows 32-bit
	//

static void coro_init_platform(coro_state *state, coroutine_t coroutine) {
	const int saved_registers_count = 4; // ebp ebx esi edi
	const int register_size = 4;

	state->sp = (char *)state
		- saved_registers_count * register_size
		- 8 // state pointer + parameter
		- register_size; // return address
	state->resume = coroutine;
	*(void **)((char *)state->sp + register_size * (saved_registers_count + 2)) = 0; // parameter
	*(void **)((char *)state->sp + register_size * (saved_registers_count + 1)) = state; // state pointer
	*(void **)((char *)state->sp + register_size * (saved_registers_count + 0)) = coro_return; // return address
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 1)) = 0; // ebp
}

#elif defined(__linux__)
#if __SIZE_MAX__ == 0xffffffff

	//
	// Linux 32-bit
	//

#error not implemented

#else

	//
	// Linux 64-bit
	//

static void coro_init_platform(coro_state *state, coroutine_t coroutine) {
	const int saved_registers_count = 6; // rbp rbx r12 r13 r14 r15
	const int register_size = 8;

	state->sp = (char *)state
		- saved_registers_count * register_size
		- register_size; // return address
	state->resume = coroutine;
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 0)) = coro_return; // return address
	*(void **)((char *)state->sp + register_size * (saved_registers_count - 1)) = 0; // rbp
}

#endif
#else

#error coro.c: invalid platform

#endif

void coro_init(coro_state **_state, coroutine_t coroutine, size_t stack_size) {
	size_t total_size = CORO_DEBUG_SIZE + stack_size + coro_extra_stack_space + sizeof(coro_state) + CORO_DEBUG_SIZE;
	char *buffer_base = (char *)CORO_MALLOC(total_size);
	char *buffer_end = buffer_base + total_size;

	g_buffer_base = buffer_base;
	g_buffer_end = buffer_end;
	g_buffer_size = total_size/8;

	coro_state *state = (coro_state *)(buffer_end - sizeof(coro_state) - CORO_DEBUG_SIZE);
	memset(state, 0, sizeof(*state));

	g_coro_state = state;

	state->buffer_base = buffer_base;

	state->mark = (void *)(size_t)0xdeadc0de;

	memset(g_debug_start = buffer_base, 0xcd, CORO_DEBUG_SIZE);
	memset(g_debug_end   = state + 1,   0xcd, CORO_DEBUG_SIZE);

	coro_init_platform(state, coroutine);

	*_state = state;
}

#define coro_debug_check(p, msg) \
	for (int i = 0; i < CORO_DEBUG_SIZE; ++i) \
		if (((char *)(p))[i] != (char)0xcd) \
			assert(!msg); \


void coro_free(coro_state **_state) {
	coro_state *state = *_state;
	*_state = 0;

	coro_debug_check(state->buffer_base, "coroutine stack overflow");
	coro_debug_check(state + 1, "coroutine stack origin corrupted");

	CORO_FREE(state->buffer_base);
}

void coro_error(void) {
	assert(!"Attempt to run a coroutine that has already finished.");
}

#endif
