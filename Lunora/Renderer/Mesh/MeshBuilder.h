

#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "../RenderData.h"

struct Vertex
{
  XMFLOAT3 pos, normal;
  XMFLOAT2 uv;
};

void BuildMesh(DeviceManager& DeviceManager,
	       RenderManager* Renderer,
               MeshGPUData& Mesh,
	       ShaderGPUData& Shader,
               const VertexBufferType* vertices, size_t vertexCount,
               const unsigned long* indices, size_t indexCount,
	       const LPCWSTR VSFilename,
	       const LPCWSTR PSFilename);

#endif
