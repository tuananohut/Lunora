@echo off

IF NOT EXIST build mkdir build
pushd build
pwd

cl -EHsc -FC -Zi /FeLunora.exe ..\..\Lunora\Lunora\main.cpp ..\..\Lunora\Lunora\Rendering\Shader\Shader.cpp ..\..\Lunora\Lunora\Engine\Renderer.cpp ..\..\Lunora\Lunora\Game\ModelLoader.cpp ..\..\Lunora\Lunora\Rendering\Mesh.cpp ..\..\Lunora\Lunora\Game\Camera\Camera.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res

popd
