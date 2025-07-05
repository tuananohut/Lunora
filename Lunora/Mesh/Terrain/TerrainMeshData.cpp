#include "TerrainMeshData.h"

Terrain::Terrain()
{
  m_vertexBuffer = nullptr;
  m_indexBuffer = nullptr; 
}

Terrain::~Terrain() {}

int Terrain::GetIndexCount()
{
  return m_indexCount; 
}

bool Terrain::InitializeBuffers(ID3D11Device* Device)
{
  VertexType* vertices;
  unsigned long* indices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;
  
  int i, j, terrainWidth, terrainHeight, index;
  XMFLOAT4 color;
  float positionX, positionZ;

  terrainHeight = 256;
  terrainWidth = 256;

  color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

  m_vertexCount = (terrainWidth - 1) * (terrainHeight - 1) * 8;

  m_indexCount = m_vertexCount;

  vertices = new VertexType[m_vertexCount];
  if (!vertices)
    {
      return false; 
    }

  indices = new unsigned long[m_indexCount];
  if (!indices)
    {
      return false;
    }

  index = 0;

  for(j=0; j<(terrainHeight-1); j++)
    {
      for(i=0; i<(terrainWidth-1); i++)
	{
	  // Line 1 - Upper left.
	  positionX = (float)i;
	  positionZ = (float)(j + 1);

	  vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 1 - Upper right.
	  positionX = (float)(i + 1);
	  positionZ = (float)(j + 1);

	  vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 2 - Upper right
	  positionX = (float)(i + 1);
	  positionZ = (float)(j + 1);

	  vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 2 - Bottom right.
	  positionX = (float)(i + 1);
	  positionZ = (float)j;

	  vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 3 - Bottom right.
	  positionX = (float)(i + 1);
	  positionZ = (float)j;

	  vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 3 - Bottom left.
	  positionX = (float)i;
	  positionZ = (float)j;

	  vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 4 - Bottom left.
	  positionX = (float)i;
	  positionZ = (float)j;

	  vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 4 - Upper left.
	  positionX = (float)i;
	  positionZ = (float)(j + 1);

	  vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;
	}
    }

  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;
  
  vertexData.pSysMem = vertices; 
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  result = Device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
  if (FAILED(result))
    {
      return false; 
    }

  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;
  
  indexData.pSysMem = indices; 
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  result = Device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
  if (FAILED(result))
    {
      return false; 
    }

  delete[] vertices;
  vertices = nullptr;

  delete[] indices;
  indices = nullptr; 
  
  return true; 
}

void Terrain::ShutdownBuffers()
{
  if (m_indexBuffer)
    {
      m_indexBuffer->Release();
      m_indexBuffer = nullptr;
    }

  if (m_vertexBuffer)
    {
      m_vertexBuffer->Release();
      m_vertexBuffer = nullptr; 
    }
}

void Terrain::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
  unsigned int stride;
  unsigned int offset;

  stride = sizeof(VertexType);
  offset = 0;

  DeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

  DeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
