#include "Entity.h"

bool InitializeEntity(Entity* Entity, size_t entity_num, RendererContext& RenderBuffers)
{
  bool running;
  HRESULT hr;
    
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i].mesh = new Mesh; 
      Entity[i].mesh->filename = "../Assets/Models/triangle.txt";

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

bool RenderEntity(RendererContext& RenderBuffers, Entity* Entity, size_t entity_num, const MatrixBufferType& matrix)
{
  bool result = true;
  
  for (size_t i = 0; i < entity_num; i++)
    {      
      InitializeModel(RenderBuffers.Device, Entity[i].mesh);
      if (Entity[i].texture_shader)
	{/*
	  RenderModel(RenderBuffers, Entity[i].mesh);	      
	  result = Render(RenderBuffers, *Entity[i].texture_shader, Entity[i].mesh->indexCount, matrix.world, matrix.view, matrix.proj);
	  if (FAILED(result))
	    {
	      return false; 
	      }*/	  
	}
      
      else if (Entity[i].color_shader)
	{
	  RenderModel(RenderBuffers, Entity[i].mesh);	      
	  result = Render(RenderBuffers, *Entity[i].color_shader, Entity[i].mesh->indexCount, matrix.world, matrix.view, matrix.proj);
	  if (FAILED(result))
	    {
	      return false; 
	    }	  	  
	}
    } 

  return result;
}

void ReleaseEntity(Entity* Entity, size_t entity_num)
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
      delete Entity->color_shader;
      Entity->color_shader = nullptr; 
    }
  
  if (Entity->texture_shader)
    {
      ReleaseShaderResources(*Entity->texture_shader);
      delete Entity->texture_shader;
      Entity->texture_shader = nullptr; 
    }
}
