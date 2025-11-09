#include "Entity.h"

bool InitializeEntity(Entity* Entity, RendererContext& RenderBuffers)
{
  size_t entity_num = sizeof(Entity) / sizeof(Entity[0]);
  bool running;
  HRESULT hr;
  
  for (size_t i = 0; i < entity_num; i++)
    {
      InitializeModel(RenderBuffers.Device, Entity[i].mesh);
      if (Entity[i].texture_shader)
	{
	  hr = InitializeShaderResources(RenderBuffers, Entity[i].texture_shader);
	  if (FAILED(hr))
	    return false; 

	}
      
      else if (Entity[i].color_shader)
	{
	  hr = InitializeShaderResources(RenderBuffers, Entity[i].color_shader);
	  if (FAILED(hr))
	    return false; 

	}
    } 
  
  return true; 
}

bool RenderEntity(Entity* Entity)
{
  return true;
}

void ReleaseEntity(Entity* Entity) {}
