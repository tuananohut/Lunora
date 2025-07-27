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
      TranslateMessage(&Message);
      DispatchMessage(&Message);
      PeekMessage(&Message, 0, 0, 0, PM_REMOVE);
      m_Continue = HandleMessage(&Message); 
    }
}

HRESULT HostApplication::InitializeD3D(bool fullscreen) 
{
  IDXGIFactory* DXFactory = NULL;
  IDXGIAdapter* DXAdapter;
  IDXGIAdapter* DXAdapters;
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
  D3D11_VIEWPORT viewport; 
  HRESULT result;

  #ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  #endif 
  
  result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&DXFactory);
  if (FAILED(result))
    {
      return false; 
    }
  
  for (UINT i = 0;
       DXFactory->EnumAdapters(i, &DXAdapter) != DXGI_ERROR_NOT_FOUND;
       ++i)
    {
      DXAdapters[i] = DXAdapter; 
    }

  if (DXFactory)
    {
      DXFactory->Release();
    }
  
  for (auto a: DXAdapters)
    result = a->EnumOutputs(0, &Output);
  
  result = Output->GetDisplayModeList(format, 0, &numModes, NULL);

  displayModes = new DXGI_MODE_DESC[numModes];

  result = Output->GetDisplayModeList(format, 0, &numModes, displayModes);

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
					 D3d_DRIVER_TYPE_HARDWARE,
					 NULL,
					 createDeviceFlags,
					 &FeatureLevelsRequested,
					 _countof(FeatureLevelsRequested) - 1,
					 D3D11_SDK_VERSION,
					 &SwapChainDesc,
					 &m_SwapChain,
					 &m_Device,
					 &FeatureLevelsSupported,
					 &m_DeviceContext);
  if (result == E_INVALIDARG)
    {
      result = D3D11CreateDeviceAndSwapChain(NULL,
					     D3d_DRIVER_TYPE_HARDWARE,
					     NULL,
					     createDeviceFlags,
					     &FeatureLevelsRequested[1],
					     _countof(FeatureLevelsRequested) - 1,
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

  /*
    TKANE (NOTE): CHECK FOR EDITOR LAYER
    
  result = DXFactory->CreateSwapChain();
  
  result = D3D11CreateDevice();
  */ 
  
  result = m_SwapChain->GetBuffer(0, __uidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);

  m_Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);

  m_DeviceContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
  
  ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
  viewport.Width = m_WindowWidth;
  viewport.Height = m_WindowHeight;
  viewport.MinDepth = 0.f;
  viewport.MaxDepth = 1.f;
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;

  m_DeviceContext->RSSetViewports(1, &viewport);
  
  return true; 
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
