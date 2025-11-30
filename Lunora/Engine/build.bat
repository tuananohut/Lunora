@echo off
SetLocal EnableDelayedExpansion

set "BIN=c:\dev\Lunora\build\bin"
set "SRC=c:\dev\Lunora\Lunora\Engine\src"

IF NOT EXIST "%BIN%" mkdir "%BIN%"
pushd "%BIN%"

del /Q *.obj >nul 2>&1
del /Q Engine.dll >nul 2>&1
del /Q Engine.lib >nul 2>&1
del /Q Engine.exp >nul 2>&1

set cppFiles
for %%f in ("%SRC%\*.cpp") do (
    set "cppFiles=!cppFiles! "%%f""
)

SET "objFiles=c:\dev\Lunora\build\bin"

SET assembly=Engine
SET linkerFlags=user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib
SET defines=-D_DEBUG -DLEXPORT -D_CRT_SECURE_NO_WARNINGS
set "ENG_OBJS="
for /R "%objFiles%" %%o in (*.obj) do (
    set "ENG_OBJS=!ENG_OBJS! "%%o""
)


cl /c /EHsc /Zi /FC /MDd -D_DEBUG -DLEXPORT -D_CRT_SECURE_NO_WARNINGS %cppFiles% %linkerFlags%

cl /LD *.obj user32.lib gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib /FeEngine.dll

popd
