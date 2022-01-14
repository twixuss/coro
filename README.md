# Simple Coroutine For C and C++
## C Example
```c
#include <coro.h>

size_t simple_coro(coro_state *state, size_t param) {
	int x = 59;
	coro_yield(state, 32);
	coro_yield(state, x);
	return 123;
}

coro_state *state;                    // stores coroutine's stack and everyting needed for transition
coro_init(state, simple_coro, 1024);  // third parameter is stack size
printf("%zu\n", coro_yield(state, 0)); // will print 32
printf("%zu\n", coro_yield(state, 0)); // will print 59
printf("%zu\n", coro_yield(state, 0)); // will print 123
// printf("%zu\n", coro_yield(state, 0)); // this will trigger an assert, because the coroutine has already finished.
```
## Building
Replace star with appropriate target platform.
- Build `src\yield_*.asm`, or link with `obj\yield_*.obj`.
- Add `include` directory to your compiler.
- Compile `src\coro.c`.
## Available platforms
- ✔️ win32
- ✔️ win64
- ⚠️ linux64 (`coro_error` segfaults instead of asserting)
