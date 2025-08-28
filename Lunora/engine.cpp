#include <windows.h>

#include "resource.h"
#include "Renderer.cpp"
#include "Time.cpp"
#include "Camera/Camera.cpp"

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
  Win32WindowProperties* Window = new Win32WindowProperties; 
  
  WNDCLASSEXA wc = {};
  
  wc.cbSize = sizeof(WNDCLASSEXA);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = Instance;
  wc.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_ICON1));;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = "Lunora";
  
  if (RegisterClassExA(&wc))
    {
      int x = CW_USEDEFAULT;
      int y = CW_USEDEFAULT;
     
      Window->hwnd = CreateWindowExA(0,                   
				     wc.lpszClassName,          
				     "Lunora",  
				     WS_OVERLAPPEDWINDOW | WS_VISIBLE,        
				     x, y, CW_USEDEFAULT, CW_USEDEFAULT,
				     NULL,       
				     NULL,       
				     Instance,  
				     NULL);
      
      
  
      if (Window->hwnd)
	{	  
	  Running = true;

	  CoreRenderBuffers Renderer;
	  Window->Width = 1080;
	  Window->Height = 720;
	  Renderer = InitializeD3D(*Window, false);

	  Time Timer;
	  Reset(Timer);

	  Camera* mCamera = new Camera; 
	  
	  while(Running)
	    {
	      MSG Message;
	      while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
		{		  
		  if (Message.message == WM_QUIT)
		    {
		      delete Window; 
		      ShutdownD3D(Renderer);
		      Running = false;
		    }
		  
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}
	      
	      BeginScene(Renderer);
	 
	      EndScene(Renderer);
	    }  
	} 
    }
  
  return 0;
}
