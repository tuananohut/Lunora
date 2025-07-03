#include "TerrainMeshData.h"

Terrain::Terrain()
{
  m_vertexBuffer = nullptr;
  m_indexBuffer = nullptr; 
}

Terrain::~Terrain() {}

bool Terrain::Initialize(ID3D11Device* Device)
{
  bool result;

  result = InitializeBuffers(Device);
  if (!result)
    {
      return false;
    }
  
  return true;
}

void Terrain::Shutdown()
{
  ShutdownBuffers(); 
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
  
  return true; 
}


