#include "MeshData.h"

bool Mesh::InitializeBuffers(ID3D11Device*)
{
  return true;
}

void Mesh::ShutdownBuffers() {}

void Mesh::RenderBuffers(ID3D11DeviceContext*) {}
