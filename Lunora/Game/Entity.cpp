#include "Entity.h"

bool InitializeEntity(Entity* Entity[], size_t entity_num,
		      RendererContext& RenderBuffers)
{
  bool running;
  HRESULT hr;
  
  const char* texture_file = "c:/dev/Lunora/Assets/Textures/lemons.tga";  
  
  for (size_t i = 0; i < entity_num; i++)
    {      
      Entity[i]->mesh.filename = "../Assets/Models/cube_trial.txt";
      // Entity[i]->hemisphericMesh.filename = "c:/dev/Lunora/Assets/Models/monkey.txt";
      Entity[i]->hemisphericMesh.filename = "c:/dev/Lunora/Assets/Models/cube_with_normal.txt";
   
      Entity[0]->transform.position = { 3.f, 0.f, 0.f };
      Entity[0]->transform.rotation = { 0.f, 45.f, 0.f };
      Entity[0]->transform.scale    = { 1.5f, 1.5f, 1.5f };

      Entity[1]->transform.position = { -3.f, 0.f, 0.f };
      Entity[1]->transform.rotation = { 0.f, 20.f, 0.f };
      Entity[1]->transform.scale    = { 1.5f, 1.5f, 1.5f };

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
	  hr = InitializeShaderResources(RenderBuffers, &Entity[i]->light_shader);
	  if (FAILED(hr))
	    return false;
	}
      
      if (Entity[i]->texture.m_textureView)
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

  XMFLOAT3 AmbientDown  = XMFLOAT3(0.14f, 0.16f, 0.19f);
  XMFLOAT3 AmbientRange = XMFLOAT3(0.26f, 0.30f, 0.35f);
  
  XMFLOAT4 ambientColor = XMFLOAT4(0.13f, 0.15f, 0.17f, 1.f);
  XMFLOAT4 diffuseColor = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.f);
  XMFLOAT3 lightDirection = XMFLOAT3(1.f, 1.f, 1.f);
   
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
	  result = Render(RenderBuffers, &Entity[0]->light_shader,
			  Entity[0]->hemisphericMesh.indexCount, Entity[0]->worldMatrix,
			  matrix.view, matrix.proj, Entity[0]->texture.m_textureView,
			  AmbientDown, AmbientRange, total_time);
	  if (FAILED(result))
	    {
	      return false; 
	    }
	}
      
      if (Entity[1]->texture.m_textureView)
	{      
	  result = Render(RenderBuffers, &Entity[1]->ambient_light_shader,
			  Entity[1]->hemisphericMesh.indexCount, Entity[1]->worldMatrix,
			  matrix.view, matrix.proj, Entity[1]->texture.m_textureView,
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

