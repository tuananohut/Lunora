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
      
   
      Entity[i].worldMatrix = ComputeWorldMatrix(Entity[i].transform);
  
      
      /*
	  RenderModel(RenderBuffers, Entity[i].mesh);	      
	  result = Render(RenderBuffers, Entity[i].texture_shader, Entity[i].mesh->indexCount, matrix.world, matrix.view, matrix.proj);
	  if (FAILED(result))
	    {
	      return false; 
	      }*/	  
            
      RenderModel(RenderBuffers, &Entity[i]->mesh);	      
      result = Render(RenderBuffers, &Entity[i]->color_shader, Entity[i]->mesh.indexCount, Entity[i].worldMatrix, matrix.view, matrix.proj);
      if (FAILED(result))
	{
	  return false; 
	}	  	  
    } 

  return result;
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
 
