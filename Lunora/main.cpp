#include <windows.h>

#include "resource.h"
#include "../Lunora/Engine/Renderer.h"
#include "../Lunora/Game/Camera/Camera.h"
#include "../Lunora/Game/Entity.h"
#include "../Lunora/Rendering/Mesh.h"
#include "../Lunora/Rendering/Shader/Shader.h"
#include "../Lunora/Rendering/Texture.h"

const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.3f;

static bool Running; 

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = 0;
 
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
  Win32WindowProperties* Window = new Win32WindowProperties;
  Window->Width = 800;
  Window->Height = 600;	  
  
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
      int x = Window->Width;
      int y = Window->Height;
 
      Window->hwnd = CreateWindowExA(0,                   
				     wc.lpszClassName,          
				     "Lunora",  
				     WS_OVERLAPPEDWINDOW | WS_VISIBLE,        
				     CW_USEDEFAULT, CW_USEDEFAULT, x, y, 
				     NULL,       
				     NULL,       
				     Instance,  
				     NULL);
      
      
  
      if (Window->hwnd)
	{	  
	  Running = true;
	  
	  RendererContext *Renderer = new RendererContext;
	  Running = InitializeRenderer(*Renderer, Window->hwnd, Window->Width, Window->Height);
	  if (!Running)
	    {
	      MessageBoxA(Window->hwnd, "Worked!", "Good", MB_OK);
	      Running = false;
	      return 0; 
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
	      MessageBoxA(Window->hwnd, "Does not worked!", "Entity", MB_OK);
	      Running = false;
	      return 0; 
	    }

	  MatrixBufferType matrix; 
	  
	  matrix.world = XMMatrixIdentity();      
	  mCamera->GetViewMatrix(matrix.view);
  
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
  
	  matrix.proj  = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

	  
	  /*
	  Texture* texture = new Texture;
	  const char* texture_file = "../Assets/Textures/palestine.tga";  
	  result = InitializeTexture(Renderer->Device,
				     Renderer->DeviceContext,
				     texture, texture_file);
	  if (FAILED(result))
	    {
	      MessageBoxA(Window->hwnd, "Something is wrong about textures!", "Bad", MB_OK | MB_ICONERROR);
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
		      delete Window; 
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
		      
		      if (entities)
			{
			  ReleaseEntity(entities, entity_num);
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
	      if (Window->Height > 0)
		{
		  screenAspect = (float)Window->Width / (float)Window->Height;
		}
	      else
		{
		  screenAspect = 1.0f; 
		}
  
	      matrix.proj  = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
	      
	      Running = RenderEntity(*Renderer, entities, entity_num, matrix, total_time);
	      if (!Running)
		{
		  MessageBoxA(Window->hwnd, "Does not worked!", "Entity", MB_OK);
		  Running = false;
		  return 0; 
		}
	      
	      RendererEndScene(*Renderer);
	    }  
	} 
    }
  
  return 0;
}
