#include "Managers.h"

bool RenderManager::layoutsInitialized = false;

D3D11_INPUT_ELEMENT_DESC* RenderManager::ColorLayout = nullptr;
D3D11_INPUT_ELEMENT_DESC* RenderManager::TextureLayout = nullptr;
D3D11_INPUT_ELEMENT_DESC* RenderManager::LightLayout = nullptr;

RenderManager::RenderManager()
{
    if (!layoutsInitialized)
    {
        InitializeLayouts();
        layoutsInitialized = true;
    }
}

RenderManager::~RenderManager() {}

void RenderManager::InitializeLayouts()
{
    ColorLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
    ColorLayout[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

    TextureLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
    TextureLayout[1] = { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

    LightLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
    LightLayout[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

D3D11_INPUT_ELEMENT_DESC* RenderManager::GetLayout(ShaderLayoutType type)
{
    switch (type)
    {
    case ShaderLayoutType::Color:   return ColorLayout;
    case ShaderLayoutType::Texture: return TextureLayout;
    case ShaderLayoutType::Light:   return LightLayout;
    default:                        return nullptr;
    }
}

unsigned int RenderManager::GetLayoutElementCount(ShaderLayoutType type)
{
    return 2;
}
