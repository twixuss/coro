@echo off
nasm -fwin32 -gcv8 src\yield_win32.asm -o obj\yield_win32.obj
cl /nologo /Zi examples\simple.c /I"include" obj\yield_win32.obj /link /out:examples\simple_win32.exe