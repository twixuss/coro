#pragma once
#include <stddef.h>

typedef struct coro_state coro_state;

typedef size_t (*coroutine_t)(coro_state *, size_t);

#ifdef __cplusplus
extern "C" {
#endif

size_t coro_yield(coro_state *state, size_t value);
void coro_return(void);
void coro_error(void);

#ifdef __cplusplus
}
#endif

void coro_init(coro_state **_state, coroutine_t coroutine, size_t stack_size);
int has_finished(coro_state *state);

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
#define CORO_MALLOC(x) malloc(x)
#define CORO_FREE(x)   free(x)
#endif

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifndef CORO_DEBUG
#define CORO_DEBUG 1
#endif

#if CORO_DEBUG
#ifndef CORO_DEBUG_SIZE
#define CORO_DEBUG_SIZE 128
#endif
#else
#define CORO_DEBUG_SIZE 0
#endif


#ifdef _WIN64

	//
	// Windows 64-bit
	//

struct coro_state {
	void *sp;
	void *buffer_base;
};

static const size_t coro_extra_stack_space = 112;

static void coro_init_platform(coro_state *state, coroutine_t coroutine) {
	void **sp = (void **)state->sp;
	sp[0] = 0; // r15
	sp[1] = 0; // r14
	sp[2] = 0; // r13
	sp[3] = 0; // r12
	sp[4] = 0; // rdi
	sp[5] = 0; // rsi
	sp[6] = 0; // rbx
	sp[7] = 0; // rbp
	sp[8] = coroutine;   // first yield address
	sp[9] = coro_return; // last  yield address
	sp[10] = 0; // shadow space
	sp[11] = 0; // shadow space
	sp[12] = 0; // shadow space
	sp[13] = 0; // shadow space
}

#elif defined(_WIN32)

	//
	// Windows 32-bit
	//

struct coro_state {
	void *sp;
	void *buffer_base;
};

static const size_t coro_extra_stack_space = 36;

static void coro_init_platform(coro_state *state, coroutine_t coroutine) {
	void **sp = state->sp;
	sp[0] = 0; // edi
	sp[1] = 0; // esi
	sp[2] = 0; // ebx
	sp[3] = 0; // ebp
	sp[4] = coroutine;   // first yield address
	sp[5] = coro_return; // last  yield address
	sp[6] = state; // first argument
	sp[7] = 0; // second argument
	sp[8] = 0; // did yield once
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

struct coro_state {
	void *sp;
	void *buffer_base;
};

static const size_t coro_extra_stack_space = 64;

static void coro_init_platform(coro_state *state, coroutine_t coroutine) {
	void **sp = state->sp;
	sp[0] = 0; // r15
	sp[1] = 0; // r14
	sp[2] = 0; // r13
	sp[3] = 0; // r12
	sp[4] = 0; // rbx
	sp[5] = 0; // rbp
	sp[6] = coroutine;   // first yield address
	sp[7] = coro_return; // last  yield address
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

	memset(g_debug_start = buffer_base, 0xcd, CORO_DEBUG_SIZE);
	memset(g_debug_end   = state + 1,   0xcd, CORO_DEBUG_SIZE);

	state->sp = (char *)state - coro_extra_stack_space;

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
