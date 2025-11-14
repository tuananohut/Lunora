#include "Entity.h"

bool InitializeEntity(Entity* Entity, size_t entity_num, RendererContext& RenderBuffers)
{
  bool running;
  HRESULT hr;
    
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i].mesh = new Mesh; 
      Entity[i].mesh->filename = "../Assets/Models/triangle.txt";

      Entity[i].mesh = new Mesh; 
      Entity[i].mesh->filename = "../Assets/Models/cube.txt";
      
      Entity[i].color_shader = new ColorShader; 
      Entity[i].texture_shader = nullptr; 
      
      InitializeModel(RenderBuffers.Device, Entity[i].mesh);
      if (Entity[i].texture_shader)
	{
	  hr = InitializeShaderResources(RenderBuffers, *Entity[i].texture_shader);
	  if (FAILED(hr))
	    return false; 
	  
	}
      
      else if (Entity[i].color_shader)
	{
	  hr = InitializeShaderResources(RenderBuffers, *Entity[i].color_shader);
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

void ReleaseEntity(Entity* Entity, size_t entity_num)
{
  for (size_t i = 0; i < entity_num; entity_num++)
    {
      if (Entity->mesh)
	{
	  ReleaseModel(Entity->mesh);
	  delete Entity->mesh;
	  Entity->mesh = nullptr; 
	}

      if (Entity->color_shader)
	{
	  ReleaseShaderResources(*Entity->color_shader);
	}
  
      if (Entity->texture_shader)
	{
	  ReleaseShaderResources(*Entity->texture_shader);
	}
    }
}
