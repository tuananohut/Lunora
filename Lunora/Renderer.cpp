#include "Renderer.h"

CoreRenderBuffers InitializeD3D(Win32WindowProperties& Window,
				bool fullscreen)
{ 
  CoreRenderBuffers Renderer; 

  IDXGIFactory* DXFactory = NULL;
  IDXGIAdapter* Adapter;
  IDXGIOutput* Output = NULL;
  UINT numModes = 0;
  DXGI_MODE_DESC* displayModes = NULL;
  DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT; 
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;
  const D3D_FEATURE_LEVEL FeatureLevelsRequested[] = { D3D_FEATURE_LEVEL_11_1,
						       D3D_FEATURE_LEVEL_11_0,
						       D3D_FEATURE_LEVEL_10_1,
						       D3D_FEATURE_LEVEL_10_0,
						       D3D_FEATURE_LEVEL_9_3 ,
						       D3D_FEATURE_LEVEL_9_2 ,
						       D3D_FEATURE_LEVEL_9_1 };
  UINT createDeviceFlags = 0; 
  D3D_FEATURE_LEVEL FeatureLevelsSupported;
  D3D11_TEXTURE2D_DESC DepthStencilDesc;
  D3D11_DEPTH_STENCIL_DESC DepthStencilStateDesc; 
  D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
  D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;
  HRESULT result;

#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 
  
  result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&DXFactory);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "DXGI Factory", "Error", MB_OK | MB_ICONERROR);
    }
  
  result = DXFactory->EnumAdapters(0, &Adapter);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Enum Adapters", "Error", MB_OK | MB_ICONERROR);
    }

  result = Adapter->EnumOutputs(0, &Output);    
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Enum Outputs", "Error", MB_OK | MB_ICONERROR);
    }
 
  result = Output->GetDisplayModeList(format, 0, &numModes, NULL);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Get Display Mode List", "Error", MB_OK | MB_ICONERROR);
    }
 
  displayModes = new DXGI_MODE_DESC[numModes];

  result = Output->GetDisplayModeList(format, 0, &numModes, displayModes);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Get Display Mode List", "Error", MB_OK | MB_ICONERROR);
    }
   
  if (DXFactory)
    {
      DXFactory->Release();
    }
  
  ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
  SwapChainDesc.BufferCount = 1;
  SwapChainDesc.BufferDesc.Width = Window.Width;
  SwapChainDesc.BufferDesc.Height = Window.Height;
  SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1; 
  SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
  SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  SwapChainDesc.SampleDesc.Count = 1;
  SwapChainDesc.SampleDesc.Quality = 0;
  SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  SwapChainDesc.OutputWindow = Window.hwnd;
  if (fullscreen)
    SwapChainDesc.Windowed = FALSE;
  else
    SwapChainDesc.Windowed = TRUE;
  SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  SwapChainDesc.Flags = 0;
  
  result = D3D11CreateDeviceAndSwapChain(NULL,
					 D3D_DRIVER_TYPE_HARDWARE,
					 NULL,
					 createDeviceFlags,
					 FeatureLevelsRequested,
					 ARRAYSIZE(FeatureLevelsRequested),
					 D3D11_SDK_VERSION,
					 &SwapChainDesc,
					 &Renderer.SwapChain,
					 &Renderer.Device,
					 &FeatureLevelsSupported,
					 &Renderer.DeviceContext);
  if (result == E_INVALIDARG)
    {
      result = D3D11CreateDeviceAndSwapChain(NULL,
					     D3D_DRIVER_TYPE_HARDWARE,
					     NULL,
					     createDeviceFlags,
					     &FeatureLevelsRequested[1],
					     ARRAYSIZE(FeatureLevelsRequested) - 1,
					     D3D11_SDK_VERSION,
					     &SwapChainDesc,
					     &Renderer.SwapChain,
					     &Renderer.Device,
					     &FeatureLevelsSupported,
					     &Renderer.DeviceContext);
    }
  
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Could not create Device and Swap Chain", "Error", MB_OK | MB_ICONERROR);
    }
  
  result = Renderer.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&Renderer.BackBuffer);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Get Buffer", "Error", MB_OK | MB_ICONERROR);
    }
 
  ZeroMemory(&Renderer.Viewport, sizeof(D3D11_VIEWPORT));
  Renderer.Viewport.Width = Window.Width;
  Renderer.Viewport.Height = Window.Height;
  Renderer.Viewport.MinDepth = 0.f;
  Renderer.Viewport.MaxDepth = 1.f;
  Renderer.Viewport.TopLeftX = 0;
  Renderer.Viewport.TopLeftY = 0;

  Renderer.DeviceContext->RSSetViewports(1, &Renderer.Viewport);

  ZeroMemory(&DepthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
  DepthStencilDesc.Width = Window.Width;
  DepthStencilDesc.Height = Window.Height;
  DepthStencilDesc.MipLevels = 1;
  DepthStencilDesc.ArraySize = 1;
  DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthStencilDesc.SampleDesc.Count = 1;
  DepthStencilDesc.SampleDesc.Quality = 0;
  DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
  DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  DepthStencilDesc.CPUAccessFlags = 0;
  DepthStencilDesc.MiscFlags = 0;

  result = Renderer.Device->CreateTexture2D(&DepthStencilDesc, 0, &Renderer.DepthStencilBuffer);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Create Texture 2D", "Error", MB_OK | MB_ICONERROR);
    }
  
  ZeroMemory(&DepthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

  // !--- CHECK FOR DEPTH STENCIL TESTS ---! //
  
  // Depth test parameters 
  DepthStencilStateDesc.DepthEnable = true;
  DepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

  // Stencil test parameters 
  DepthStencilStateDesc.StencilEnable = true;
  DepthStencilStateDesc.StencilReadMask = 0xFF;
  DepthStencilStateDesc.StencilWriteMask = 0xFF;

  // Stencil operations if pixel is front-facing
  DepthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  DepthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; 

  // Stencil operations if pixel is back-facing 
  DepthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  DepthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; 

  result = Renderer.Device->CreateDepthStencilState(&DepthStencilStateDesc,
						    &Renderer.DepthStencilState);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Create Depth Stencil State", "Error", MB_OK | MB_ICONERROR);
    }

  Renderer.DeviceContext->OMSetDepthStencilState(Renderer.DepthStencilState, 1); 
  
  ZeroMemory(&DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
  DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  DepthStencilViewDesc.Texture2D.MipSlice = 0;
    
  result = Renderer.Device->CreateDepthStencilView(Renderer.DepthStencilBuffer,
						   &DepthStencilViewDesc,
						   &Renderer.DepthStencilView);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Create Depth Stencil State", "Error", MB_OK | MB_ICONERROR);
    }

  
  result = Renderer.Device->CreateRenderTargetView(Renderer.BackBuffer,
						   NULL,
						   &Renderer.RenderTargetView);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Create Render Target View", "Error", MB_OK | MB_ICONERROR);
    }

  Renderer.DeviceContext->OMSetRenderTargets(1, &Renderer.RenderTargetView,
					     Renderer.DepthStencilView);

  
  return Renderer; 
}

void ShutdownD3D(CoreRenderBuffers& Renderer)
{
  if (Renderer.SwapChain)
    {
      delete Renderer.SwapChain;
      Renderer.SwapChain->Release();
      Renderer.SwapChain = nullptr; 
    }

  if (Renderer.Device)
    {
      delete Renderer.Device;
      Renderer.Device->Release();
      Renderer.Device = nullptr; 
    }
      
  if (Renderer.DeviceContext)
    {
      delete Renderer.DeviceContext;
      Renderer.DeviceContext->Release();
      Renderer.DeviceContext = nullptr; 
    }
      
  if (Renderer.BackBuffer)
    {
      delete Renderer.BackBuffer;
      Renderer.BackBuffer->Release();
      Renderer.BackBuffer = nullptr; 
    }

  if (Renderer.RenderTargetView)
    {
      delete Renderer.RenderTargetView;
      Renderer.RenderTargetView->Release();
      Renderer.RenderTargetView = nullptr; 
    }
      
  if (Renderer.DepthStencilBuffer)
    {
      delete Renderer.DepthStencilBuffer;
      Renderer.DepthStencilBuffer->Release();
      Renderer.DepthStencilBuffer = nullptr; 
    }
      
  if (Renderer.DepthStencilState)
    {
      delete Renderer.DepthStencilState;
      Renderer.DepthStencilState->Release();
      Renderer.DepthStencilState = nullptr; 
    }
      
  if (Renderer.DepthStencilView)
    {
      delete Renderer.DepthStencilView;
      Renderer.DepthStencilView->Release();
      Renderer.DepthStencilView = nullptr; 
    }
          
}
