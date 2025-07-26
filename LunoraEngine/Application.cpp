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
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;
  D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;
  D3D_FEATURE_LEVEL FeatureLevel;
  HRESULT result; 

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
					 0,
					 &FeatureLevels,
					 1,
					 D3D11_SDK_VERSION,
					 &SwapChainDesc,
					 &m_SwapChain,
					 &m_Device,
					 &FeatureLevel,
					 &m_DeviceContext);
  if (FAILED(result))
    {
      return false; 
    }

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
