#include "MeshBuilder.h"

#include <cassert>
#include <d3dcompiler.h>

#include "../../Managers.h"

struct MatrixBufferType
{
  XMMATRIX World;
  XMMATRIX View;
  XMMATRIX Projection;
};

struct LightBufferType
{
  XMFLOAT4 ambientColor;
  XMFLOAT4 diffuseColor;
  XMFLOAT3 lightDirection;
  float padding; 
};

void BuildMesh(DeviceManager& DeviceManager,
               RenderManager* Renderer,
               MeshGPUData& Mesh,  
               ShaderGPUData& Shader,
               const VertexBufferType* vertices, size_t vertexCount,
               const unsigned long* indices, size_t indexCount,
               const LPCWSTR VSFilename,
               const LPCWSTR PSFilename)
{
  if (Mesh.VertexBuffer != nullptr)
    OutputDebugStringA("Mesh.VertexBuffer is already initialized!\n");
  if (Mesh.IndexBuffer != nullptr)
    OutputDebugStringA("Mesh.IndexBuffer is already initialized!\n");
  if (vertices == nullptr)
    OutputDebugStringA("vertices is null!\n");
  if (indices == nullptr)
    OutputDebugStringA("indices is null!\n");

  // assert(Mesh.VertexBuffer == nullptr && Mesh.IndexBuffer == nullptr && vertices != nullptr && indices != nullptr);

    Mesh.indexCount = static_cast<unsigned int>(indexCount);
    Mesh.stride = sizeof(VertexBufferType);

    // Vertex buffer
    D3D11_BUFFER_DESC VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = UINT(sizeof(VertexBufferType) * vertexCount);
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
    VertexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vbData{};
    vbData.pSysMem = vertices;

    HRESULT Result = DeviceManager.Device->CreateBuffer(&VertexBufferDesc, &vbData, &Mesh.VertexBuffer);
    assert(SUCCEEDED(Result));

    // Index buffer
    D3D11_BUFFER_DESC ibDesc{};
    ibDesc.ByteWidth = UINT(sizeof(unsigned long) * indexCount);
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA ibData{};
    ibData.pSysMem = indices;

    Result = DeviceManager.Device->CreateBuffer(&ibDesc, &ibData, &Mesh.IndexBuffer);
    assert(SUCCEEDED(Result));

    // !!!!!!!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!!!!!!!!
    // Create Matrix buffer vs. shader constant buffer

    if (!LoadShader(DeviceManager.Device, VSFilename, PSFilename, &Shader, Renderer))
    {
        OutputDebugStringA("Could not load shader!\n");
    }

    // Matrix buffer
    D3D11_BUFFER_DESC BufferDesc{};
    BufferDesc.ByteWidth = sizeof(MatrixBufferType);
    BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    BufferDesc.MiscFlags = 0;
    BufferDesc.StructureByteStride = 0;

    Result = DeviceManager.Device->CreateBuffer(&BufferDesc, nullptr, &Mesh.MatrixBuffer);
    if (FAILED(Result))
    {
        OutputDebugStringA("Could not create matrix buffer.\n");
        Mesh.MatrixBuffer = nullptr;
    }

    // Light buffer
    D3D11_BUFFER_DESC LightBufferDesc{};
    LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    LightBufferDesc.ByteWidth = sizeof(LightBufferType);
    LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    LightBufferDesc.MiscFlags = 0;
    LightBufferDesc.StructureByteStride = 0;

    Result = DeviceManager.Device->CreateBuffer(&LightBufferDesc, nullptr, &Shader.LightBuffer);
    if (FAILED(Result))
    {
        OutputDebugStringA("Could not create light buffer.\n");
        Shader.LightBuffer = nullptr;
    }
    else
    {
        OutputDebugStringA("Light buffer created successfully.\n");
    }
}
