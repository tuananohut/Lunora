#include "Mesh.h"

HRESULT CreateVertexBuffer(ID3D11Device *Device, Mesh* Mesh)
{
  HRESULT hr;
  
  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(Vertex) * Mesh->vertexCount;
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;
  
  D3D11_SUBRESOURCE_DATA InitData = {};
  InitData.pSysMem = Mesh->vertices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;
  
  hr = Device->CreateBuffer( &bufferDesc, &InitData, &Mesh->vertexBuffer );
  
  return hr; 
}

HRESULT CreateIndexBuffer(ID3D11Device *Device, Mesh* Mesh)
{
  HRESULT hr;
 
  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(unsigned long) * Mesh->indexCount;
  bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData = {};
  InitData.pSysMem = Mesh->indices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;
  
  hr = Device->CreateBuffer( &bufferDesc, &InitData, &Mesh->indexBuffer );

  return hr; 
}

void RenderModel(RendererContext& RenderBuffers, Mesh* Buffer)
{
  UINT stride = sizeof(Vertex);
  UINT offset = 0;

  RenderBuffers.DeviceContext->IASetVertexBuffers(0, 
						  1,
						  &Buffer->vertexBuffer,
						  &stride, 
						  &offset );
  
  RenderBuffers.DeviceContext->IASetIndexBuffer(Buffer->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  RenderBuffers.DeviceContext->RSSetViewports(1, &RenderBuffers.Viewport);

  RenderBuffers.DeviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool InitializeModel(ID3D11Device *Device, Mesh* ModelBuffer)
{
  HRESULT result;
  
  bool loaded = LoadModelFromFile(ModelBuffer);
  if (!loaded)
    {
      return false; 
    }
  
  result = CreateVertexBuffer(Device, ModelBuffer); 
  if (FAILED(result))
    {
      return false; 
    }
	  
  result = CreateIndexBuffer(Device, ModelBuffer);
  if (FAILED(result))
    {
      return false; 
    }

  /*
  delete[] ModelBuffer->vertices;
  ModelBuffer->vertices = nullptr;
    
  delete[] ModelBuffer->indices;
  ModelBuffer->indices = nullptr;
  */
  return true;
}

void ReleaseModel(Mesh* Buffer)
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
}

bool LoadModelFromFile(Mesh* Buffer) 
{
  char* filename = Buffer->filename; 
  
  FILE* file = fopen(filename, "r");
  if (!file)
    return false;

  XMFLOAT3 positions[1024];
  XMFLOAT4 colors[1024];
  unsigned long indicesTemp[2048];
  int posCount = 0, colCount = 0;
  UINT idxCount = 0;

  char type[2];
  while (fscanf(file, "%1s", type) == 1)
    {
      if (strcmp(type, "v") == 0)
        {
	  float x, y, z;
	  if (fscanf(file, "%f %f %f", &x, &y, &z) == 3)
	    positions[posCount++] = XMFLOAT3(x, y, z);
        }
      else if (strcmp(type, "c") == 0)
        {
	  float r, g, b, a;
	  if (fscanf(file, "%f %f %f %f", &r, &g, &b, &a) == 4)
	    colors[colCount++] = XMFLOAT4(r, g, b, a);
        }
      else if (strcmp(type, "i") == 0)
        {
	  unsigned long a, b, c;
	  if (fscanf(file, "%lu %lu %lu", &a, &b, &c) == 3)
            {
	      indicesTemp[idxCount++] = a;
	      indicesTemp[idxCount++] = b;
	      indicesTemp[idxCount++] = c;
            }
        }
    }

  fclose(file);

  int vertexCount = (posCount < colCount) ? posCount : colCount;
  Buffer->vertexCount = vertexCount;

  Buffer->vertices = new Vertex[vertexCount];
  if (!Buffer->vertices)
    return false;

  for (int i = 0; i < vertexCount; ++i)
    {
      (Buffer->vertices)[i].position = positions[i];
      (Buffer->vertices)[i].color = colors[i];
    }

  Buffer->indices = new unsigned long[idxCount];
  if (!Buffer->indices)
    return false;

  for (int i = 0; i < idxCount; ++i)
    (Buffer->indices)[i] = indicesTemp[i];
    
  Buffer->indexCount = idxCount;
 
  return true;
}
