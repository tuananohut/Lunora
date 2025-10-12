#include <windows.h>

#include "resource.h"
#include "Renderer.h"
#include "Camera/Camera.h"
#include "Geometry.h"
#include "Shader.h"

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
	  
	  CoreRenderBuffers *Renderer = new CoreRenderBuffers;
	  *Renderer = InitializeD3D(*Window, false);
	  
	  Camera *mCamera = new Camera;
	  mCamera->SetPosition(0.0f, 0.0f, -10.0f);

	  ModelBuffer *mModelBuffer = new ModelBuffer;

	  char filename[] = "../Lunora/cube.txt";
	  
	  Running = InitializeModel(*Renderer, mModelBuffer, filename); 
	  if (FAILED(Running))
	    {
	      MessageBoxA(Window->hwnd, "Worked!", "Good", MB_OK);
	      Running = false; 
	    }
	  
	  if (FAILED(InitializeShaderResources(*Renderer)))
	    {
	      (Window->hwnd, "Something is wrong!", "Bad", MB_OK | MB_ICONERROR);
	    }
	  
	  LARGE_INTEGER frequency;
	  LARGE_INTEGER startTime;
	  QueryPerformanceFrequency(&frequency); 
	  QueryPerformanceCounter(&startTime);

	  float rotationSpeed = 2.0f;
	  
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

		      if (mModelBuffer)
			{
			  ReleaseModel(*mModelBuffer); 
			  delete mModelBuffer;
			  mModelBuffer = nullptr; 
			} 
		      
		      if (Renderer)
			{
			  delete Renderer;
			  Renderer = nullptr; 
			}
		      ShutdownD3D(*Renderer);
		      Running = false;
		    }
		  
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}
	      
	      BeginScene(*Renderer);

	      mCamera->Render();

	      RECT clientRect;
	      GetClientRect(Window->hwnd, &clientRect);
	      long windowWidth = clientRect.right - clientRect.left;
	      long windowHeight = clientRect.bottom - clientRect.top;

	      Renderer->Viewport.Width = (float)windowWidth;
	      Renderer->Viewport.Height = (float)windowHeight;
	      Renderer->DeviceContext->RSSetViewports(1, &Renderer->Viewport);

	      LARGE_INTEGER currentTime;
	      QueryPerformanceCounter(&currentTime);

	      float elapsedTime = (float)(currentTime.QuadPart - startTime.QuadPart) / (float)frequency.QuadPart;

	      XMMATRIX world = XMMatrixRotationX(elapsedTime * 0.7f) * XMMatrixRotationY(elapsedTime);

	      // XMMATRIX world = XMMatrixIdentity();
	      XMMATRIX view;	      
	      mCamera->GetViewMatrix(view);
  
	      float fieldOfView = 3.141592654f / 4.0f;
	      float screenAspect = (float)Window->Width / (float)Window->Height;
	      
	      XMMATRIX proj  = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
	      
	      RenderModel(*Renderer, *mModelBuffer);
	      
	      result = Render(*Renderer, world, view, proj);
	      if (FAILED(result))
		{
	       Running = false; 
		}
	      
	      EndScene(*Renderer);
	    }  
	} 
    }
  
  return 0;
}
