#include <windows.h>

#include "managers.h"
#include "resource.h"
#include "renderer.h"
#include "Input/InputHandler.h"
#include "Camera/FreeCamera.h"
#include "Mesh/Terrain/TerrainMeshData.h"
#include "Shader/ColorShader.h"

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

	  Terrain terrain;
	  terrain.InitializeBuffers(DeviceManager.Device);

	  ColorShader shader;
	  shader.InitializeShader(DeviceManager.Device, Window,
				  L"../Lunora/Assets/color.vs",
				  L"../Lunora/Assets/color.ps");
	  
	  
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

		      terrain.ShutdownBuffers();
		      shader.ShutdownShader();
			
		      Running = false;
		    }
		  
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}
	      
	      float color[4] = { 1.f, 0.f, 1.f, 1.f };
	      
	      DeviceManager.DeviceContext->ClearRenderTargetView(RenderTargetManager.RenderTargetView, color);

	      DeviceManager.DeviceContext->ClearDepthStencilView(RenderTargetManager.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	      XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	      
	      terrain.RenderBuffers(DeviceManager.DeviceContext);
	      shader.SetShaderParameters(DeviceManager.DeviceContext,
					 worldMatrix,
					 viewMatrix,
					 projectionMatrix);
	      shader.RenderShader(DeviceManager.DeviceContext,
				  terrain.GetIndexCount());
	      
	      DeviceManager.SwapChain->Present(1, 0);
	    }
	  
	      
	} 
    }
  
  return 0;
}
