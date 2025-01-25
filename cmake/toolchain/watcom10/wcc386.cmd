@echo off
setlocal enabledelayedexpansion
set dp0=%~dp0
set args=
:loop
if "%~1"=="" goto end
set "f=%~1"
set "f=!f:/=\!"
set "f=!f:`==!"
set "args=!args! !f!"
shift
goto loop
:end
"%dp0%..\..\..\tools\watcom10\binnt\wcc386" %args% || exit /B 1
