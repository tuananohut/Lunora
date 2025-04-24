#include "MeshBuilder.h"
#include <cassert>

void BuildMesh(DeviceManager& devMgr,
               MeshGPUData* mesh,
               const Vertex* verts, size_t vertCount,
               const unsigned long* inds, size_t indCount)
{
    assert(mesh && verts && inds);

    mesh->indexCount = (unsigned int)indCount;
    mesh->stride     = sizeof(Vertex);

    // Vertex buffer
    D3D11_BUFFER_DESC vbDesc{};
    vbDesc.ByteWidth      = UINT(sizeof(Vertex) * vertCount);
    vbDesc.Usage          = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA vbData{};
    vbData.pSysMem = verts;

    HRESULT hr = devMgr.Device->CreateBuffer(&vbDesc, &vbData, &mesh->VertexBuffer);
    assert(SUCCEEDED(hr));

    // Index buffer
    D3D11_BUFFER_DESC ibDesc{};
    ibDesc.ByteWidth      = UINT(sizeof(unsigned long) * indCount);
    ibDesc.Usage          = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA ibData{};
    ibData.pSysMem = inds;

    hr = devMgr.Device->CreateBuffer(&ibDesc, &ibData, &mesh->IndexBuffer);
    assert(SUCCEEDED(hr));

    // !!!!!!!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!!!!!!!!
    // Create Matrix buffer vs. shader constant buffer
}
