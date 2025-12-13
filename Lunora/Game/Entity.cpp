#include "Entity.h"

bool InitializeEntity(Entity* Entity[], size_t entity_num, RendererContext& RenderBuffers)
{
  bool running, texture_check;
  HRESULT hr;
  
  const char* texture_file = "c:/dev/Lunora/Assets/Textures/flag.tga";  
  hr = InitializeTexture(RenderBuffers.Device,
			 RenderBuffers.DeviceContext,
			 &Entity[0]->texture, texture_file);
  if (FAILED(hr))
    {
      texture_check = false; 
      return false; 
    }
  else
    texture_check = true;
 
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i]->transform.position = {0.f + (i * 5.f), 0.f, -5.f};
      Entity[i]->transform.rotation = {0.f, 0.f, 0.f};
      Entity[i]->transform.scale = {1.f, 1.f, 1.f};
      Entity[i]->mesh.filename = "../Assets/Models/triangle.txt";

      hr = InitializeTexture(RenderBuffers.Device,
			     RenderBuffers.DeviceContext,
			     &Entity[0]->texture, texture_file);

      
      bool result = InitializeModel(RenderBuffers.Device, &Entity[i]->mesh);
      if(!result)
	{
          return false;
	}

      if (&Entity[i]->texture != nullptr) 
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

  float rotationSpeed = 1.0f;
  
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i]->worldMatrix = ComputeWorldMatrix(Entity[i]->transform);

      RenderModel(RenderBuffers, &Entity[i]->mesh);	      

      if (Entity[i]->texture.m_textureView)
	{      
	  result = Render(RenderBuffers, &Entity[i]->texture_shader, Entity[i]->mesh.indexCount, Entity[i]->worldMatrix, matrix.view, matrix.proj, Entity[i]->texture.m_textureView);
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


void ReleaseEntity(Entity* entities[], size_t entity_num)
{
  for (size_t i = 0; i < entity_num; ++i)
    {
      ReleaseModel(&entities[i]->mesh);
      ReleaseShaderResources(&entities[i]->color_shader);
      // ReleaseShaderResources(Entity->texture_shader);
    } 
}
 
