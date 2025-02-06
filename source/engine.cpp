#include <windows.h>
#include "renderer.h"
#include "Renderer.cpp"
#include "resource.h"
#include <d3d11.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "gdi32")

// RenderManager2D manager2D; 

static bool Running;

void InitializeDX11(HWND Window)
{
  HRESULT Result;
  D3D_FEATURE_LEVEL FeatureLevels[] =
    {
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
    };
  ID3D11Device* Device;
  D3D_FEATURE_LEVEL SelectedFeatureLevel;
  ID3D11DeviceContext* DeviceContext;
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;
  IDXGISwapChain* SwapChain;

  ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
  SwapChainDesc.BufferDesc.Width = 1280;
  SwapChainDesc.BufferDesc.Height = 720;
  SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  Result = D3D11CreateDeviceAndSwapChain(NULL,
					 D3D_DRIVER_TYPE_HARDWARE,
					 NULL,
					 0,
					 FeatureLevels,
					 1,
					 D3D11_SDK_VERSION,
					 &SwapChainDesc,
					 &SwapChain, 
					 &Device,
					 NULL,
					 &DeviceContext);
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Error", "Device released successfully.", MB_OK | MB_ICONINFORMATION);
    }
    
  
  // Swap chain
  
  /*
  if (Device)
    {
      Device->Release();
      Device = nullptr;
      MessageBoxA(Window, "Congrats", "Device released successfully.", MB_OK | MB_ICONINFORMATION);
    }
  
  if (DeviceContext)
    {
      DeviceContext->Release();
      DeviceContext = nullptr;
      MessageBoxA(Window, "Congrats", "Device context released successfully.", MB_OK | MB_ICONINFORMATION);
    }

  if (DXGISwapChain)
    {
      DXGISwapChain->Release();
      DXGISwapChain = nullptr;
      MessageBoxA(Window, "Congrats", "DXGI Swap Chain released successfully.", MB_OK | MB_ICONINFORMATION);
    }

    if (RenderTargetView)
    {
      RenderTargetView->Release();
      RenderTargetView = nullptr;
      MessageBoxA(Window, "Congrats", "Render Target View released successfully.", MB_OK | MB_ICONINFORMATION);
    }
  */
}

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = DefWindowProc(Window, Message, WParam, LParam);

  switch (Message)
    {
      
    case WM_CREATE:
      {
	InitializeDX11(Window);
      } break;
      
    case WM_CLOSE: 
      {
        // TODO: Handle this with a message to the user.
        Running = false;
	DestroyWindow(Window);
      } break;

    case WM_DESTROY:
      {
        Running = false;
	PostQuitMessage(0); 
      } break;

    case WM_PAINT:
      {
	// InitializeDX11(Window);
      } break;
      
    case WM_SIZE:
      {

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
  
  DWORD wStyles = WS_EX_ACCEPTFILES | WS_EX_TOOLWINDOW | WS_OVERLAPPEDWINDOW;

  wc.cbSize = sizeof(WNDCLASSEXA);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC; // Try OWNDC 
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = Instance;
  wc.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_ICON1));;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName = 0; 
  wc.lpszClassName = "Lunora";

  if (RegisterClassExA(&wc))
    {
      int x = CW_USEDEFAULT;
      int y = CW_USEDEFAULT;

      HWND Window = CreateWindowExA(0,                   
				    wc.lpszClassName,          
				    "Lunora",  
				    wStyles,        
				    x, y, 1280, 720,
				    NULL,       
				    NULL,       
				    Instance,  
				    NULL);

      
  
      if (Window)
	{
	  ShowWindow(Window, ShowCode);
	  UpdateWindow(Window);
	  
	  InitializeDX11(Window); // You can't call initializing code in a while loop.  
	  // manager2D.GetHwnd(Window);
	  
	  Running = true;
	  while(Running)
	    {
	      MSG Message;
	      while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
		{		  
		  if (Message.message == WM_QUIT)
		    {
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


