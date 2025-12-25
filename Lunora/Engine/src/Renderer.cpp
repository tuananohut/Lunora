#include "Renderer.h"

namespace LunoraEngine {
  bool InitializeRenderer(RendererContext& context,
			  HWND hwnd,
			  int width,
			  int height)
  { 
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
	MessageBoxA(hwnd, "DXGI Factory", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }
  
    result = DXFactory->EnumAdapters(0, &Adapter);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Enum Adapters", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }

    result = Adapter->EnumOutputs(0, &Output);    
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Enum Outputs", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }
 
    result = Output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
					DXGI_ENUM_MODES_INTERLACED,
					&numModes, NULL);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Get Display Mode List", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }
 
    displayModeList = new DXGI_MODE_DESC[numModes];

    result = Output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
					DXGI_ENUM_MODES_INTERLACED,
					&numModes, displayModeList);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Get Display Mode List", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }
  
    for (i = 0; i < numModes; i++)
      {
	if (displayModeList[i].Width == (unsigned int)width)
	  {
	    if (displayModeList[i].Height == (unsigned int)height)
	      {
		numerator = displayModeList[i].RefreshRate.Numerator;
		denominator = displayModeList[i].RefreshRate.Denominator; 
	      }	  
	  }
      }

    result = Adapter->GetDesc(&adapterDesc);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Get Adapter Desc", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }

    videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
    error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0)
      {
	MessageBoxA(hwnd, "Video Card Memory", "Error", MB_OK | MB_ICONERROR);
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
  
    SwapChainDesc.BufferDesc.Width = width;
    SwapChainDesc.BufferDesc.Height = height;

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

    SwapChainDesc.OutputWindow = hwnd;
  
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;

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
					   &context.SwapChain,
					   &context.Device,
					   NULL,
					   &context.DeviceContext);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Could not create Device and Swap Chain", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }
  
    result = context.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&context.BackBuffer);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Get Buffer", "Error", MB_OK | MB_ICONERROR);
	return false;
      }
  
    result = context.Device->CreateRenderTargetView(context.BackBuffer,
						    NULL,
						    &context.RenderTargetView);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Create Render Target View", "Error", MB_OK | MB_ICONERROR);
	return false;
      }

    context.BackBuffer->Release();
    context.BackBuffer = nullptr; 

    ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));
  
    DepthBufferDesc.Width = width;
    DepthBufferDesc.Height = height;
    DepthBufferDesc.MipLevels = 1;
    DepthBufferDesc.ArraySize = 1;
    DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DepthBufferDesc.SampleDesc.Count = 1;
    DepthBufferDesc.SampleDesc.Quality = 0;
    DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DepthBufferDesc.CPUAccessFlags = 0;
    DepthBufferDesc.MiscFlags = 0;

    result = context.Device->CreateTexture2D(&DepthBufferDesc, 0, &context.DepthStencilBuffer);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Create Texture 2D", "Error", MB_OK | MB_ICONERROR);
	return false; 
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

    result = context.Device->CreateDepthStencilState(&DepthStencilDesc,
						     &DepthStencilState);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Create Depth Stencil State", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }

    context.DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);
  
    ZeroMemory(&DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

    DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DepthStencilViewDesc.Texture2D.MipSlice = 0;
    
    result = context.Device->CreateDepthStencilView(context.DepthStencilBuffer,
						    &DepthStencilViewDesc,
						    &context.DepthStencilView);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Create Depth Stencil State", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }

    context.DeviceContext->OMSetRenderTargets(1, &context.RenderTargetView, context.DepthStencilView);
  
    rasterDesc.AntialiasedLineEnable = true;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
    rasterDesc.DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;

  
    context.Device->CreateRasterizerState(&rasterDesc, &RasterState);
    if (FAILED(result))
      {
	MessageBoxA(hwnd, "Create Raster Desc", "Error", MB_OK | MB_ICONERROR);
	return false; 
      }

    context.DeviceContext->RSSetState(RasterState); 
  
    context.Viewport.Width = static_cast<float>(SCREEN_WIDTH);
    context.Viewport.Height = static_cast<float>(SCREEN_HEIGHT);
    context.Viewport.MinDepth = 0.f;
    context.Viewport.MaxDepth = 1.f;
    context.Viewport.TopLeftX = 0.f;
    context.Viewport.TopLeftY = 0.f;

    context.DeviceContext->RSSetViewports(1, &context.Viewport);

    return true; 
  }

  void RendererBeginScene(RendererContext& context, float r, float g, float b, float a)
  {
    float color[4] = {r, g, b, a};
  
    context.DeviceContext->ClearRenderTargetView(context.RenderTargetView, color);

    context.DeviceContext->ClearDepthStencilView(context.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
  }

  bool ResizeRenderer(RendererContext& context, int width, int height)
  {
    HRESULT result;

    assert(context.DeviceContext);
    assert(context.Device);
    assert(context.SwapChain);
    
    if (context.RenderTargetView)
      {
	context.RenderTargetView->Release();
	context.RenderTargetView = nullptr;
      }
    
    if (context.DepthStencilView)
      {
	context.DepthStencilView->Release();
	context.DepthStencilView = nullptr;
      }

    if (context.DepthStencilBuffer)
      {
	context.DepthStencilBuffer->Release();
	context.DepthStencilBuffer = nullptr;
      }
    
    result = context.SwapChain->ResizeBuffers(1, width, height,
					      DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (FAILED(result))
      {
	return false; 
      }

    result = context.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&context.BackBuffer);
    if (FAILED(result))
      {
	return false; 
      }
    
    result = context.Device->CreateRenderTargetView(context.BackBuffer, 0, &context.RenderTargetView); 
    if (FAILED(result))
      {
	return false; 
      }
    
    context.BackBuffer->Release();
    context.BackBuffer = nullptr; 

    D3D11_TEXTURE2D_DESC DepthBufferDesc;
    
    ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));
    
    DepthBufferDesc.Width = width;
    DepthBufferDesc.Height = height;
    DepthBufferDesc.MipLevels = 1;
    DepthBufferDesc.ArraySize = 1;
    DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DepthBufferDesc.SampleDesc.Count = 1;
    DepthBufferDesc.SampleDesc.Quality = 0;
    DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DepthBufferDesc.CPUAccessFlags = 0;
    DepthBufferDesc.MiscFlags = 0;

    result = context.Device->CreateTexture2D(&DepthBufferDesc, 0,
					     &context.DepthStencilBuffer);
    if (FAILED(result))
      {
	return false; 
      }

    result = context.Device->CreateDepthStencilView(context.DepthStencilBuffer,
						    0,
						    &context.DepthStencilView);
    if (FAILED(result))
      {
	return false; 
      }
    
    context.DeviceContext->OMSetRenderTargets(1, &context.RenderTargetView,
					      context.DepthStencilView);

    context.Viewport.Width = (float)width;
    context.Viewport.Height = (float)height;
    context.Viewport.MinDepth = 0.f;
    context.Viewport.MaxDepth = 1.f;
    context.Viewport.TopLeftX = 0.f;
    context.Viewport.TopLeftY = 0.f;
    
    context.DeviceContext->RSSetViewports(1, &context.Viewport);

    return true; 
  }
  
  void RendererEndScene(RendererContext& context)
  {
    context.SwapChain->Present(0, 0);
  }

  void ShutdownRenderer(RendererContext& context)
  {
    if (context.Device)
      {
	context.Device->Release();
	context.Device = nullptr; 
      }
      
    if (context.DeviceContext)
      {
	context.DeviceContext->Release();
	context.DeviceContext = nullptr; 
      }

    if (context.RenderTargetView)
      {
	context.RenderTargetView->Release();
	context.RenderTargetView = nullptr; 
      }

    if (context.DepthStencilBuffer)
      {
	context.DepthStencilBuffer->Release();
	context.DepthStencilBuffer = nullptr; 
      }

    if (context.DepthStencilView)
      {
	context.DepthStencilView->Release();
	context.DepthStencilView = nullptr; 
      }

    if (context.SwapChain)
      {
	context.SwapChain->Release();
	context.SwapChain = nullptr; 
      }
  }
};
