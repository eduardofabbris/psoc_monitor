@echo off
rem https://www.makeuseof.com/tag/write-simple-batch-bat-file/
title build and run
if not exist "src" goto end
echo Building psoc project...
if not exist "objects" mkdir "objects"
for %%i in (src\*.c) do gcc %%i -o "objects\%%~ni.o" -Wall -Wextra -c
gcc -o monitor_psoc.exe "objects\*.o"
rem echo Opening project...
rem start main.exe
pause
exit
:end
echo Source folder not found!
pause
