@echo off

IF NOT EXIST build mkdir build
pushd build
pwd

cl -EHsc -FC -Zi ..\..\Lunora\Lunora\*.cpp ..\..\Lunora\Lunora\Core\Device\DeviceManager.cpp ..\..\Lunora\Lunora\Core\Pipeline\PipelineStateManager.cpp ..\..\Lunora\Lunora\Core\Resource\RenderTargetManager.cpp ..\..\Lunora\Lunora\Camera\Camera.cpp ..\..\Lunora\Lunora\Shader\Shader.cpp ..\..\Lunora\Lunora\Shader\ColorShader.cpp ..\..\Lunora\Lunora\Mesh\MeshData.cpp ..\..\Lunora\Lunora\Mesh\Terrain\TerrainMeshData.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res 

cl -EHsc -FC -Zi ..\..\Lunora\Lunora\Terrain\*.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res /link /SUBSYSTEM:WINDOWS

cl -EHsc -FC -Zi ..\..\Lunora\LunoraEngine\*.cpp user32.lib Gdi32.lib kernel32.lib ..\data\resource.res /link /SUBSYSTEM:WINDOWS

popd
