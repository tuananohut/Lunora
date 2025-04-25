#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "../Renderer/RenderData.h"

struct Vertex
{
  XMFLOAT3 pos, normal;
  XMFLOAT2 uv;
};

void BuildMesh(DeviceManager& DeviceManager,
               MeshGPUData* mesh,
               const Vertex* vertices, size_t vertexCount,
               const unsigned long* indices, size_t indexCount);
#endif
