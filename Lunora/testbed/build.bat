@echo off
SetLocal EnableDelayedExpansion

set "BIN=c:\dev\Lunora\build\bin"
set "SRC=c:\dev\Lunora\Lunora\testbed\src"
set "ENGINE_INC=c:\dev\Lunora\Lunora\Engine\src"

IF NOT EXIST "%BIN%" mkdir "%BIN%"
pushd "%BIN%"

SET cppFilenames="c:\dev\Lunora\Lunora\testbed\src\main.cpp"
REM FOR /R "%root%" %%f in (*.cpp) do (
REM    SET cppFilenames=!cppFilenames! %%f
REM )

SET assembly=testbed
SET compilerFlags=-EHsc -FC -Zi
SET includeFlags=-Isrc -I"c:\dev\Lunora\Lunora\Engine\src"
SET linkerFlags=-link /LIBPATH:"c:\dev\build\bin"
SET defines=-D_DEBUG -DLIMPORT

echo "Building %assembly%..."
cl /EHsc /Zi /FC /MDd -D_DEBUG -DLIMPORT %cppFilenames% -I"c:\dev\Lunora\Lunora\Engine\src" -link /LIBPATH:"c:\dev\build\bin" Engine.lib /OUT:testbed.exe
