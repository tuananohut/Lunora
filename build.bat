@echo off

IF NOT EXIST build mkdir build
pushd build

SET cppFilenames=
FOR /R %%f in (*.cpp) do (
    SET cppFilenames=!cppFilenames! %%f
)

SET assembly=engine

cl -EHsc -FC -Zi /FeLunora.exe c:\dev\Lunora\Lunora\main.cpp c:\dev\Lunora\Lunora\Rendering\*.cpp c:\dev\Lunora\Lunora\Rendering\Shader\*.cpp c:\dev\Lunora\Lunora\Engine\src\Renderer.cpp c:\dev\Lunora\Lunora\Game\Camera\Camera.cpp c:\dev\Lunora\Lunora\Game\Entity.cpp c:\dev\Lunora\Lunora\Game\Scene.cpp user32.lib Gdi32.lib d3d11.lib d3dcompiler.lib dxgi.lib c:\dev\Lunora\data\resource.res

popd
