@echo off
cl /nologo /ZI examples\simple.c src\coro.c /I"include" obj\yield_win32.obj /link /out:examples\simple32.exe