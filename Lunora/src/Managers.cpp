#include "Managers.h"

D3D11_INPUT_ELEMENT_DESC RenderManager::Layouts[3][2]; 

XMFLOAT4 UpdatePosition()
{
  // We need another solution for this

  XMFLOAT4 position = {0.f, 1.f, 0.f, 1.f};

  return position; 
}

D3D11_INPUT_ELEMENT_DESC (&RenderManager::ColorShader())[2]
{
  static D3D11_INPUT_ELEMENT_DESC PolygonLayoutColor[2]; 
  
  PolygonLayoutColor[0].SemanticName = "POSITION";
  PolygonLayoutColor[0].SemanticIndex = 0;
  PolygonLayoutColor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  PolygonLayoutColor[0].InputSlot = 0;
  PolygonLayoutColor[0].AlignedByteOffset = 0;
  PolygonLayoutColor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  PolygonLayoutColor[0].InstanceDataStepRate = 0;
  
  PolygonLayoutColor[1].SemanticName = "COLOR";
  PolygonLayoutColor[1].SemanticIndex = 0;
  PolygonLayoutColor[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  PolygonLayoutColor[1].InputSlot = 0;
  PolygonLayoutColor[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  PolygonLayoutColor[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  PolygonLayoutColor[1].InstanceDataStepRate = 0;

  Layouts[0][0] = PolygonLayoutColor[0]; 
  Layouts[0][1] = PolygonLayoutColor[1]; 

  return PolygonLayoutColor;
}

D3D11_INPUT_ELEMENT_DESC (&RenderManager::TextureShader())[2]
  {
    static D3D11_INPUT_ELEMENT_DESC PolygonLayoutTexture[2];

    PolygonLayoutTexture[0].SemanticName = "POSITION";
    PolygonLayoutTexture[0].SemanticIndex = 0;
    PolygonLayoutTexture[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    PolygonLayoutTexture[0].InputSlot = 0;
    PolygonLayoutTexture[0].AlignedByteOffset = 0;
    PolygonLayoutTexture[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    PolygonLayoutTexture[0].InstanceDataStepRate = 0;
  
    PolygonLayoutTexture[1].SemanticName = "TEXTURE";
    PolygonLayoutTexture[1].SemanticIndex = 0;
    PolygonLayoutTexture[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    PolygonLayoutTexture[1].InputSlot = 0;
    PolygonLayoutTexture[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    PolygonLayoutTexture[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    PolygonLayoutTexture[1].InstanceDataStepRate = 0;

    Layouts[0][0] = PolygonLayoutTexture[0]; 
    Layouts[0][1] = PolygonLayoutTexture[1]; 

    return PolygonLayoutTexture;
  }

D3D11_INPUT_ELEMENT_DESC (&RenderManager::LightShader())[2]
  {
    static D3D11_INPUT_ELEMENT_DESC PolygonLayoutLight[2];

    PolygonLayoutLight[0].SemanticName = "POSITION";
    PolygonLayoutLight[0].SemanticIndex = 0;
    PolygonLayoutLight[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    PolygonLayoutLight[0].InputSlot = 0;
    PolygonLayoutLight[0].AlignedByteOffset = 0;
    PolygonLayoutLight[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    PolygonLayoutLight[0].InstanceDataStepRate = 0;
  
    PolygonLayoutLight[1].SemanticName = "NORMAL";
    PolygonLayoutLight[1].SemanticIndex = 0;
    PolygonLayoutLight[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    PolygonLayoutLight[1].InputSlot = 0;
    PolygonLayoutLight[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    PolygonLayoutLight[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    PolygonLayoutLight[1].InstanceDataStepRate = 0;

    Layouts[0][0] = PolygonLayoutLight[0]; 
    Layouts[0][1] = PolygonLayoutLight[1]; 

    return PolygonLayoutLight;
  }
