#include <DirectXMath.h>
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

ID3D11Buffer* g_pVertexBuffer;

struct SimpleVertexCombined
{
  XMFLOAT3 Pos;
  XMFLOAT3 Col; 
};

SimpleVertexCombined verticesCombo[] =
  {
    XMFLOAT3(  0.f ,  0.5f,  0.5f  ),
    XMFLOAT3(  0.f ,  0.f ,  0.5f  ),
    XMFLOAT3(  0.5f, -0.5f,  0.5f  ),
    XMFLOAT3(  0.5f, -0.5f,  0.5f  ),
    XMFLOAT3( -0.5f, -0.5f,  0.5f  ),
    XMFLOAT3(  0.f,   0.5f,  0.f   ),
  };

HRESULT CreateVertexBuffer(CoreRenderBuffers& RenderBuffers)
{
  HRESULT hr; 
  
  D3D11_BUFFER_DESC bufferDesc;
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(SimpleVertexCombined) * _countof(verticesCombo);
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = verticesCombo;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;

  hr = RenderBuffers.Device->CreateBuffer( &bufferDesc, &InitData, &g_pVertexBuffer );
  
  return hr; 
}

ID3D11Buffer *g_pIndexBuffer = NULL;

unsigned int indices[] = { 0, 1, 2 };


HRESULT CreateIndexBuffer(CoreRenderBuffers& RenderBuffers)
{
  HRESULT hr; 
  
  D3D11_BUFFER_DESC bufferDesc;
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(unsigned int) * _countof(indices);
  bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = indices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;

  hr = RenderBuffers.Device->CreateBuffer( &bufferDesc, &InitData, &g_pIndexBuffer );
  
  return hr; 
}
