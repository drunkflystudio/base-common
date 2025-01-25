@echo off
setlocal disabledelayedexpansion
if exist bcc32-failed del bcc32-failed
("%~dp0..\..\..\tools\borland45\bin\bcc32" %* || echo 1 > bcc32-failed) ^
    | "%~dp0..\..\..\tools\gnuwin32\grep" ^
        -v "Borland C++ 4.52 for Win32 Copyright (c) 1993, 1994 Borland International" ^
    | "%~dp0..\..\..\tools\gnuwin32\grep" ^
        -v "Turbo Link  Version 1.50 for Win32 Copyright (c) 1993,1994 Borland International" ^
    | "%~dp0..\..\..\tools\gnuwin32\grep" ^
        -v -E "%~d0[^.]*(.tab)?.c:$"
if exist bcc32-failed (
    del bcc32-failed
    exit /B 1
    )
