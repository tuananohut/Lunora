#include "Scene.h"

bool InitializeScene(Scene &scene, RendererContext &Renderer, HWND hwnd)
{  
  scene.mCamera = new Camera;
  scene.mCamera->SetPosition(5.0f, 0.0f, -15.0f);

  scene.entity_num = 3; 
  
  for (size_t i = 0; i < scene.entity_num; ++i)
    {
      scene.entities[i] = new Entity();
    }
	  
  Running = InitializeEntity(scene.entities, scene.entity_num, Renderer);
  if (!Running)
    {
      Running = false;
      return 0; 
    }

  QueryPerformanceFrequency(&scene.frequency); 
  QueryPerformanceCounter(&scene.startTime);

  return true;
}

bool RenderScene(Scene &scene, RendererContext &Renderer)
{
  bool Running;
  MatrixBufferType matrix;
 	  
  RendererBeginScene(Renderer, 0.f, 0.f, 0.f, 1.f);

  LARGE_INTEGER currentTime;
  QueryPerformanceCounter(&currentTime);

  float total_time = (float)(currentTime.QuadPart - scene.startTime.QuadPart) / (float)scene.frequency.QuadPart;
	      
  scene.mCamera->Render();
	      
  matrix.world = XMMatrixIdentity();	      
  scene.mCamera->GetViewMatrix(matrix.view);
  
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
	      
  Running = RenderEntity(Renderer, scene.entities, scene.entity_num, matrix, total_time);
  if (!Running)
    {
      Running = false;
      return 0; 
    }
	      
  RendererEndScene(Renderer);
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
}
