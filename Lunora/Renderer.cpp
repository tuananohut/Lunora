#include "Renderer.h"

CoreRenderBuffers InitializeD3D(Win32WindowProperties Window,
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
  D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
  HRESULT result;

#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 
  
  result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&DXFactory);
  if (FAILED(result))
    {}
  
  result = DXFactory->EnumAdapters(0, &Adapter);
  if (FAILED(result))
    {}

  result = Adapter->EnumOutputs(0, &Output);    
  
  // for (numModes = 0;
  //   )
  // { 
  result = Output->GetDisplayModeList(format, 0, &numModes, NULL);
  
  displayModes = new DXGI_MODE_DESC[numModes];

  result = Output->GetDisplayModeList(format, 0, &numModes, displayModes);
  // }
  
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
    {}
  
  result = Renderer.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&Renderer.BackBuffer);
  
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
  DepthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
  DepthStencilDesc.SampleDesc.Count = 1;
  DepthStencilDesc.SampleDesc.Quality = 0;
  DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
  DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  DepthStencilDesc.CPUAccessFlags = 0;
  DepthStencilDesc.MiscFlags = 0;

  result = Renderer.Device->CreateTexture2D(&DepthStencilDesc, 0, &Renderer.DepthStencilBuffer);
  if (FAILED(result))
    {}
  
  // result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, "/**/",&DepthView);
    
  return Renderer; 

}
