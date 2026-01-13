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
  
  delete[] Buffer->vertices;
  Buffer->vertices = nullptr;

  delete[] Buffer->indices;
  Buffer->indices = nullptr;

  Buffer->vertexCount = 0;
  Buffer->indexCount = 0;
}

bool LoadModelFromFile(Mesh* Buffer) 
{
  char* filename = Buffer->filename; 
  
  FILE* file = fopen(filename, "r");
  if (!file)
    return false;

  XMFLOAT3 positions[1024];
  XMFLOAT4 colors[1024];
  XMFLOAT2 texCoords[1024];
  uint32_t indicesTemp[2048];
  int posCount = 0, colCount = 0, texCount = 0;
  UINT idxCount = 0;

  char line[256];
  while (fgets(line, sizeof(line), file))
    {
      if (line[0] == 'v' && line[1] == ' ')
	{
	  float x, y, z;
	  sscanf(line, "v %f %f %f", &x, &y, &z);
	  positions[posCount++] = XMFLOAT3(x, y, z);
	}
      else if (line[0] == 'c' && line[1] == ' ')
	{
	  float r, g, b, a;
	  sscanf(line, "c %f %f %f %f", &r, &g, &b, &a);
	  colors[colCount++] = XMFLOAT4(r, g, b, a);
	}
      else if (line[0] == 'v' && line[1] == 't')
	{
	  float u, v;
	  sscanf(line, "vt %f %f", &u, &v);
	  texCoords[texCount++] = XMFLOAT2(u, v);
	}
       else if (line[0] == 'i' && line[1] == ' ')
	{
	  unsigned long a, b, c;
	  sscanf(line, "i %lu %lu %lu", &a, &b, &c);
	  indicesTemp[idxCount++] = a;
	  indicesTemp[idxCount++] = b;
	  indicesTemp[idxCount++] = c;
	}
    }
  fclose(file);
  
  // int vertexCount = (posCount < colCount) ? posCount : colCount;
  int vertexCount = posCount; 
  Buffer->vertexCount = vertexCount;

  Buffer->vertices = new Vertex[vertexCount];
  if (!Buffer->vertices)
    return false;

  for (int i = 0; i < vertexCount; ++i)
    {
      (Buffer->vertices)[i].position = positions[i];
      
      if (colCount > 0)
	(Buffer->vertices)[i].color = colors[i];
      else
	(Buffer->vertices)[i].color = XMFLOAT4(1,1,1,1);
      
      if (texCount > 0)
	(Buffer->vertices)[i].texture = texCoords[i % texCount];
      else
	(Buffer->vertices)[i].texture = XMFLOAT2(0, 0);
    }
  
  Buffer->indices = new uint32_t[idxCount];
  if (!Buffer->indices)
    return false;

  for (int i = 0; i < idxCount; ++i)
    (Buffer->indices)[i] = indicesTemp[i];
    
  Buffer->indexCount = idxCount;
  
  return true;
}
