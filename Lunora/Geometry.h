#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "Renderer.h"
#include "ModelLoader.h"

struct HexMeshVertexBuffer
{
  XMFLOAT3 position;
  XMFLOAT2 texture; 
  XMFLOAT3 normal;
  XMFLOAT3 tangent;
  XMFLOAT3 binormal; 
  XMFLOAT3 color; 
};

struct MeshVertexBuffer
{
  XMFLOAT3 position;
  XMFLOAT2 texture; 
  XMFLOAT3 normal;
  XMFLOAT3 tangent;
  XMFLOAT3 binormal; 
  XMFLOAT3 color; 
};
/*
struct SimpleVertexCombined
{
  XMFLOAT3 position;
  XMFLOAT4 color; 
};
*/
struct ModelBuffer
{  
  ID3D11Buffer *VertexBuffer = nullptr;
  ID3D11Buffer *IndexBuffer = nullptr;
};

HRESULT CreateVertexBuffer(ID3D11Device *Device, ID3D11Buffer** VertexBuffer);
HRESULT CreateIndexBuffer(ID3D11Device *Device, ID3D11Buffer** IndexBuffer);

bool InitializeModel(CoreRenderBuffers& RenderBuffers, ModelBuffer* Buffer); 

void RenderModel(CoreRenderBuffers& RenderBuffers, ModelBuffer& Buffer); 

void ReleaseModel(ModelBuffer& Buffer); 

#endif
