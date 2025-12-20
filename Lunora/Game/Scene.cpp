#include "Scene.h"

bool InitializeScene(ID3D11Device* device)
{
  Camera *mCamera = new Camera;
  mCamera->SetPosition(0.0f, 0.0f, -15.0f);
	  
  Entity* entities[1] = {0};
  Entity* triangle = new Entity;
  entities[0] = triangle;

  Entity* cube = new Entity;
  entities[1] = cube;

  size_t entity_num = 2;  
	  
  Running = InitializeEntity(*entities, entity_num, *Renderer);
  if (!Running)
    {
      MessageBoxA(Window->hwnd, "Does not worked!", "Entity", MB_OK);
      Running = false;
      return 0; 
    }
  
  return true;
}

void CleanScene()
{
  if (mCamera)
    {
      delete mCamera;
      mCamera = nullptr; 
    }

  for (int i = 0; i < entity_num; i++)
    {
      if (entities[i]->mesh.vertexBuffer)
	ReleaseModel(&entities[i]->mesh);
			  
      if (entities[i]->color_shader.baseShader.m_vertexShader)
	ReleaseShaderResources(&entities[i]->color_shader);
      else if (entities[i]->texture_shader.baseShader.m_vertexShader)
	ReleaseShaderResources(&entities[i]->texture_shader);

      if (entities[i]->texture.m_textureView)
	ReleaseTexture(&entities[i]->texture);

			  
      delete entities[i];
      entities[i] = nullptr;
    }
		      		      
  if (Renderer)
    {
      ShutdownRenderer(*Renderer);
      delete Renderer;
      Renderer = nullptr; 
    }		     

}
