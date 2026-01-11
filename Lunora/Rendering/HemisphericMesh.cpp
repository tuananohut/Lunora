#include "HemisphericMesh.h"

bool HemisphericMeshCreateVertexBuffer(ID3D11Device* Device, HemisphericMesh* Mesh)
{
  HRESULT result;
  
  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(HemisphericVertex) * Mesh->vertexCount;
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

bool HemisphericMeshCreateIndexBuffer(ID3D11Device* Device, HemisphericMesh* Mesh)
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

void HemisphericMeshRender(RendererContext& RenderBuffers, HemisphericMesh* mesh)
{
  uint32_t stride = sizeof(HemisphericVertex);
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

#include <vector>

bool HemisphericMeshLoadFromFile(HemisphericMesh* Buffer)
{
  assert(Buffer && Buffer->filename);

  FILE* file = fopen(Buffer->filename, "r");
  if (!file)
    return false;

  XMFLOAT3 positions[14700];
  XMFLOAT3 normals[14700];
  XMFLOAT2 texcoords[14700];
  
  int posCount = 0;
  int normalCount = 0;
  int texCount = 0;

  char line[15000];

  while (fgets(line, sizeof(line), file))
    {
      if (line[0] == 'v' && line[1] == ' ')
	{
	  float x, y, z;
	  float tx, ty; 
	  float nx, ny, nz;
	  int read = sscanf(line, "v %f %f %f %f %f %f %f %f",
			    &x, &y, &z, &tx, &ty, &nx, &ny, &nz);
	  
	  if (read == 8)
	    {
	      positions[posCount++] = XMFLOAT3(x, y, z);	    
	      normals[normalCount++] = XMFLOAT3(nx, ny, nz); 
	      texcoords[texCount++] = XMFLOAT2(tx, ty); 	    
	    }
	}
    }

  fclose(file);

  if (posCount == 0 || posCount != normalCount)
    return false;

  Buffer->vertexCount = posCount;
  Buffer->indexCount  = Buffer->vertexCount;

  Buffer->vertices = new HemisphericVertex[Buffer->vertexCount];
  Buffer->indices  = new uint32_t[Buffer->indexCount];

  for (int i = 0; i < posCount; ++i)
    {
      Buffer->vertices[i].position = positions[i];
      Buffer->vertices[i].normal   = normals[i];
      Buffer->vertices[i].texture  = texcoords[i];

      Buffer->indices[i] = i;
    }

  return true;
}
 
