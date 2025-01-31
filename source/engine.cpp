#include <windows.h>
#include "renderer.h"
#include "Renderer.cpp"
#include "resource.h"
#include <d3d11.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "gdi32")

RenderManager2D manager2D; 

static bool Running;

static void InitializeDX11(HWND Window)
{
  HRESULT result;
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;
  ID3D11Device* Device = nullptr;
  ID3D11DeviceContext* DeviceContext = nullptr; 
  IDXGISwapChain* DXGISwapChain = nullptr;              
  
  ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
  SwapChainDesc.BufferCount = 1;
  SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  SwapChainDesc.OutputWindow = Window;
  SwapChainDesc.SampleDesc.Count = 4;
  SwapChainDesc.Windowed = TRUE;

  D3D_FEATURE_LEVEL FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
  UINT numFeatureLevelsRequested = 1;
  D3D_FEATURE_LEVEL FeatureLevelsSupported;

  result = D3D11CreateDeviceAndSwapChain(NULL,
					 D3D_DRIVER_TYPE_HARDWARE,
					 NULL,
					 0,
					 &FeatureLevelsRequested,
					 numFeatureLevelsRequested,
					 D3D11_SDK_VERSION,
					 &SwapChainDesc,
					 &DXGISwapChain,
					 &Device,
					 &FeatureLevelsSupported,
					 &DeviceContext);
  if (FAILED(result))
    {
      MessageBoxA(Window, "Error", "Could not create device.", MB_OK | MB_ICONERROR);
    }
  else
    {
      MessageBoxA(Window, "Congrates", "Device created successfully.", MB_OK | MB_ICONINFORMATION);
    }
}


LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
  WNDCLASSA wc = {};

  DWORD wStyles = WS_EX_ACCEPTFILES | WS_EX_TOOLWINDOW | WS_OVERLAPPEDWINDOW;

  wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wc.lpfnWndProc = WindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = Instance;
  wc.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_ICON1));;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName = NULL; 
  wc.lpszClassName = "Lunora";

  if (!RegisterClassA(&wc))
    {
      DWORD dwError = GetLastError();
      if (dwError != ERROR_CLASS_ALREADY_EXISTS)
        {
	  return HRESULT_FROM_WIN32(dwError);
        }
    }

  int x = CW_USEDEFAULT;
  int y = CW_USEDEFAULT;

  HWND Window = CreateWindowExA(0,                   
				wc.lpszClassName,          
				"Lunora",  
				wStyles,        
				x, y, x, y,
				NULL,       
				NULL,       
				Instance,  
				NULL);

  if (Window)
    {
      ShowWindow(Window, ShowCode);
      manager2D.GetHwnd(Window);  
      InitializeDX11(Window);
      
      MSG Message;
      Running = true; 
      while(Running)
        {
	  BOOL MessageResult = GetMessageA(&Message, NULL, 0, 0);
	  if (MessageResult)
            {
	      TranslateMessage(&Message);
	      DispatchMessageA(&Message);
            }
	  else
            {
	      // renderer->Update(); 
	      // renderer->Render();
	      // deviceResources->Present();
	      manager2D.~RenderManager2D();
	      break;
            }
        }
    }
   
  return 0;
}

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = 0;

  switch (Message)
    {
    case WM_CREATE:
      {
	//	InitializeDX11(Window);
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &manager2D.Factory)))
	  {
            return -1;
	  }
      } break;

    case WM_CLOSE: 
      {
        // TODO: Handle this with a message to the user. 
        Running = false;
      } break;

    case WM_DESTROY:
      {
        Running = false;
        manager2D.Shutdown();
      } break;

    case WM_PAINT:
      {
        manager2D.OnPaint();
      } break;

    case WM_SIZE:
      {
        manager2D.Resize();
      } break;

    default:
      {
        Result = DefWindowProc(Window, Message, WParam, LParam);
      } break;
    }
    
  return Result;
}
