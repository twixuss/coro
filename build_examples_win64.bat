@echo off
cl /nologo /ZI examples\simple.c src\coro.c /I"include" obj\yield_win64.obj /link /out:examples\simple64.exe