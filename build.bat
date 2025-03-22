@echo off

IF NOT EXIST build mkdir build
pushd build
pwd

cl -FC -Zi /D LNR_BUILD_DLL /LD /FeLunora.dll ..\..\Lunora\Lunora\src\Lunora\Application.cpp  

cl -FC -Zi /D LNR_BUILD_DLL /I..\..\Lunora\Lunora\src ..\..\Lunora\Sandbox\engine.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res 

cl -FC -Zi /D LNR_BUILD_DLL /I..\..\Lunora\Lunora\src ..\..\Lunora\Sandbox\SandboxApp.cpp Lunora.lib
popd
