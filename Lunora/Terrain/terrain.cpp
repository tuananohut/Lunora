#include "terrain.h"

Terrain::Terrain()
{
  m_vertexBuffer = nullptr;
  m_indexBuffer = nullptr; 
}

Terrain::Terrain(const Terrain& other) {}

Terrain::~Terrain() {}

bool Terrain::Initialize(ID3D11Device* device)
{
  bool result;

  result = InitializeBuffers(device);
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

