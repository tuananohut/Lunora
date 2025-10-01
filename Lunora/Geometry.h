#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <string>

#include "Renderer.h"

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

struct SimpleVertexCombined
{
  XMFLOAT3 position;
  XMFLOAT4 color; 
};

struct ModelBuffer
{  
  ID3D11Buffer *VertexBuffer = NULL;
  ID3D11Buffer *IndexBuffer = NULL;
};

HRESULT CreateVertexBuffer(ID3D11Device& Device, ID3D11Buffer &VertexBuffer);
HRESULT CreateIndexBuffer(ID3D11Device& Device, ID3D11Buffer &IndexBuffer);

bool InitializeModel(ID3D11Device& Device, ModelBuffer& Buffer); 

void RenderModel(CoreRenderBuffers& RenderBuffers, ModelBuffer& Buffer); 

void ReleaseModel(ModelBuffer& Buffer); 
