#include "Executable.h"
#include "Application.h"

HostApplication::HostApplication()
{
  m_WindowWidth = 640;
  m_WindowHeight = 480;
}

HostApplication::~HostApplication()
{
  if (m_hWnd)
    {
      DestroyWindow(m_hWnd);
    }
}

void HostApplication::Go()
{
  WNDCLASSEX WindowClass =
    {
      sizeof(WNDCLASSEX),
      CS_CLASSDC,
      EntryMessageHandler,
      0,
      0,
      GetModuleHandle(NULL),
      NULL,
      NULL,
      NULL,
      NULL,
      "Host Application",
      NULL
    };

  RegisterClassEx(&WindowClass);

  m_hWnd = CreateWindow("Host Application",
			"Host Application",
			WS_OVERLAPPEDWINDOW,
			0, 0,
			m_WindowWidth, m_WindowHeight,
			GetDesktopWindow(),
			NULL,
			WindowClass.hInstance,
			NULL);

  if (m_hWnd)
    ShowWindow(m_hWnd, SW_SHOW);
  else
    return; 

  if (!InitializeD3D(false))
    {
      MessageBoxA(m_hWnd, "E", "B", MB_OK);
    }
  
  MSG Message;
  PeekMessage(&Message, 0, 0, 0, PM_REMOVE);
  while(Message.message != WM_QUIT)
    {      
      Render();
      TranslateMessage(&Message);
      DispatchMessage(&Message);
      PeekMessage(&Message, 0, 0, 0, PM_REMOVE);
      m_Continue = HandleMessage(&Message); 
    }
}

HRESULT HostApplication::InitializeD3D(bool fullscreen) 
{
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
    {
      return false; 
    }
  
  result = DXFactory->EnumAdapters(0, &Adapter);
  if (FAILED(result))
    {
      return false; 
    }

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
  SwapChainDesc.BufferDesc.Width = m_WindowWidth;
  SwapChainDesc.BufferDesc.Height = m_WindowHeight;
  SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1; 
  SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
  SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  SwapChainDesc.SampleDesc.Count = 1;
  SwapChainDesc.SampleDesc.Quality = 0;
  SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  SwapChainDesc.OutputWindow = m_hWnd;
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
					 &m_SwapChain,
					 &m_Device,
					 &FeatureLevelsSupported,
					 &m_DeviceContext);
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
					     &m_SwapChain,
					     &m_Device,
					     &FeatureLevelsSupported,
					     &m_DeviceContext);
    }
  
  if (FAILED(result))
    {
      return false; 
    }
  
  result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_BackBuffer);
  
  ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
  viewport.Width = m_WindowWidth;
  viewport.Height = m_WindowHeight;
  viewport.MinDepth = 0.f;
  viewport.MaxDepth = 1.f;
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;

  m_DeviceContext->RSSetViewports(1, &viewport);

  ZeroMemory(&DepthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
  DepthStencilDesc.Width = m_WindowWidth;
  DepthStencilDesc.Height = m_WindowHeight;
  DepthStencilDesc.MipLevels = 1;
  DepthStencilDesc.ArraySize = 1;
  DepthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
  DepthStencilDesc.SampleDesc.Count = 1;
  DepthStencilDesc.SampleDesc.Quality = 0;
  DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
  DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  DepthStencilDesc.CPUAccessFlags = 0;
  DepthStencilDesc.MiscFlags = 0;

  result = m_Device->CreateTexture2D(&DepthStencilDesc, 0, &m_DepthStencilBuffer);
  if (FAILED(result))
    return false;
  
  // result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, "/**/",&DepthView);
    
  return true; 
}

BOOL HostApplication::Render()
{
  m_RenderTargetView = NULL;
  m_DepthStencilView = 0;

  m_DeviceContext->OMGetRenderTargets(0, &m_RenderTargetView, &m_DepthStencilView);
  
  float clearColours[] = { 1.f, 0.f, 1.f, 1.f };

  m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColours );
  if(m_RenderTargetView)
    {
      m_RenderTargetView->Release();
      m_RenderTargetView = nullptr; 
    }
	
  if (m_DepthStencilView)
    {
      m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.,
					 D3Dll_CLEAR_DEPTH, depth, stencil );
    }
 
  
  if(m_DepthStencilView)
    {
      m_DepthStencilView->Release();
      m_DepthStencilView = nullptr; 
    }

  m_SwapChain->Present(0, 0);

  return true;
}

void HostApplication::Shutdown()
{
  m_DeviceContext->ClearState();

  if (m_Device)
    {
      m_Device->Release();
      m_Device = nullptr; 
    }

  if (m_RenderTargetView)
    {
      m_RenderTargetView->Release();
      m_RenderTargetView = nullptr; 
    }

  if (m_DeviceContext)
    {
      m_DeviceContext->Release();
      m_DeviceContext = nullptr; 
    }

  if (m_BackBuffer)
    {
      m_BackBuffer->Release();
      m_BackBuffer = nullptr; 
    }

  if (m_RenderTargetView)
    {
      m_RenderTargetView->Release();
      m_RenderTargetView = nullptr; 
    }
    

  if (m_DepthStencilBuffer)
    {
      m_DepthStencilBuffer->Release();
      m_DepthStencilBuffer = nullptr; 
    }

  if (m_DepthStencilView)
    {
      m_DepthStencilView->Release();
      m_DepthStencilView = nullptr; 
    }
}

LRESULT WINAPI HostApplication::MessageHandler(HWND hWnd,
					       UINT Message,
					       WPARAM wParam,
					       LPARAM lParam)
{
  switch(Message)
    {
    case WM_DESTROY:
      {
	Shutdown(); 
	PostQuitMessage(0);
      } break; 
    }

  return DefWindowProc(hWnd, Message, wParam, lParam);
}

BOOL HostApplication::HandleMessage(MSG *pMessage)
{
  if (pMessage->message == WM_KEYDOWN &&
      pMessage->wParam == VK_ESCAPE)
    return FALSE;

  return TRUE;
}
