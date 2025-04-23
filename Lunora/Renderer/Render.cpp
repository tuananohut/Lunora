#include "Render.h"

extern XMFLOAT4 ambientColor = XMFLOAT4(1.f, 0.15f, 0.15f, 1.f);
extern XMFLOAT4 diffuseColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
extern XMFLOAT3 lightDirection = XMFLOAT3(1.f, 0.f, 0.f); 

void RenderScene(DeviceManager& DeviceManager,
		 ShaderGPUData* Shader,
		 Scene* Scene,
		 XMMATRIX ViewMatrix,
		 XMMATRIX ProjectionMatrix)
{
  for (int i = 0; i < Scene->Meshes.Count; ++i)
    {
      MeshGPUData* Mesh = Scene->Meshes.Meshes[i];
      XMMATRIX WorldMatrix = TransformSystem::Compose(Mesh->Transform);

      RenderCube(DeviceManager, Shader, Mesh, WorldMatrix, ViewMatrix, ProjectionMatrix);
    }
}
