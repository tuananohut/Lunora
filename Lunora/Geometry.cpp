#include "Geometry.h"

static ID3D11Buffer* g_pVertexBuffer = NULL;
static ID3D11Buffer *g_pIndexBuffer = NULL;

HRESULT CreateVertexBuffer(CoreRenderBuffers& RenderBuffers)
{
  HRESULT hr;

  SimpleVertexCombined *vertices;;

  vertices = new SimpleVertexCombined[3];
  if (!vertices)
    {
      return false;
    }

  vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f); 
  vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

  vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
  vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

  vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
  vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
  
  D3D11_BUFFER_DESC bufferDesc;
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(SimpleVertexCombined) * 3;
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = vertices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;

  hr = RenderBuffers.Device->CreateBuffer( &bufferDesc, &InitData, &g_pVertexBuffer );

  delete []vertices; 
  
  return hr; 
}

HRESULT CreateIndexBuffer(CoreRenderBuffers& RenderBuffers)
{
  HRESULT hr;
  
  unsigned long *indices;

  indices = new unsigned long[3];
  if (!indices)
    {
      return false;
    }
  
  indices[0] = 0;  
  indices[1] = 1;  
  indices[2] = 2;  
 
  D3D11_BUFFER_DESC bufferDesc;
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(unsigned long) * 3;
  bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = indices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;

  hr = RenderBuffers.Device->CreateBuffer( &bufferDesc, &InitData, &g_pIndexBuffer );

  delete[] indices; 
  
  return hr; 
}

void RenderModel(CoreRenderBuffers& RenderBuffers)
{
  UINT stride = sizeof( SimpleVertexCombined );
  UINT offset = 0;
  
  RenderBuffers.DeviceContext->IASetVertexBuffers(0, 
						  1,
						  &g_pVertexBuffer,
						  &stride, 
						  &offset );
  
  RenderBuffers.DeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
  
  
  
  RenderBuffers.DeviceContext->RSSetViewports(1, &RenderBuffers.Viewport);
  
  RenderBuffers.DeviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

