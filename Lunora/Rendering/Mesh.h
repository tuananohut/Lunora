#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <cstdint>

struct Mesh
{
    ID3D11Buffer* vertexBuffer = nullptr;
    ID3D11Buffer* indexBuffer  = nullptr;

    uint32_t vertexCount = 0;
    uint32_t indexCount  = 0;

    uint32_t stride = 0;
    uint32_t offset = 0;
};

bool CreateMesh(
    ID3D11Device* device,
    Mesh& mesh,
    const void* vertexData,
    uint32_t vertexCount,
    uint32_t stride,
    const uint32_t* indexData,
    uint32_t indexCount);

void RenderMesh(ID3D11DeviceContext* context, const Mesh& mesh);
void ReleaseMesh(Mesh& mesh);

#endif
