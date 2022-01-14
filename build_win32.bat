@echo off
nasm -fwin32 src\yield_win32.asm -o obj\yield_win32.obj
cl /nologo /ZI examples\simple.c src\coro.c /I"include" obj\yield_win32.obj /link /out:examples\simple_win32.exe