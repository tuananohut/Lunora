@echo off

IF NOT EXIST build mkdir build
pushd build
pwd

cl -EHsc -FC -Zi /FeLunora.exe ..\..\Lunora\Lunora\Engine.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res

cl -EHsc -FC -Zi ..\..\Lunora\Lunora\Terrain\*.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res /link /SUBSYSTEM:WINDOWS /OUT:Terrain.exe

cl -EHsc -FC -Zi ..\..\Lunora\LunoraEngine\*.cpp user32.lib Gdi32.lib kernel32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res /link /SUBSYSTEM:WINDOWS /OUT:LunoraEngine.exe

popd
