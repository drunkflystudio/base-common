@echo off
setlocal enabledelayedexpansion
set args=
set file=
set skip=1
for %%f in (%*) do (
    set "f=%%f"
    set "f=!f:/=\!"
    if q!skip!==q1 set file=!f!
    if q!skip!==q0 set args=!args! "+!f!"
    set skip=0
)
if exist tlib-failed del tlib-failed
("%~dp0..\..\..\tools\borland45\bin\tlib" /p512 "%file%" %args% || echo 1 > tlib-failed) ^
    | "%~dp0..\..\..\tools\gnuwin32\grep" ^
        -v "TLIB 4.00 Copyright (c) 1987, 1994 Borland International"
if exist tlib-failed (
    del tlib-failed
    exit /B 1
    )
