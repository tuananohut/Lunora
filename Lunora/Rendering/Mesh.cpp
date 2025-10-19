#include "Mesh.h"

HRESULT CreateVertexBuffer(ID3D11Device *Device, SimpleVertexCombined* vertices,
			   int vertexCount, ID3D11Buffer** VertexBuffer)
{
  HRESULT hr;

  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(SimpleVertexCombined) * vertexCount;
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;
  
  D3D11_SUBRESOURCE_DATA InitData = {};
  InitData.pSysMem = vertices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;
  
  hr = Device->CreateBuffer( &bufferDesc, &InitData, VertexBuffer );
  
  return hr; 
}

HRESULT CreateIndexBuffer(ID3D11Device *Device, unsigned long* indices,
			  int indexCount, ID3D11Buffer** IndexBuffer)
{
  HRESULT hr;
 
  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
  bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData = {};
  InitData.pSysMem = indices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;
  
  hr = Device->CreateBuffer( &bufferDesc, &InitData, IndexBuffer );

  return hr; 
}

void RenderModel(RendererContext& RenderBuffers, Mesh& Buffer)
{
  UINT stride = sizeof( SimpleVertexCombined );
  UINT offset = 0;

  RenderBuffers.DeviceContext->IASetVertexBuffers(0, 
						  1,
						  &Buffer.vertexBuffer,
						  &stride, 
						  &offset );
  
  RenderBuffers.DeviceContext->IASetIndexBuffer(Buffer.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  RenderBuffers.DeviceContext->RSSetViewports(1, &RenderBuffers.Viewport);

  RenderBuffers.DeviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool InitializeModel(RendererContext& RenderBuffers, Mesh* ModelBuffer,
		     char filename[])
{
  HRESULT result; 
  SimpleVertexCombined* localVertices; 
  unsigned long* localIndices = nullptr;
  
  bool loaded = LoadModelFromFile(filename, &localVertices, &ModelBuffer->vertexCount,
				  &localIndices, &ModelBuffer->indexCount);
  if (!loaded)
    {
      return false; 
    }
  
  result = CreateVertexBuffer(RenderBuffers.Device, localVertices, ModelBuffer->vertexCount,
			      &ModelBuffer->vertexBuffer); 
  if (FAILED(result))
    {
      return false; 
    }
	  
  result = CreateIndexBuffer(RenderBuffers.Device, localIndices, ModelBuffer->indexCount,
			     &ModelBuffer->indexBuffer);
  if (FAILED(result))
    {
      return false; 
    }

  delete[] localVertices;
  delete[] localIndices;
  localVertices = nullptr;
  localIndices = nullptr; 
  
  return true;
}

void ReleaseModel(Mesh& Buffer)
{
  if (Buffer.vertexBuffer)
    {
      Buffer.vertexBuffer->Release();
      Buffer.vertexBuffer = nullptr; 
    }
  if (Buffer.indexBuffer)
    {
      Buffer.indexBuffer->Release();
      Buffer.indexBuffer = nullptr; 
    }
}
