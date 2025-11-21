#include "Entity.h"

bool InitializeEntity(Entity* Entity[], size_t entity_num, RendererContext& RenderBuffers)
{
  bool running;
  HRESULT hr;
    
  for (size_t i = 0; i < entity_num; i++)
    {
      Entity[i]->mesh.filename = "../Assets/Models/triangle.txt";
            
      bool result = InitializeModel(RenderBuffers.Device, &Entity[i]->mesh);
      if(!result)
	{
          MessageBoxA(nullptr, "Model could not load!", "Error", MB_OK);
          return false;
	}
      
      hr = InitializeShaderResources(RenderBuffers, &Entity[i]->texture_shader);
      if (FAILED(hr))
	return false; 	  
      
      hr = InitializeShaderResources(RenderBuffers, &Entity[i]->color_shader);
      if (FAILED(hr))
	return false; 
    } 
  
  return true; 
}

bool RenderEntity(RendererContext& RenderBuffers, Entity* Entity[], size_t entity_num, MatrixBufferType& matrix, float total_time)
{
  bool result = true;

  float rotationSpeed = 1.0f;
  
  for (size_t i = 0; i < entity_num; i++)
    {
      XMMATRIX rotationMatrix = XMMatrixRotationX(total_time * rotationSpeed) * XMMatrixRotationY(total_time * rotationSpeed);
      
      XMMATRIX translationMatrix = XMMatrixTranslation((float)i * 5.f - 2.f, 0.0f, 0.0f);
      
      matrix.world = rotationMatrix * translationMatrix;
      
      /*
	  RenderModel(RenderBuffers, Entity[i].mesh);	      
	  result = Render(RenderBuffers, Entity[i].texture_shader, Entity[i].mesh->indexCount, matrix.world, matrix.view, matrix.proj);
	  if (FAILED(result))
	    {
	      return false; 
	      }*/	  
            
      RenderModel(RenderBuffers, &Entity[i]->mesh);	      
      result = Render(RenderBuffers, &Entity[i]->color_shader, Entity[i]->mesh.indexCount, matrix.world, matrix.view, matrix.proj);
      if (FAILED(result))
	{
	  return false; 
	}	  	  
    } 

  return result;
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
 
