@echo off
cd /D "%~dp0.." || exit /B 1
cmake -DBORLAND_4_5=release -P "%~dp0buildtool/buildtool.cmake" || exit /B 1
echo == SUCCESS! ==
