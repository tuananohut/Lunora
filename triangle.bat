@echo off

IF NOT EXIST triangle mkdir triangle
pushd triangle
pwd

cl -EHsc -FC -Zi ..\..\Lunora\Triangle\*.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res /link /SUBSYSTEM:WINDOWS /OUT:triangle.exe

popd
