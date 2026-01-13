#include "Mesh.h"

bool CreateMesh(ID3D11Device* device,
		Mesh& mesh,
		const void* vertexData,
		uint32_t vertexCount,
		uint32_t stride,
		const uint32_t* indexData,
		uint32_t indexCount)
{
  mesh.vertexCount = vertexCount;
  mesh.indexCount  = indexCount;
  mesh.stride      = stride;
  mesh.offset      = 0;

  D3D11_BUFFER_DESC vbDesc = {};
  vbDesc.Usage = D3D11_USAGE_DEFAULT;
  vbDesc.ByteWidth = stride * vertexCount;
  vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  D3D11_SUBRESOURCE_DATA vbData = {};
  vbData.pSysMem = vertexData;

  if (FAILED(device->CreateBuffer(&vbDesc, &vbData, &mesh.vertexBuffer)))
    return false;

  
  D3D11_BUFFER_DESC ibDesc = {};
  ibDesc.Usage = D3D11_USAGE_DEFAULT;
  ibDesc.ByteWidth = sizeof(uint32_t) * indexCount;
  ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

  D3D11_SUBRESOURCE_DATA ibData = {};
  ibData.pSysMem = indexData;

  if (FAILED(device->CreateBuffer(&ibDesc, &ibData, &mesh.indexBuffer)))
    return false;

  return true;
}

void RenderMesh(ID3D11DeviceContext* context, const Mesh& mesh)
{
  UINT stride = mesh.stride;
  UINT offset = mesh.offset;

  context->IASetVertexBuffers(0, 1, &mesh.vertexBuffer, &stride, &offset);
  context->IASetIndexBuffer(mesh.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
  context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  context->DrawIndexed(mesh.indexCount, 0, 0);
}

void ReleaseMesh(Mesh& mesh)
{
  if (mesh.vertexBuffer)
    {
      mesh.vertexBuffer->Release();
      mesh.vertexBuffer = nullptr;
    }

  if (mesh.indexBuffer)
    {
      mesh.indexBuffer->Release();
      mesh.indexBuffer = nullptr;
    }
}
