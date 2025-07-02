#include <windows.h>
#include <d3dcompiler.h>

#include "managers.cpp"
#include "resource.h"
#include "renderer.cpp"
#include "Input/InputHandler.h"
#include "Camera/FreeCamera.h"
#include "Mesh/Terrain/TerrainMeshData.h"

static bool Running; 

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = 0;
  HRESULT ResultE;
  
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
  
  HRESULT Result; 
  
  if (RegisterClassExA(&wc))
    {
      int x = CW_USEDEFAULT;
      int y = CW_USEDEFAULT;

      HWND Window = CreateWindowExA(0,                   
				    wc.lpszClassName,          
				    "Lunora",  
				    WS_OVERLAPPEDWINDOW | WS_VISIBLE,        
				    x, y, ScreenWidth, ScreenHeight,
				    NULL,       
				    NULL,       
				    Instance,  
				    NULL);
      
      
  
      if (Window)
	{
	  DeviceManager DeviceManager;
	  DeviceManager.Initialize(Window, ScreenWidth, ScreenHeight);
	  
	  RenderTargetManager RenderTargetManager;
	  RenderTargetManager.Initialize(DeviceManager, ScreenWidth, ScreenHeight);
	  
	  PipelineStateManager PipelineStateManager; 
	  PipelineStateManager.Initialize(DeviceManager);
	 
	  Running = true;
	  while(Running)
	    {
	      MSG Message;
	      while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
		{		  
		  if (Message.message == WM_QUIT)
		    {
		      DeviceManager.Cleanup();
		      RenderTargetManager.Cleanup();
		      PipelineStateManager.Cleanup();	     
		      
		      Running = false;
		    }
		  
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}
	    } 
	}
    }
   
  return 0;
}
