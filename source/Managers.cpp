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
