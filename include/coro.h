#pragma once

typedef struct {
	void *resume;
	void *sp;
} coro_state;

#ifdef __cplusplus
extern "C" {
#endif

void *t_coro_yield(coro_state *state, void *value);
	
#ifdef __cplusplus
}
#endif
