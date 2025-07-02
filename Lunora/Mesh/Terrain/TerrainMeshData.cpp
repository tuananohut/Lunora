#include "TerrainMeshData.h"

Terrain::Terrain()
{
  m_vertexBuffer = nullptr;
  m_indexBuffer = nullptr; 
}

bool Terrain::InitializeBuffers(ID3D11Device* Device)
{
  return true; 
}
