@echo off
nasm -fwin64 -gcv8 src\yield_win64.asm -o obj\yield_win64.obj
cl /nologo /Od /ZI examples\simple.c /I"include" obj\yield_win64.obj /link /out:examples\simple_win64.exe