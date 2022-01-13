build:
	nasm src/yield_linux32.asm -felf32 -o obj/yield_linux32.o
	gcc examples/simple.c src/coro.c obj/yield_linux32.o -o examples/simple-linux32 -I "include"

