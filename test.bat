@echo off
setlocal
call "F:\exec\vcvars32.bat"
call build_win32
examples\simple_win32.exe
call "F:\exec\vcvars.bat"
call build_win64
examples\simple_win64.exe