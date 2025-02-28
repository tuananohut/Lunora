#include "renderer.h"

static void InitializeDX11(HWND Window)
{
  HRESULT Result;
  UINT CreateDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
  CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  
  D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_1;

  IDXGIFactory *DXGIFactory;
  IDXGIAdapter *Adapter = nullptr;
  IDXGIOutput *Display;
  UINT NumModes, Numerator, Denominator;

  DXGI_MODE_DESC *DisplayModeList;
  DXGI_ADAPTER_DESC AdapterDesc; 
  
  Result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&DXGIFactory); 
  if (FAILED(Result))
    {
      OutputDebugStringA("Failed to get DXGI Factory");
    }

  Result = DXGIFactory->EnumAdapters(0, &Adapter);
  if (FAILED(Result))
    {
      OutputDebugStringA("There is no adapter");
    }
  
  Result = Adapter->EnumOutputs(0, &Display);
  if (FAILED(Result))
    {
      OutputDebugStringA("There is no display");
    }

  Result = Display->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumModes, NULL);
  if (FAILED(Result))
    {
      OutputDebugStringA("GetDisplayModeList did not work");
    }

  DisplayModeList = new DXGI_MODE_DESC[NumModes];
  if(!DisplayModeList)
    {
      OutputDebugStringA("Could not create DisplayModeList");
    }

  Result = Display->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumModes, DisplayModeList);
  if (FAILED(Result))
    {
      OutputDebugStringA("GetDisplayModeList with DisplayModeList did not work");
    }
  
  for (UINT i = 0;
       i < NumModes;
       i++)
    {
      if (DisplayModeList[i].Width == (UINT)ScreenWidth)
	{
	  if (DisplayModeList[i].Height == (UINT)ScreenHeight)
	    {
	      Numerator = DisplayModeList[i].RefreshRate.Numerator;
	      Denominator = DisplayModeList[i].RefreshRate.Denominator; 
	    }
	}
    }

  Result = Adapter->GetDesc(&AdapterDesc);
  if (FAILED(Result))
    {
      OutputDebugStringA("GetDesc did not work");
    }

  int VideoCardMemory = (int)(AdapterDesc.DedicatedVideoMemory / 1024 / 1024); 

  delete[] DisplayModeList;
  DisplayModeList = 0;

  Display->Release();
  Display = 0;

  DXGIFactory->Release();
  DXGIFactory = 0;
  
  UINT MultiSamplingCount = 4;
  UINT MultiSamplingQualityLevels = 0;
  bool MultiSamplingEnabled = false;
  
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    
  ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
  SwapChainDesc.BufferDesc.Width = ScreenWidth;
  SwapChainDesc.BufferDesc.Height = ScreenHeight;
  SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  SwapChainDesc.BufferCount = 1;
  SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  SwapChainDesc.OutputWindow = Window;

  bool FullScreen = false;
  
  if (FullScreen)
    {
      SwapChainDesc.Windowed = FALSE;
    }
  else
    {
      SwapChainDesc.Windowed = TRUE;
    }
  
  bool VSYNC_ENABLED = true;

  if (VSYNC_ENABLED)
    {
      SwapChainDesc.BufferDesc.RefreshRate.Numerator = Numerator;
      SwapChainDesc.BufferDesc.RefreshRate.Denominator = Denominator;
    }
  else
    {
      SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
      SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }
  
  SwapChainDesc.Flags = 0;
  
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
  
  Result = D3D11CreateDeviceAndSwapChain(NULL, 
					 D3D_DRIVER_TYPE_HARDWARE, 
					 NULL, 
					 0, 
					 &FeatureLevel, 
					 1,
					 D3D11_SDK_VERSION, 
					 &SwapChainDesc,
					 &SwapChain, 
					 &Device, 
					 NULL,
					 &DeviceContext); 
  if (FAILED(Result))
    {
      OutputDebugStringA("Device and swap chain");
    }
  
  IDXGIDevice* DXGIDevice = nullptr;
  Result = Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DXGIDevice));
  if (FAILED(Result))
    {
      OutputDebugStringA("Failed to get DXGI Device");
    }
  
  Result = DXGIDevice->GetParent(__uuidof(Adapter), reinterpret_cast<void**>(&Adapter));
  if (FAILED(Result))
    {
      OutputDebugStringA("DXGI Adapter");
    }
   
  Result = Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
						 MultiSamplingCount,
						 &MultiSamplingQualityLevels);
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Multi sample quality levels", "Error.", MB_OK | MB_ICONERROR);
    }

  ReleaseObject(DXGIDevice);
  ReleaseObject(Adapter);
  
  ID3D11Texture2D* BackBuffer;
  Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
  if (FAILED(Result))
    {
      OutputDebugStringA("Back Buffer failed");
    }

  Result = Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderTargetView); 
  if (FAILED(Result))
    {
      OutputDebugStringA("Render target view failed");
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
      OutputDebugStringA("Depth Stencil View failed");
    }

  
  Result = Device->CreateDepthStencilView(DepthStencilBuffer, nullptr, &DepthStencilView);
  if (FAILED(Result))
    {
      OutputDebugStringA("Depth stencil buffer failed");
    }
  
  D3D11_VIEWPORT Viewport;
  Viewport.TopLeftX = 0.f;
  Viewport.TopLeftY = 0.f;
  Viewport.Width = (float)ScreenWidth;
  Viewport.Height = (float)ScreenHeight;
  Viewport.MinDepth = 0.f;
  Viewport.MaxDepth = 1.f;

  DeviceContext->RSSetViewports(1, &Viewport);

  DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
  
  ID3D11RasterizerState* RasterizerState = nullptr;

  D3D11_RASTERIZER_DESC RasterizerDesc;
  ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

  RasterizerDesc.FillMode = D3D11_FILL_SOLID;
  RasterizerDesc.CullMode = D3D11_CULL_NONE;  
  RasterizerDesc.FrontCounterClockwise = FALSE; 
  RasterizerDesc.DepthBias = 0;
  RasterizerDesc.DepthBiasClamp = 0;
  RasterizerDesc.SlopeScaledDepthBias = 0;
  RasterizerDesc.DepthClipEnable = TRUE;
  RasterizerDesc.ScissorEnable = FALSE;
  RasterizerDesc.MultisampleEnable = FALSE;
  RasterizerDesc.AntialiasedLineEnable = TRUE;

  Result = Device->CreateRasterizerState(&RasterizerDesc, &RasterizerState);
  if (FAILED(Result))
    {
      OutputDebugStringA("Rasterizer failed");
    }
  
  DeviceContext->RSSetState(RasterizerState);
}
