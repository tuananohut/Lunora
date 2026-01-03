#include "HemisphericMesh.h"


bool HemisphericMeshCreateVertexBuffer(ID3D11Device* device, HemisphericMesh* mesh)
{
  HRESULT result;
  
  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(HemisphericMesh) * Mesh->vertexCount;
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;
  
  D3D11_SUBRESOURCE_DATA InitData = {};
  InitData.pSysMem = Mesh->vertices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;
  
  result = Device->CreateBuffer( &bufferDesc, &InitData, &Mesh->vertexBuffer );
  if (FAILED(result))
    reutrn false; 
  
  return true; 

}

bool HemisphericMeshCreateIndexBuffer(ID3D11Device* device, HemisphericMesh* mesh)
{
  HRESULT hr;
 
  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(uint32_t) * Mesh->indexCount;
  bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData = {};
  InitData.pSysMem = Mesh->indices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;
  
  hr = Device->CreateBuffer( &bufferDesc, &InitData, &Mesh->indexBuffer );
  if (FAILED(hr))
    return false; 
  
  return true; 
}


bool HemisphericMeshInitialize(ID3D11Device *Device, HemisphericMesh* ModelBuffer)
{
  HRESULT result;
 
  bool loaded = HemisphericMeshLoadFromFile(ModelBuffer);
  if (!loaded)
    {
      return false; 
    } 
  
  result = HemisphericMeshCreateVertexBuffer(Device, ModelBuffer); 
  if (FAILED(result))
    {
      return false; 
    }
	  
  result = HemisphericMeshCreateIndexBuffer(Device, ModelBuffer);
  if (FAILED(result))
    {
      return false; 
    }

  return true;
}

void HemisphericMeshRender(ID3D11Device* device, HemisphericMesh* mesh)
{
  uint32_t stride = sizeof(HemisphericMesh);
  uint32_t offset = 0;

  RenderBuffers.DeviceContext->IASetVertexBuffers(0, 
						  1,
						  &mesh->vertexBuffer,
						  &stride, 
						  &offset );
  
  RenderBuffers.DeviceContext->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  RenderBuffers.DeviceContext->RSSetViewports(1, &RenderBuffers.Viewport);

  RenderBuffers.DeviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST ); 

}

void HemisphericMeshRelease(HemisphericMesh* Buffer)
{
  if (Buffer->vertexBuffer)
    {
      Buffer->vertexBuffer->Release();
      Buffer->vertexBuffer = nullptr; 
    }
  if (Buffer->indexBuffer)
    {
      Buffer->indexBuffer->Release();
      Buffer->indexBuffer = nullptr; 
    }
  
  delete[] Buffer->vertices;
  Buffer->vertices = nullptr;

  delete[] Buffer->indices;
  Buffer->indices = nullptr;

  Buffer->vertexCount = 0;
  Buffer->indexCount = 0;

}

bool HemisphericMeshLoadFromFile(HemisphericMesh* mesh)
{
  
}
