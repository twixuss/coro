build:
	nasm src/yield_linux64.asm -felf64 -o obj/yield_linux64.o
	gcc -g examples/simple.c src/coro.c obj/yield_linux64.o -o examples/simple_linux64 -I "include"

