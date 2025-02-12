@echo off
mkdir ..\build
pushd ..\build
pwd
cl -FC -Zi ..\..\Lunora\source\engine.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res 
popd

