#include "MeshBuilder.h"

#include <cassert>
#include <d3dcompiler.h>

void BuildMesh(DeviceManager& DeviceManager,
               MeshGPUData* Mesh,
	       ShaderGPUData* Shader,
               const Vertex* vertices, size_t vertexCount,
               const unsigned long* indices, size_t indexCount,
	       const LPCWSTR VSFilename,
	       const LPCWSTR PSFilename)
{
    assert(Mesh && vertices && indices);

    mesh->indexCount = (unsigned int)indexCount;
    mesh->stride = sizeof(Vertex);

    // Vertex buffer
    D3D11_BUFFER_DESC VertexBufferDesc;
    ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
    VertexBufferDesc.ByteWidth = UINT(sizeof(Vertex) * vertexCount);
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
    VertexBufferDesc.StructureByteStride = 0; 

    D3D11_SUBRESOURCE_DATA vbData{};
    vbData.pSysMem = vertices;

    HRESULT Result = DeviceManager.Device->CreateBuffer(&vbDesc, &vbData, &Mesh->VertexBuffer);
    assert(SUCCEEDED(Result));

    // Index buffer
    D3D11_BUFFER_DESC ibDesc{};
    ibDesc.ByteWidth = UINT(sizeof(unsigned long) * indCount);
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA ibData{};
    ibData.pSysMem = inds;
    
    Result = devMgr.Device->CreateBuffer(&ibDesc, &ibData, &Mesh->IndexBuffer);
    assert(SUCCEEDED(Result));

    // !!!!!!!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!!!!!!!!
    // Create Matrix buffer vs. shader constant buffer

    if (!LoadShader(DeviceManager.Device,
		    VSFilename, PSFilename,
		    Shader, Renderer));
    {
      OutputDebugStringA("Could not load shader!");
    }
}
