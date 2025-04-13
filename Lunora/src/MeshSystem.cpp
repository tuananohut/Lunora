#include "MeshSystem.h"

void UpdateMeshSystem(ID3D11DeviceContext* DeviceContext,
		      const MeshData* MeshData)
{
  for (UINT i = 0; i < MeshData->Count; ++i)
    {
      const MeshGPUData& gpu = MeshData->MeshArray[i];

      UINT offset = 0;

      DeviceContext->IASetVertexBuffers(0, 1, &gpu.VertexBuffer, &gpu.stride, &offset);
      DeviceContext->IASetIndexBuffers(gpu.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
      DeviceContext->VSSetConstantBuffers(0, 1, &gpu.MatrixBuffer);

      DeviceContext->DrawIndexed(gpu.indexCount, 0, 0);
    }
}
