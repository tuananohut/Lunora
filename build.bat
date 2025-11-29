@echo off

IF NOT EXIST build mkdir build
pushd build

SET cppFilenames=
FOR /R %%f in (*.cpp) do (
    SET cppFilenames=!cppFilenames! %%f
)

SET assembly=engine

cl -EHsc -FC -Zi /FeLunora.exe ..\..\Lunora\Lunora\main.cpp ..\..\Lunora\Lunora\Rendering\Texture.cpp ..\..\Lunora\Lunora\Rendering\Shader\Shader.cpp ..\..\Lunora\Lunora\Engine\Renderer.cpp ..\..\Lunora\Lunora\Rendering\Mesh.cpp ..\..\Lunora\Lunora\Game\Camera\Camera.cpp ..\..\Lunora\Lunora\Game\Entity.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib ..\data\resource.res

popd
