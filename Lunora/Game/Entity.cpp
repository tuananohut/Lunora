#include "Entity.h"

bool InitializeEntity(Entity* Entity[], size_t entity_num,
		      RendererContext& RenderBuffers)
{
  bool running;
  HRESULT hr;
  
  const char* texture_file = "c:/dev/Lunora/Assets/Textures/lemons.tga";  
  
  for (size_t i = 0; i < entity_num; i++)
    {      
      // Entity[i]->hemisphericMesh.filename = "c:/dev/Lunora/Assets/Models/monkey.txt";
      Entity[i]->hemisphericMesh.filename = "c:/dev/Lunora/Assets/Models/plane.txt";

      Entity[i]->transform.position = { 0.f, -2.f, 0.f };
      Entity[i]->transform.rotation = { 0.f, 0.f, 0.f };
      Entity[i]->transform.scale    = { 3.5f, 1.5f, 3.5f };

      texture_file = "c:/dev/Lunora/Assets/Textures/white.tga";
      running = InitializeTexture(RenderBuffers.Device,
				  RenderBuffers.DeviceContext,
				  &Entity[i]->texture, texture_file);
      if (!running)
	{
	  return false; 
	}
      assert(Entity[i]->texture.m_textureView != nullptr);

      running = HemisphericMeshInitialize(RenderBuffers.Device, &Entity[i]->hemisphericMesh);
      if (!running)
	{
	  return false; 
	}
      assert(Entity[i]->hemisphericMesh.vertices != nullptr);
      
      if (Entity[i]->texture.m_textureView)
	{ 
	  hr = InitializeShaderResources(RenderBuffers, &Entity[i]->water_shader);
	  if (FAILED(hr))
	    return false;
	}
    } 
  
  return true; 
}

bool RenderEntity(RendererContext& RenderBuffers, Entity* Entity[], size_t entity_num, MatrixBufferType& matrix, float total_time)
{
  HRESULT result = true;

  XMFLOAT3 AmbientDown  = XMFLOAT3(0.12f, 0.15f, 0.18f);
  XMFLOAT3 AmbientRange = XMFLOAT3(0.22f, 0.28f, 0.34f);
  
  XMFLOAT4 ambientColor = XMFLOAT4(0.11f, 0.14f, 0.18f, 1.f);
  XMFLOAT4 diffuseColor = XMFLOAT4(0.88f, 0.90f, 0.92f, 1.f);
  XMFLOAT3 lightDirection = XMFLOAT3(1.f, 1.f, 1.f);

  XMFLOAT3 cameraPosition = XMFLOAT3(0.f, 0.0f, -10.f); 
   
  for (size_t i = 0; i < entity_num; i++)
    {
      // Entity[i]->transform.rotation.y = 1.f * total_time;
      Entity[i]->worldMatrix = ComputeWorldMatrix(Entity[i]->transform);
    }
  
  for (size_t i = 0; i < entity_num; i++)
    {
      HemisphericMeshRender(RenderBuffers, &Entity[i]->hemisphericMesh);
      if (Entity[i]->texture.m_textureView)
	{      
	  result = Render(RenderBuffers, &Entity[i]->water_shader,
			  Entity[i]->hemisphericMesh.indexCount, Entity[i]->worldMatrix,
			  matrix.view, matrix.proj, Entity[i]->texture.m_textureView,
			  total_time, cameraPosition);
	  if (FAILED(result))
	    {
	      return false; 
	    }
	}
    }

  return true;
}

XMMATRIX ComputeWorldMatrix(const Transform& t)
{
    XMMATRIX S = XMMatrixScaling(t.scale.x, t.scale.y, t.scale.z);
    XMMATRIX RX = XMMatrixRotationX(t.rotation.x);
    XMMATRIX RY = XMMatrixRotationY(t.rotation.y);
    XMMATRIX RZ = XMMatrixRotationZ(t.rotation.z);
    XMMATRIX T = XMMatrixTranslation(t.position.x, t.position.y, t.position.z);

    return S * RX * RY * RZ * T;
}

