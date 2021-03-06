# Simple Coroutine For C and C++
This is an implementation of stackful coroutines
## Interface
There are 3 functions you need to know about:
```cpp
void coro_init(coro_state **state, coroutine_t coroutine, size_t stack_size)
```
Initializes coroutine stack.

```cpp
void coro_free(coro_state **_state);
```
Releases coroutine stack.

```cpp
size_t coro_yield(coro_state *state, size_t value);
```
Suspends execution, sending `value` to the coroutine. At the first yield, coroutine will receive `value` in it's parameter. Next time `value` will be returned from `coro_yield`.

## C Example
```c
#define CORO_IMPL // define this to bring in function definitions
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
- Include `include\coro.h` and define `CORO_IMPL` in one of your .c files.
## Available platforms
- ✔️ win32
- ✔️ win64
- ✔️ linux64
