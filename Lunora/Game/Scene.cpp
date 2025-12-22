#include "Scene.h"

bool InitializeScene(Scene *scene, HWND hwnd)
{
  scene->Renderer = new RendererContext; 
  Running = InitializeRenderer(*scene->Renderer, hwnd, SCREEN_WIDTH, SCREEN_HEIGHT);
  if (!Running)
    {
      return false; 
    }  
  
  scene->mCamera = new Camera;
  scene->mCamera->SetPosition(5.0f, 0.0f, -15.0f);

  scene->entity_num = 3; 
  
  for (size_t i = 0; i < scene->entity_num; ++i)
    {
      scene->entities[i] = new Entity();
    }
	  
  Running = InitializeEntity(scene->entities, scene->entity_num, *scene->Renderer);
  if (!Running)
    {
      Running = false;
      return 0; 
    }

  QueryPerformanceFrequency(&scene->frequency); 
  QueryPerformanceCounter(&scene->startTime);

  return true;
}

bool RenderScene(Scene *Scene)
{
  bool Running; 
  MatrixBufferType matrix; 
	  
  RendererBeginScene(*Scene->Renderer, 0.f, 0.f, 0.f, 1.f);

  LARGE_INTEGER currentTime;
  QueryPerformanceCounter(&currentTime);

  float total_time = (float)(currentTime.QuadPart - Scene->startTime.QuadPart) / (float)Scene->frequency.QuadPart;
	      
  Scene->mCamera->Render();
	      
  matrix.world = XMMatrixIdentity();	      
  Scene->mCamera->GetViewMatrix(matrix.view);
  
  float fieldOfView = 3.141592654f / 4.0f;
  float screenAspect = 1.f; 
  if (SCREEN_HEIGHT > 0)
    {
      screenAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    }
  else
    {
      screenAspect = 1.0f; 
    }
  
  matrix.proj  = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
	      
  Running = RenderEntity(*Scene->Renderer, Scene->entities, Scene->entity_num, matrix, total_time);
  if (!Running)
    {
      Running = false;
      return 0; 
    }
	      
  RendererEndScene(*Scene->Renderer);
  return true; 
}

void CleanScene(Scene *scene)
{
  if (scene->mCamera)
    {
      delete scene->mCamera;
      scene->mCamera = nullptr; 
    }

  for (int i = 0; i < scene->entity_num; i++)
    {
      if (scene->entities[i]->mesh.vertexBuffer)
	ReleaseModel(&scene->entities[i]->mesh);
			  
      if (scene->entities[i]->color_shader.baseShader.m_vertexShader)
	ReleaseShaderResources(&scene->entities[i]->color_shader);
      else if (scene->entities[i]->texture_shader.baseShader.m_vertexShader)
	ReleaseShaderResources(&scene->entities[i]->texture_shader);

      if (scene->entities[i]->texture.m_textureView)
	ReleaseTexture(&scene->entities[i]->texture);

			  
      delete scene->entities[i];
      scene->entities[i] = nullptr;
    }
		      		      
  if (scene->Renderer)
    {
      ShutdownRenderer(*scene->Renderer);
      delete scene->Renderer;
      scene->Renderer = nullptr; 
    }		     

}
