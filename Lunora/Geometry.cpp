#include "Geometry.h"

HRESULT CreateVertexBuffer(ID3D11Device *Device, ID3D11Buffer **VertexBuffer)
{
  HRESULT hr;

  SimpleVertexCombined *vertices;;

  vertices = new SimpleVertexCombined[3];
  if (!vertices)
    {
      return false;
    }

  vertices[0].position = XMFLOAT3(-1.f, -1.f, 0.f); 
  vertices[0].color = XMFLOAT4(1.f, 1.f, 0.f, 1.f);

  vertices[1].position = XMFLOAT3(0.f, 1.f, 0.f);
  vertices[1].color = XMFLOAT4(0.f, 1.f, 1.f, 1.f);

  vertices[2].position = XMFLOAT3(1.f, -1.f, 0.f);
  vertices[2].color = XMFLOAT4(1.f, 0.f, 1.f, 1.f);
  
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

  hr = Device->CreateBuffer( &bufferDesc, &InitData, VertexBuffer );

  delete []vertices; 
  
  return hr; 
}

HRESULT CreateIndexBuffer(ID3D11Device* Device, ID3D11Buffer** IndexBuffer)
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

  hr = Device->CreateBuffer( &bufferDesc, &InitData, IndexBuffer );

  delete[] indices; 
  
  return hr; 
}

void RenderModel(CoreRenderBuffers& RenderBuffers, ModelBuffer& Buffer)
{
  UINT stride = sizeof( SimpleVertexCombined );
  UINT offset = 0;
  
  RenderBuffers.DeviceContext->IASetVertexBuffers(0, 
						  1,
						  &Buffer.VertexBuffer,
						  &stride, 
						  &offset );
  
  RenderBuffers.DeviceContext->IASetIndexBuffer(Buffer.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
  
  
  
  RenderBuffers.DeviceContext->RSSetViewports(1, &RenderBuffers.Viewport);
  
  RenderBuffers.DeviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool InitializeModel(CoreRenderBuffers& RenderBuffers, ModelBuffer* ModelBuffer)
{
  HRESULT result;
  
  result = CreateVertexBuffer(*RenderBuffers.Device, ModelBuffer); 
  if (FAILED(result))
    {
      return false; 
    }
	  
  result = CreateIndexBuffer(*RenderBuffers.Device, ModelBuffer);
  if (FAILED(result))
    {
      return false; 
    }

  return true;
}

void ReleaseModel(ModelBuffer& Buffer)
{
  if (ModelBuffer)
    {
      if (ModelBuffer.VertexBuffer)
	{
	  ModelBuffer.VertexBuffer->Release();
	  delete ModelBuffer.VertexBuffer;
	  ModelBuffer.VertexBuffer = nullptr; 
	}
      if (ModelBuffer.IndexBuffer)
	{
	  ModelBuffer.IndexBuffer->Release();
	  delete ModelBuffer.IndexBuffer;
	  ModelBuffer.IndexBuffer = nullptr; 
	}
    }
}
