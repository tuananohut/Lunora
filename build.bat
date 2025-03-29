@echo off

IF NOT EXIST build mkdir build
pushd build
pwd

cl -FC -Zi ..\..\Lunora\Lunora\src\engine.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res 

popd
