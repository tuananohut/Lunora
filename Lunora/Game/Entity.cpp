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
  
  texture_file = "c:/dev/Lunora/Assets/Textures/flag.tga";  
  running = InitializeTexture(RenderBuffers.Device,
			      RenderBuffers.DeviceContext,
			      &Entity[1]->texture, texture_file);
  if (!running)
    {
      return false; 
    }

  assert(Entity[1]->texture.m_textureView != nullptr);
 
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i]->transform.position = {0.f + (i * 5.f), 0.f, -5.f};
      Entity[i]->transform.rotation = {0.f, 10.f, 0.f};
      Entity[i]->transform.scale = {1.f, 1.f, 1.f};
      Entity[i]->mesh.filename = "../Assets/Models/cube_trial.txt";

      if (Entity[i]->texture.m_textureView != nullptr) 
	Entity[i]->mesh.filename = "../Assets/Models/cube_trial_2.txt";
      
      running = InitializeModel(RenderBuffers.Device, &Entity[i]->mesh);
      if(!running)
	{
          return false;
	}

      if (Entity[i]->texture.m_textureView != nullptr) 
	{
	  hr = InitializeShaderResources(RenderBuffers, &Entity[i]->texture_shader);
	  if (FAILED(hr))
	    return false;
	}

      else
	{
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
  
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i]->transform.rotation = {2.f * total_time, 1.f * total_time, 61.f};
      Entity[i]->worldMatrix = ComputeWorldMatrix(Entity[i]->transform);

      RenderModel(RenderBuffers, &Entity[i]->mesh);	      

      if (Entity[i]->texture.m_textureView)
	{      
	  result = Render(RenderBuffers, &Entity[i]->texture_shader,
			  Entity[i]->mesh.indexCount, Entity[i]->worldMatrix,
			  matrix.view, matrix.proj, Entity[i]->texture.m_textureView);
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

