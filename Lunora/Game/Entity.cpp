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

bool RenderEntity(RendererContext& Renderer, Entity* Entity)
{
  XMMATRIX world = XMMatrixIdentity();
  XMMATRIX view;	      
  mCamera->GetViewMatrix(view);
  
  float fieldOfView = 3.141592654f / 4.0f;
  float screenAspect = 1.f; 
  if (Window->Height > 0)
    {
       screenAspect = (float)Window->Width / (float)Window->Height;
    }
  else
    {
      screenAspect = 1.0f; 
    }
  
  XMMATRIX proj  = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

  for (size_t i = 0; i < entity_num; i++)
    {      
      InitializeModel(RenderBuffers.Device, Entity[i].mesh);
      if (Entity[i].texture_shader)
	{
	  RenderModel(*Renderer, *Entity[i].mesh);	      
	  result = Render(*Renderer, *Entity[i].texture_shader, Entity[i].mesh->indexCount, world, view, proj);
	  if (FAILED(result))
	    {
	      return false; 
	    }	  
	}
      
      else if (Entity[i].color_shader)
	{
	  RenderModel(*Renderer, *Entity[i].mesh);	      
	  result = Render(*Renderer, *Entity[i].color_shader, Entity[i].mesh->indexCount, world, view, proj);
	  if (FAILED(result))
	    {
	      return false; 
	    }	  	  
	}
    } 

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
