#include "SkyDome.h"

bool SkyDomeMeshCreateVertexBuffer(ID3D11Device* Device, SkyDomeMesh* Mesh)
{
  HRESULT result;
  
  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(SkyDomeVertex) * Mesh->vertexCount;
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;
  
  D3D11_SUBRESOURCE_DATA InitData = {};
  InitData.pSysMem = Mesh->vertices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;
  
  result = Device->CreateBuffer( &bufferDesc, &InitData, &Mesh->vertexBuffer );
  if (FAILED(result))
    return false; 
  
  return true; 
}

bool SkyDomeMeshCreateIndexBuffer(ID3D11Device* Device, SkyDomeMesh* Mesh)
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

bool SkyDomeMeshInitialize(ID3D11Device* Device, SkyDomeMesh* ModelBuffer)
{
  HRESULT result;
 
  bool loaded = SkyDomeMeshLoadFromFile(ModelBuffer);
  if (!loaded)
    {
      return false; 
    }
  
  result = SkyDomeMeshCreateVertexBuffer(Device, ModelBuffer); 
  if (FAILED(result))
    {
      return false; 
    }
	  
  result = SkyDomeMeshCreateIndexBuffer(Device, ModelBuffer);
  if (FAILED(result))
    {
      return false; 
    }

  return true;
}

void SkyDomeMeshRender(RendererContext& RenderBuffers, SkyDomeMesh* mesh)
{
  uint32_t stride = sizeof(SkyDomeVertex);
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

void SkyDomeMeshRelease(SkyDomeMesh* Buffer)
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

bool SkyDomeMeshLoadFromFile(SkyDomeMesh* Buffer)
{
  assert(Buffer && Buffer->filename);

  FILE* file = fopen(Buffer->filename, "r");
  if (!file)
    return false;

  XMFLOAT3 positions[14700];
  
  int posCount = 0;

  char line[15000];

  while (fgets(line, sizeof(line), file))
    {
      if (line[0] == 'v' && line[1] == ' ')
	{
	  float x, y, z;
	  int read = sscanf(line, "v %f %f %f", &x, &y, &z);
	  
	  if (read == 8)
	    {
	      positions[posCount++] = XMFLOAT3(x, y, z);	    
	    }
	}
    }

  fclose(file);

  if (posCount == 0)
    return false;

  Buffer->vertexCount = posCount;
  Buffer->indexCount  = Buffer->vertexCount;

  Buffer->vertices = new SkyDomeVertex[Buffer->vertexCount];
  Buffer->indices  = new uint32_t[Buffer->indexCount];

  for (int i = 0; i < posCount; ++i)
    {
      Buffer->vertices[i].position = positions[i];

      Buffer->indices[i] = i;
    }

  return true;
}
 
