#include "Scene.h"

bool InitializeScene(ID3D11Device* device)
{
  Camera *mCamera = new Camera;
  mCamera->SetPosition(0.0f, 0.0f, -15.0f);
	  
  Entity* entities[1] = {0};
  Entity* triangle = new Entity;
  entities[0] = triangle;
  
  size_t entity_num = 1;  
	  
  Running = InitializeEntity(*entities, entity_num, *Renderer);
  if (!Running)
    {
      MessageBoxA(Window->hwnd, "Does not worked!", "Entity", MB_OK);
      Running = false;
      return 0; 
    }
  
  return true;
}
