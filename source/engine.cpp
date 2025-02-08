#include <windows.h>
#include "renderer.h"
#include "Renderer.cpp"
#include "resource.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "gdi32")

// RenderManager2D manager2D; 

#define DeleteObject(object) if ((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if ((objects) != NULL) { delete[] objects; objects = NULL; }
#define ReleaseObject(object) if ((object) != NULL) { object->Release(); object = NULL; }

static bool Running;

static ID3D11Device* Device = nullptr;
static ID3D11DeviceContext* DeviceContext = nullptr;
static IDXGISwapChain* SwapChain;
static ID3D11RenderTargetView* RenderTargetView = nullptr;  
static ID3D11DepthStencilView* DepthStencilView = nullptr;
static FLOAT BackgroundColor[4] = {0.141f, 0.137f, 0.365f, 1.f};

void InitializeDX11(HWND Window)
{
  HRESULT Result;
  UINT CreateDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
  CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  
  D3D_FEATURE_LEVEL FeatureLevels[] =
    {
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
    };

  D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_1;
  
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;

  UINT MultiSamplingCount = 4;
  UINT MultiSamplingQualityLevels = 0;
  UINT ScreenWidth = 800;
  UINT ScreenHeight = 600;
  bool MultiSamplingEnabled = false; 

  ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
  SwapChainDesc.OutputWindow = Window;
  SwapChainDesc.Windowed = TRUE;
  SwapChainDesc.BufferDesc.Width = ScreenWidth;
  SwapChainDesc.BufferDesc.Height = ScreenHeight;
  SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  if (MultiSamplingEnabled)
    {
      SwapChainDesc.SampleDesc.Count = MultiSamplingCount;
      SwapChainDesc.SampleDesc.Quality = MultiSamplingQualityLevels - 1;
    }
  else
    {
      SwapChainDesc.SampleDesc.Count = 1;
      SwapChainDesc.SampleDesc.Quality = 0;
    }
   
  SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  SwapChainDesc.BufferCount = 1;
  SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
 
  Result = D3D11CreateDeviceAndSwapChain(NULL, 
					 D3D_DRIVER_TYPE_HARDWARE, 
					 NULL, 
					 CreateDeviceFlags, 
					 FeatureLevels, 
					 ARRAYSIZE(FeatureLevels),
					 D3D11_SDK_VERSION, 
					 &SwapChainDesc,
					 &SwapChain, 
					 &Device, 
					 &FeatureLevel, 
					 &DeviceContext); 
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Device and swap chain", "Error", MB_OK | MB_ICONERROR);
    }
  
  IDXGIDevice* DXGIDevice = nullptr;
  Result = Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DXGIDevice));
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Failed to get DXGI Device", "Error", MB_OK | MB_ICONERROR);
    }
  
  IDXGIAdapter* DXGIAdapter = nullptr;
  Result = DXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&DXGIAdapter));
  if (FAILED(Result))
    {
      MessageBoxA(Window, "DXGI Adapter", "Error", MB_OK | MB_ICONERROR);
    }
    
  Result = Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
						 MultiSamplingCount,
						 &MultiSamplingQualityLevels);
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Multi sample quality levels", "Error.", MB_OK | MB_ICONERROR);
    }

  ReleaseObject(DXGIDevice);
  ReleaseObject(DXGIAdapter);
  
  ID3D11Texture2D* BackBuffer;
  Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Back Buffer failed", "Error.", MB_OK | MB_ICONERROR);
    }

  Result = Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderTargetView); 
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Render target view failed", "Error.", MB_OK | MB_ICONERROR);
    }
  
  ReleaseObject(BackBuffer);

  ID3D11Texture2D* DepthStencilBuffer = nullptr;
  
  D3D11_TEXTURE2D_DESC DepthStencilDesc;
  ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));
  DepthStencilDesc.Width = ScreenWidth;
  DepthStencilDesc.Height = ScreenHeight;
  DepthStencilDesc.MipLevels = 1;
  DepthStencilDesc.ArraySize = 1;
  DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

  if (MultiSamplingEnabled)
    {
      DepthStencilDesc.SampleDesc.Count = MultiSamplingCount;
      DepthStencilDesc.SampleDesc.Quality = MultiSamplingQualityLevels - 1;
    }
  else
    {
      DepthStencilDesc.SampleDesc.Count = 1;
      DepthStencilDesc.SampleDesc.Quality = 0;
    }
  
  Result = Device->CreateTexture2D(&DepthStencilDesc, nullptr, &DepthStencilBuffer);
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Depth Stencil View failed", "Error.", MB_OK | MB_ICONERROR);
    }

  
  Result = Device->CreateDepthStencilView(DepthStencilBuffer, nullptr, &DepthStencilView);
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Depth stencil buffer failed", "Error.", MB_OK | MB_ICONERROR);
    }
  
  DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
  
  D3D11_VIEWPORT Viewport;
  Viewport.TopLeftX = 0.f;
  Viewport.TopLeftY = 0.f;
  Viewport.Width = static_cast<float>(ScreenWidth);
  Viewport.Height = static_cast<float>(ScreenHeight);
  Viewport.MinDepth = 0.f;
  Viewport.MaxDepth = 1.f;

  DeviceContext->RSSetViewports(1, &Viewport);
}

struct MatrixBufferType
{
  XMMATRIX World;
  XMMATRIX View;
  XMMATRIX Projection;
};

void CreateCube()
{
  HRESULT Result; 
  ID3D11Buffer* MatrixBuffer;
  MatrixBufferType* MatrixBufferTypePointer;
  
  D3D11_BUFFER_DESC BufferDesc;

  ZeroMemory(&BufferDesc, sizeof(BufferDesc));
  BufferDesc.ByteWidth = sizeof(MatrixBufferType);
  BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  BufferDesc.MiscFlags = 0;
  BufferDesc.StructureByteStride = 0;
  
  Result = Device->CreateBuffer(&BufferDesc, 0,  &MatrixBuffer);

  if (FAILED(Result))
    {
      MessageBoxA(NULL, "Buffer desc failed", "Error.", MB_OK | MB_ICONERROR);
    }
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
	ReleaseObject(DeviceContext);
	ReleaseObject(RenderTargetView);
	ReleaseObject(DepthStencilView);
	ReleaseObject(SwapChain);
	ReleaseObject(Device);
	
	Running = false;
	PostQuitMessage(0); 
      } break;

    case WM_PAINT:
      {
	CreateCube();
	
	DeviceContext->ClearRenderTargetView(RenderTargetView, BackgroundColor);
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	SwapChain->Present(0, 0);
	// InitializeDX11(Window);
      } break;
      
    case WM_SIZE:
      {
	InitializeDX11(Window);
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

  HRESULT Result; 
  
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
	      
	      DeviceContext->ClearRenderTargetView(RenderTargetView, BackgroundColor);
	      DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	      SwapChain->Present(0, 0);
	    }
	 
	}

    }

   
  return 0;
}


