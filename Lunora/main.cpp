#include <windows.h>

#include "resource.h"

#include "../Lunora/Game/Scene.h"

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = 0;

  RendererContext* Renderer =
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

      RendererContext* Renderer = new RendererContext();
      Scene *scene = new Scene();
      
      HWND hwnd = CreateWindowExA(0,                   
				  wc.lpszClassName,          
				  "Lunora",  
				  WS_OVERLAPPEDWINDOW | WS_VISIBLE,        
				  CW_USEDEFAULT, CW_USEDEFAULT, x, y, 
				  NULL,       
				  NULL,       
				  Instance,  
				  Renderer);

      
      Running = InitializeScene(scene, hwnd); 
      if (!Running)
	return 1;

      Running = InitializeRenderer(*Renderer, hwnd, SCREEN_WIDTH, SCREEN_HEIGHT);
      if (!Running)
	{
	  return false; 
	}  
   
      scene->Renderer = Renderer;
     
      if (hwnd)
	{	  
	  Running = true;
	  
	  while(Running)
	    {
	      MSG Message;
	      while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
		{		  
		  if (Message.message == WM_QUIT)
		    { 
		      CleanScene(scene);

		      delete scene;
		      scene = nullptr; 
		      
		      Running = false;
		    }
		  
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}

	      if (!Running)
		break;

	      Running = RenderScene(scene);
	      if (!Running)
		{
		  Running = false;
		  return 0; 
		}
	    }  
	} 
    }
  
  return 0;
}
