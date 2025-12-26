#include <windows.h>

#include "resource.h"

#include "../Lunora/Game/Scene.h"

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = 0;

  RendererContext *Renderer = 
    (RendererContext*)GetWindowLongPtr(Window, GWLP_USERDATA);

  switch (Message)
    {  
    case WM_CLOSE: 
      {
        Running = false;
      } break;

    case WM_DESTROY:
      {	
	Running = false;
      } break;
      
    case WM_CREATE:
      {
	CREATESTRUCTA* create = (CREATESTRUCTA*)LParam;
	RendererContext* renderer =
	  (RendererContext*)create->lpCreateParams;

	SetWindowLongPtr(Window, GWLP_USERDATA, (LONG_PTR)renderer);
      } break;

    case WM_SIZE:
      {
	if (WParam == SIZE_MINIMIZED) break;
	
	RECT rect;
	GetClientRect(Window, &rect);
	
	int clientWidth  = rect.right - rect.left;
	int clientHeight = rect.bottom - rect.top;

	SCREEN_WIDTH = (int)clientWidth;  
	SCREEN_HEIGHT = (int)clientHeight;

	if (Renderer && Renderer->Device && Renderer->DeviceContext && Renderer->SwapChain)
	  {
	    if (ResizeRenderer(*Renderer, SCREEN_WIDTH, SCREEN_HEIGHT))
	      {}
	    else
	      Running = false;
	    
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

      RendererContext *Renderer = new RendererContext();

      HWND hwnd = CreateWindowExA(0,                   
				  wc.lpszClassName,          
				  "Lunora",  
				  WS_OVERLAPPEDWINDOW | WS_VISIBLE,        
				  CW_USEDEFAULT, CW_USEDEFAULT, x, y, 
				  NULL,       
				  NULL,       
				  Instance,  
				  Renderer);
      
      if (hwnd)
	{	  
	  Running = true;

	  Running = InitializeRenderer(*Renderer, hwnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	  if (!Running)
	    {
	      Running = false; 
	    }

	  Camera *mCamera = new Camera;
	  mCamera->SetPosition(5.0f, 0.0f, -15.0f);

	  const size_t entity_num = 3; 
	  Entity *entities[entity_num];	   

	  for (size_t i = 0; i < entity_num; ++i)
	    entities[i] = new Entity();
  
	  Running = InitializeEntity(entities, entity_num, *Renderer);
	  if (!Running)
	    {
	      Running = false;
	    }
	  
	  MatrixBufferType matrix;

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
		      
		      Running = false;
		    }
		  
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}

	      if (!Running)
		break;
	      
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
		  Running = false;
		  return 0; 
		}
	      
	      RendererEndScene(*Renderer);
	    }  
	} 
    }
  
  return 0;
}
