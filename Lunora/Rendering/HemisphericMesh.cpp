#include "HemisphericMesh.h"

bool HemisphericMeshCreateVertexBuffer(ID3D11Device* Device, HemisphericMesh* Mesh)
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

/*
bool HemisphericMeshLoadFromFile(HemisphericMesh* Buffer)
{
  char* filename = Buffer->filename;
  assert(filename != nullptr);
  
  FILE* file = fopen(filename, "r");
  if (!file)
    return false;

  XMFLOAT3 positions[1024];
  XMFLOAT2 texCoords[1024];
  XMFLOAT3 normal[1024];
  uint32_t indicesTemp[2048];
  int posCount = 0, normalCount = 0, texCount = 0;
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
      else if (line[0] == 'v' && line[1] == 'n')
	{
	  float nx, ny, nz;	  
	  sscanf(line, "vn %f %f %f", &nx, &ny, &nz);
	  normal[normalCount++] = XMFLOAT3(nx, ny, nz);
	}
      else if (line[0] == 'v' && line[1] == 't')
	{
	  float u, v;
	  sscanf(line, "vt %f %f", &u, &v);
	  texCoords[texCount++] = XMFLOAT2(u, v);
	}
      else if (line[0] == 'i' && line[1] == ' ')
	{
	  uint32_t a, b, c;
	  sscanf(line, "i %u %u %u", &a, &b, &c);
	  indicesTemp[idxCount++] = a;
	  indicesTemp[idxCount++] = b;
	  indicesTemp[idxCount++] = c;
	}
    }
  fclose(file);
  
  // int vertexCount = (posCount < colCount) ? posCount : colCount;
  int vertexCount = posCount; 
  Buffer->vertexCount = vertexCount;

  Buffer->vertices = new HemisphericVertex[vertexCount];
  if (!Buffer->vertices)
    return false;

  for (int i = 0; i < vertexCount; ++i)
    {
      Buffer->vertices[i].position = positions[i];

      if (i < normalCount)
        Buffer->vertices[i].normal = normal[i];
      else
        Buffer->vertices[i].normal = XMFLOAT3(0,1,0);

      if (texCount > 0)
        Buffer->vertices[i].texture = texCoords[i % texCount];
      else
        Buffer->vertices[i].texture = XMFLOAT2(0,0);
    }

  Buffer->indices = new uint32_t[idxCount];
  if (!Buffer->indices)
    return false;
  
  for (int i = 0; i < idxCount; ++i)
    (Buffer->indices)[i] = indicesTemp[i];
    
  Buffer->indexCount = idxCount;
  
  return true;
}
*/

bool HemisphericMeshLoadFromFile(HemisphericMesh* Buffer)
{
    assert(Buffer && Buffer->filename);

    FILE* file = fopen(Buffer->filename, "r");
    if (!file)
        return false;

    XMFLOAT3 positions[256];
    XMFLOAT3 normals[256];
    XMFLOAT2 texcoords[16];

    int posCount = 0;
    int normalCount = 0;
    int texCount = 0;

    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            sscanf(line, "v %f %f %f %f %f %f",
                   &positions[posCount].x,
                   &positions[posCount].y,
                   &positions[posCount].z,
		   &normals[normalCount].x,
		   &normals[normalCount].y,
		   &normals[normalCount].z);
            posCount++;
	    normalCount++; 
        }
        else if (line[0] == 'v' && line[1] == 't')
        {
            sscanf(line, "vt %f %f",
                   &texcoords[texCount].x,
                   &texcoords[texCount].y);
            texCount++;
        }
    }

    fclose(file);

    if (posCount == 0 || posCount != normalCount)
        return false;

    Buffer->vertexCount = posCount;
    Buffer->indexCount  = posCount;

    Buffer->vertices = new HemisphericVertex[posCount];
    Buffer->indices  = new uint32_t[posCount];

    for (int i = 0; i < posCount; ++i)
    {
        Buffer->vertices[i].position = positions[i];
        Buffer->vertices[i].normal   = normals[i];
        Buffer->vertices[i].texture  = texcoords[i % texCount];

        Buffer->indices[i] = i;
    }

    return true;
}
