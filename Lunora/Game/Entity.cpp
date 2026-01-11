#include "Entity.h"

bool InitializeEntity(Entity* Entity[], size_t entity_num, RendererContext& RenderBuffers)
{
  bool running;
  HRESULT hr;
  
  const char* texture_file = "c:/dev/Lunora/Assets/Textures/lemons.tga";  
  
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i]->transform.position = {0.f + (i * 5.f), 0.f, -5.f};
      Entity[i]->transform.rotation = {0.f, 10.f, 0.f};
      Entity[i]->transform.scale = {1.5f, 1.5f, 1.5f};
      Entity[i]->mesh.filename = "../Assets/Models/cube_trial.txt";
      Entity[i]->hemisphericMesh.filename = "c:/dev/Lunora/Assets/Models/sphere.txt";

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
       
      if (i % 2 != 0 && Entity[i]->texture.m_textureView)
	{ 
	  hr = InitializeShaderResources(RenderBuffers, &Entity[i]->light_shader);
	  if (FAILED(hr))
	    return false;
	}
      
      if (i % 2 == 0 && Entity[i]->texture.m_textureView)
	{ 
	  hr = InitializeShaderResources(RenderBuffers, &Entity[i]->ambient_light_shader);
	  if (FAILED(hr))
	    return false;
	}
    } 
  
  return true; 
}

bool RenderEntity(RendererContext& RenderBuffers, Entity* Entity[], size_t entity_num, MatrixBufferType& matrix, float total_time)
{
  HRESULT result = true;

  XMFLOAT4 AmbientDown = XMFLOAT4(0.1f, 0.05f, 0.0f, 1.f);
  XMFLOAT4 AmbientRange = XMFLOAT4(0.2f, 0.4f, 1.f, 1.f);

  XMFLOAT4 ambientColor = XMFLOAT4 (0.15f, 0.15f, 0.15f, 1.f);
  XMFLOAT4 diffuseColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
  XMFLOAT3 lightDirection = XMFLOAT3(1.f, 1.f, 1.f);

  AmbientDown  = XMFLOAT4(0.05f, 0.05f, 0.06f, 1.f);
  AmbientRange = XMFLOAT4(0.35f, 0.35f, 0.32f, 1.f);
		 
  // AmbientDown  = XMFLOAT4(0.02f, 0.02f, 0.02f, 1.f);
  // AmbientRange = XMFLOAT4(0.20f, 0.20f, 0.18f, 1.f);
  
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i]->transform.rotation = {0.f, 1.f * total_time, 0.f};

      Entity[i]->worldMatrix = ComputeWorldMatrix(Entity[i]->transform);

      HemisphericMeshRender(RenderBuffers, &Entity[i]->hemisphericMesh);
      if (i % 2 != 0 && Entity[i]->texture.m_textureView)
	{      
	  result = Render(RenderBuffers, &Entity[i]->light_shader,
			  Entity[i]->hemisphericMesh.indexCount, Entity[i]->worldMatrix,
			  matrix.view, matrix.proj, Entity[i]->texture.m_textureView,
			  AmbientDown, AmbientRange);
	  if (FAILED(result))
	    {
	      return false; 
	    }
	}
      if (i % 2 == 0 && Entity[i]->texture.m_textureView)
	{      
	  result = Render(RenderBuffers, &Entity[i]->ambient_light_shader,
			  Entity[i]->hemisphericMesh.indexCount, Entity[i]->worldMatrix,
			  matrix.view, matrix.proj, Entity[i]->texture.m_textureView,
			  ambientColor, diffuseColor, lightDirection);
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

