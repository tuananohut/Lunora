#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgi.h>

using namespace DirectX; 

class HostApplication
{
public:
  HostApplication();
  ~HostApplication();

  LRESULT WINAPI MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  void Go(); 

protected:
  HRESULT InitializeD3D(bool fullscreen);
  BOOL Render(); 
  void Shutdown();
  
  BOOL HandleMessage(MSG *pMessage);
  
protected:
  HWND m_hWnd;
  long m_WindowWidth;
  long m_WindowHeight;
  IDXGISwapChain* m_SwapChain;
  ID3D11Device* m_Device;
  ID3D11DeviceContext* m_DeviceContext;
  ID3D11Texture2D* m_BackBuffer;
  ID3D11RenderTargetView* m_RenderTargetView;
  ID3D11Texture2D* m_DepthStencilBuffer;
  ID3D11DepthStencilView* m_DepthStencilView; 
  D3D11_VIEWPORT viewport; 
  BOOL m_Continue; 
};

#endif
