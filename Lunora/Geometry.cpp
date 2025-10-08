#include "Geometry.h"

SimpleVertexCombined *vertices = nullptr;
unsigned long *indices;
int vertexCount = 0;
int indexCount = 0;

HRESULT CreateVertexBuffer(ID3D11Device *Device, ID3D11Buffer **VertexBuffer)
{
  HRESULT hr;

  vertices = new SimpleVertexCombined[3];
  if (!vertices)
    {
      return false;
    }

   /*
   vertices[0].position = XMFLOAT3(-1.f, -1.f, 0.f); 
   vertices[0].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

   vertices[1].position = XMFLOAT3(0.f, 1.f, 0.f);
   vertices[1].color = XMFLOAT4(0.f, 1.f, 1.f, 1.f);

   vertices[2].position = XMFLOAT3(1.f, -1.f, 0.f);
   vertices[2].color = XMFLOAT4(0.f, 0.f, 1.f, 1.f);
   */

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
  
  delete []vertices; 
  
  return hr; 
}

 HRESULT CreateIndexBuffer(ID3D11Device* Device, ID3D11Buffer** IndexBuffer)
 {
   HRESULT hr;

   /*unsigned long *indices;

   indices = new unsigned long[3];
   if (!indices)
     {
       return false;
     }

   indices[0] = 0;  
   indices[1] = 1;  
   indices[2] = 2;  */

   unsigned long indices[] = {
	 0, 1, 2,
	 0, 2, 3,

	 4, 6, 5,
	 4, 7, 6,

	 4, 5, 1,
	 4, 1, 0,

	 3, 2, 6,
	 3, 6, 7,

	 1, 5, 6,
	 1, 6, 2,

	 4, 0, 3,
	 4, 3, 7
     };

   D3D11_BUFFER_DESC bufferDesc = {};
   bufferDesc.Usage = D3D11_USAGE_DEFAULT;
   bufferDesc.ByteWidth = sizeof(indices);
   bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
   bufferDesc.CPUAccessFlags = 0;
   bufferDesc.MiscFlags = 0;

   D3D11_SUBRESOURCE_DATA InitData = {};
   InitData.pSysMem = indices;
   InitData.SysMemPitch = 0;
   InitData.SysMemSlicePitch = 0;

   hr = Device->CreateBuffer( &bufferDesc, &InitData, IndexBuffer );

   // delete[] indices; 

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

bool InitializeModel(CoreRenderBuffers& RenderBuffers, ModelBuffer* ModelBuffer,
		     const char* filename)
{
  HRESULT result;

  result = LoadModelFromFile(filename, &vertices, &vertexCount, &indices, &indexCount);
  if (FAILED(result))
    {
      return false; 
    }
  
  result = CreateVertexBuffer(RenderBuffers.Device, &ModelBuffer->VertexBuffer); 
  if (FAILED(result))
    {
      return false; 
    }
	  
  result = CreateIndexBuffer(RenderBuffers.Device, &ModelBuffer->IndexBuffer);
  if (FAILED(result))
    {
      return false; 
    }

  return true;
}

void ReleaseModel(ModelBuffer& Buffer)
{
  if (Buffer.VertexBuffer)
    {
      Buffer.VertexBuffer->Release();
      Buffer.VertexBuffer = nullptr; 
    }
  if (Buffer.IndexBuffer)
    {
      Buffer.IndexBuffer->Release();
      Buffer.IndexBuffer = nullptr; 
    }
}
