#include "Renderer.h"

CoreRenderBuffers InitializeD3D(Win32WindowProperties& Window,
				bool fullscreen)
{ 
  CoreRenderBuffers Renderer; 

  IDXGIFactory* DXFactory = NULL;
  IDXGIAdapter* Adapter;
  IDXGIOutput* Output = NULL;
  unsigned int numModes, i, numerator, denominator;
  unsigned long long stringLength; 
  DXGI_MODE_DESC* displayModeList;
  DXGI_ADAPTER_DESC adapterDesc;
  int error; 
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;
  D3D_FEATURE_LEVEL featureLevel;
  ID3D11Texture2D *BackBuffer;
  D3D11_TEXTURE2D_DESC DepthBufferDesc;
  D3D11_DEPTH_STENCIL_DESC DepthStencilDesc; 
  D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
  D3D11_RASTERIZER_DESC rasterDesc; 
  HRESULT result;
  bool vsync_enabled;
  int videoCardMemory;
  char videoCardDescription[128];
  ID3D11DepthStencilState *DepthStencilState; 
  ID3D11RasterizerState *RasterState;

  vsync_enabled = false; 
  
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
 
  result = Output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
				      DXGI_ENUM_MODES_INTERLACED,
				      &numModes, NULL);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Get Display Mode List", "Error", MB_OK | MB_ICONERROR);
    }
 
  displayModeList = new DXGI_MODE_DESC[numModes];

  result = Output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
				      DXGI_ENUM_MODES_INTERLACED,
				      &numModes, displayModeList);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Get Display Mode List", "Error", MB_OK | MB_ICONERROR);
    }

  for (i = 0; i < numModes; i++)
    {
      if (displayModeList[i].Width == (unsigned int)Window.Width)
	{
	  if (displayModeList[i].Height == (unsigned int)Window.Height)
	    {
	      numerator = displayModeList[i].RefreshRate.Numerator;
	      denominator = displayModeList[i].RefreshRate.Denominator; 
	    }	  
	}
    }

  result = Adapter->GetDesc(&adapterDesc);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Get Adapter Desc", "Error", MB_OK | MB_ICONERROR);
    }

  videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
  error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
  if (error != 0)
    {
      MessageBoxA(Window.hwnd, "Video Card Memory", "Error", MB_OK | MB_ICONERROR);
    }

  delete []displayModeList;
  displayModeList = nullptr;

  Output->Release();
  Output = nullptr;

  Adapter->Release();
  Adapter = nullptr; 
  
  DXFactory->Release();
  DXFactory = nullptr; 
  
  ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
  
  SwapChainDesc.BufferCount = 1;
  
  SwapChainDesc.BufferDesc.Width = Window.Width;
  SwapChainDesc.BufferDesc.Height = Window.Height;

  SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  if (vsync_enabled)
    {
      SwapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
      SwapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
  else 
    {
      SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
      SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1; 
    }

  SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  SwapChainDesc.OutputWindow = Window.hwnd;
  
  SwapChainDesc.SampleDesc.Count = 1;
  SwapChainDesc.SampleDesc.Quality = 0;

  if (fullscreen)
    SwapChainDesc.Windowed = false;
  else
    SwapChainDesc.Windowed = true;
  
  SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  
  SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  SwapChainDesc.Flags = 0;

  featureLevel = D3D_FEATURE_LEVEL_11_0; 
  
  result = D3D11CreateDeviceAndSwapChain(NULL,
					 D3D_DRIVER_TYPE_HARDWARE,
					 NULL,
					 0,
					 &featureLevel,
					 1,
					 D3D11_SDK_VERSION,
					 &SwapChainDesc,
					 &Renderer.SwapChain,
					 &Renderer.Device,
					 NULL,
					 &Renderer.DeviceContext);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Could not create Device and Swap Chain", "Error", MB_OK | MB_ICONERROR);
    }
  
  result = Renderer.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Get Buffer", "Error", MB_OK | MB_ICONERROR);
    }
  
  result = Renderer.Device->CreateRenderTargetView(BackBuffer,
						   NULL,
						   &Renderer.RenderTargetView);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Create Render Target View", "Error", MB_OK | MB_ICONERROR);
    }

  BackBuffer->Release();
  BackBuffer = nullptr; 

  ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));
  
  DepthBufferDesc.Width = Window.Width;
  DepthBufferDesc.Height = Window.Height;
  DepthBufferDesc.MipLevels = 1;
  DepthBufferDesc.ArraySize = 1;
  DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthBufferDesc.SampleDesc.Count = 1;
  DepthBufferDesc.SampleDesc.Quality = 0;
  DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  DepthBufferDesc.CPUAccessFlags = 0;
  DepthBufferDesc.MiscFlags = 0;

  result = Renderer.Device->CreateTexture2D(&DepthBufferDesc, 0, &Renderer.DepthStencilBuffer);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Create Texture 2D", "Error", MB_OK | MB_ICONERROR);
    }

  ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));
  
  DepthStencilDesc.DepthEnable = true;
  DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  
  DepthStencilDesc.StencilEnable = true;
  DepthStencilDesc.StencilReadMask = 0xFF;
  DepthStencilDesc.StencilWriteMask = 0xFF;

  DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; 

  DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; 

  result = Renderer.Device->CreateDepthStencilState(&DepthStencilDesc,
						    &DepthStencilState);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Create Depth Stencil State", "Error", MB_OK | MB_ICONERROR);
    }

  Renderer.DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);
  
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

  Renderer.DeviceContext->OMSetRenderTargets(1, &Renderer.RenderTargetView, Renderer.DepthStencilView);

  rasterDesc.AntialiasedLineEnable = false;
  rasterDesc.CullMode = D3D11_CULL_BACK;
  rasterDesc.DepthBias = 0;
  rasterDesc.DepthBiasClamp = 0.f;
  rasterDesc.DepthClipEnable = true;
  rasterDesc.FillMode = D3D11_FILL_SOLID;
  rasterDesc.FrontCounterClockwise = false;
  rasterDesc.MultisampleEnable = false;
  rasterDesc.ScissorEnable = false;
  rasterDesc.SlopeScaledDepthBias = 0;
  
  Renderer.Device->CreateRasterizerState(&rasterDesc, &RasterState);
  if (FAILED(result))
    {
      MessageBoxA(Window.hwnd, "Create Raster Desc", "Error", MB_OK | MB_ICONERROR);
    }

  Renderer.DeviceContext->RSSetState(RasterState); 
  
  Renderer.Viewport.Width = (float)Window.Width;
  Renderer.Viewport.Height = (float)Window.Height;
  Renderer.Viewport.MinDepth = 0.f;
  Renderer.Viewport.MaxDepth = 1.f;
  Renderer.Viewport.TopLeftX = 0.f;
  Renderer.Viewport.TopLeftY = 0.f;

  Renderer.DeviceContext->RSSetViewports(1, &Renderer.Viewport);

  return Renderer; 
}

void BeginScene(CoreRenderBuffers& Renderer)
{
  float color[4];

  color[0] = 0.f;
  color[1] = 0.f;
  color[2] = 0.f;
  color[3] = 1.f;

  Renderer.DeviceContext->ClearRenderTargetView(Renderer.RenderTargetView, color);

  Renderer.DeviceContext->ClearDepthStencilView(Renderer.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void EndScene(CoreRenderBuffers& Renderer)
{
  Renderer.SwapChain->Present(0, 0);
}

void ShutdownD3D(CoreRenderBuffers& Renderer)
{
  if (Renderer.Device)
    {
      Renderer.Device->Release();
      Renderer.Device = nullptr; 
    }
      
  if (Renderer.DeviceContext)
    {
      Renderer.DeviceContext->Release();
      Renderer.DeviceContext = nullptr; 
    }
}
 
