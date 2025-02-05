@echo off

mkdir ..\..\build
pushd ..\..\build
pwd
cl -FC -Zi ..\handmade_series\code\win32_handmade.cpp user32.lib Gdi32.lib dinput8.lib dxguid.lib 
popd
