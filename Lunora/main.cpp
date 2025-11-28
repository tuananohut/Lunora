#include <windows.h>

#include "resource.h"
#include "../Lunora/Engine/Renderer.h"
#include "../Lunora/Game/Camera/Camera.h"
#include "../Lunora/Game/Entity.h"
#include "../Lunora/Rendering/Mesh.h"
#include "../Lunora/Rendering/Shader/Shader.h"
#include "../Lunora/Rendering/Texture.h"

using namespace LunoraEngine; 

const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.3f;

static bool Running;

static RendererContext *Renderer = new RendererContext();

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = 0;
  ID3D11Texture2D *BackBuffer;
  
  switch (Message)
    {  
    case WM_CLOSE: 
      {
        Running = false;
	DestroyWindow(Window);
      } break;

    case WM_DESTROY:
      {			
	Running = false;
	PostQuitMessage(0);
      } break;

    case WM_SIZE:
      {
	if (WParam == SIZE_MINIMIZED) break;
	
	RECT rect;
	GetClientRect(Window, &rect);

	int clientWidth  = rect.right - rect.left;
	int clientHeight = rect.bottom - rect.top;

	SCREEN_WIDTH = clientWidth;  
	SCREEN_HEIGHT = clientHeight;

	if (Renderer && Renderer->Device && Renderer->DeviceContext && Renderer->SwapChain)
	  {
	    ResizeRenderer(*Renderer, SCREEN_WIDTH, SCREEN_HEIGHT); 
	  }
	
      } break;
      
    default:
      {
	Result = DefWindowProc(Window, Message, WParam, LParam);
      } break;
    }
    
  return Result;
}

int WINAPI WinMain(HINSTANCE Instance,
		   HINSTANCE PrevInstance,
		   LPSTR CommandLine,
		   int ShowCode)
{  
  WNDCLASSEXA wc = {};
  
  wc.cbSize = sizeof(WNDCLASSEXA);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = Instance;
  wc.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_ICON1));;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = "Lunora";

  HRESULT result; 
  
  if (RegisterClassExA(&wc))
    {
      int x = SCREEN_WIDTH;
      int y = SCREEN_HEIGHT; 
 
      HWND hwnd = CreateWindowExA(0,                   
				  wc.lpszClassName,          
				  "Lunora",  
				  WS_OVERLAPPEDWINDOW | WS_VISIBLE,        
				  CW_USEDEFAULT, CW_USEDEFAULT, x, y, 
				  NULL,       
				  NULL,       
				  Instance,  
				  NULL);
      
     
      if (hwnd)
	{	  
	  Running = true;
	  
	  Running = InitializeRenderer(*Renderer, hwnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	  if (!Running)
	    {
	      MessageBoxA(hwnd, "Worked!", "Good", MB_OK);
	      Running = false;
	      return false; 
	    }  

	  Camera *mCamera = new Camera;
	  mCamera->SetPosition(0.0f, 0.0f, -15.0f);

	  const size_t entity_num = 2;  
	  
	  Entity* entities[entity_num];

	  entities[0] = new Entity();
	  entities[1] = new Entity();
	  
	  Running = InitializeEntity(entities, entity_num, *Renderer);
	  if (!Running)
	    {
	      MessageBoxA(hwnd, "Does not worked!", "Entity", MB_OK);
	      Running = false;
	      return 0; 
	    }

	  MatrixBufferType matrix; 
	  
	  /*
	  Texture* texture = new Texture;
	  const char* texture_file = "../Assets/Textures/palestine.tga";  
	  result = InitializeTexture(Renderer->Device,
				     Renderer->DeviceContext,
				     texture, texture_file);
	  if (FAILED(result))
	    {
	      MessageBoxA(hwnd, "Something is wrong about textures!", "Bad", MB_OK | MB_ICONERROR);
	      Running = false;
	      return 0; 
	    }
	  */

	  LARGE_INTEGER frequency;
	  LARGE_INTEGER startTime;
	  QueryPerformanceFrequency(&frequency); 
	  QueryPerformanceCounter(&startTime);
	  
	  while(Running)
	    {
	      MSG Message;
	      while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
		{		  
		  if (Message.message == WM_QUIT)
		    { 
		      if (mCamera)
			{
			  delete mCamera;
			  mCamera = nullptr; 
			}
		      
		      if (Renderer)
			{
			  ShutdownRenderer(*Renderer);
			  delete Renderer;
			  Renderer = nullptr; 
			}		     
		      
		      /*if (entities)
			{
			  ReleaseEntity(entities, entity_num);
			  }*/
		      
		      for (int i = 0; i < entity_num; i++)
			{
			  delete entities[i];
			  entities[i] = nullptr;
			}

		      Running = false;
		    }
		  
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}

	      RendererBeginScene(*Renderer, 0.f, 0.f, 0.f, 1.f);

	      LARGE_INTEGER currentTime;
	      QueryPerformanceCounter(&currentTime);

	      float total_time = (float)(currentTime.QuadPart - startTime.QuadPart) / (float)frequency.QuadPart;
	      
	      mCamera->Render();
	      
	      matrix.world = XMMatrixIdentity();	      
	      mCamera->GetViewMatrix(matrix.view);
  
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
	      
	      Running = RenderEntity(*Renderer, entities, entity_num, matrix, total_time);
	      if (!Running)
		{
		  MessageBoxA(hwnd, "Does not worked!", "Entity", MB_OK);
		  Running = false;
		  return 0; 
		}
	      
	      RendererEndScene(*Renderer);
	    }  
	} 
    }
  
  return 0;
}
