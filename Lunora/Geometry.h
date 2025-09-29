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

HRESULT CreateVertexBuffer(CoreRenderBuffers& RenderBuffers);
HRESULT CreateIndexBuffer(CoreRenderBuffers& RenderBuffers);

void RenderModel(CoreRenderBuffers& RenderBuffers); 

