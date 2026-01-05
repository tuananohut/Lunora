#include "Entity.h"

bool InitializeEntity(Entity* Entity[], size_t entity_num, RendererContext& RenderBuffers)
{
  bool running;
  HRESULT hr;
  
  const char* texture_file = "c:/dev/Lunora/Assets/Textures/palestine.tga";
  running = InitializeTexture(RenderBuffers.Device,
			      RenderBuffers.DeviceContext,
			      &Entity[0]->texture, texture_file);
  if (!running)
    {
      return false; 
    }
  assert(Entity[0]->texture.m_textureView != nullptr);
  
  texture_file = "c:/dev/Lunora/Assets/Textures/lemons.tga";  
  running = InitializeTexture(RenderBuffers.Device,
			      RenderBuffers.DeviceContext,
			      &Entity[1]->texture, texture_file);
  if (!running)
    {
      return false; 
    }
  assert(Entity[1]->texture.m_textureView != nullptr);
  
  texture_file = "c:/dev/Lunora/Assets/Textures/palestine_normalized.tga";
  
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i]->transform.position = {0.f + (i * 5.f), 0.f, -5.f};
      Entity[i]->transform.rotation = {0.f, 10.f, 0.f};
      Entity[i]->transform.scale = {1.f, 1.f, 1.f};
      Entity[i]->mesh.filename = "../Assets/Models/cube_trial.txt";

      if (Entity[i]->texture.m_textureView != nullptr) 
	Entity[i]->hemisphericMesh.filename = "c:/dev/Lunora/Assets/Models/cube_with_normal.txt";
      
      running = InitializeModel(RenderBuffers.Device, &Entity[i]->mesh);
      if(!running)
	{
          return false;
	}

      if (Entity[i]->texture.m_textureView != nullptr) 
	{
	  HemisphericMeshInitialize(RenderBuffers.Device, &Entity[i]->hemisphericMesh);
	  hr = InitializeShaderResources(RenderBuffers, &Entity[i]->light_shader);
	  if (FAILED(hr))
	    return false;
	}

      else
	{
	  InitializeModel(RenderBuffers.Device, &Entity[i]->mesh);
	  hr = InitializeShaderResources(RenderBuffers, &Entity[i]->color_shader);
	  if (FAILED(hr))
	    return false;
	}

    } 
  
  return true; 
}

bool RenderEntity(RendererContext& RenderBuffers, Entity* Entity[], size_t entity_num, MatrixBufferType& matrix, float total_time)
{
  HRESULT result = true;

  XMFLOAT3 AmbientDown = XMFLOAT3(4.f, 4.f, 4.f);
  XMFLOAT3 AmbientRange = XMFLOAT3(2.f, 2.f, 2.f);
  
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[0]->transform.rotation = {2.f * total_time, -1.f * total_time, 61.f};
      Entity[1]->transform.rotation = {2.f * total_time, 1.f * total_time, 61.f};
      Entity[2]->transform.rotation = {2.f * total_time, -1.f * total_time, 61.f};

      Entity[i]->worldMatrix = ComputeWorldMatrix(Entity[i]->transform);

      RenderModel(RenderBuffers, &Entity[i]->mesh);	      

      if (Entity[i]->texture.m_textureView)
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
      else
	{
	  result = Render(RenderBuffers, &Entity[i]->color_shader, Entity[i]->mesh.indexCount, Entity[i]->worldMatrix, matrix.view, matrix.proj);
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

