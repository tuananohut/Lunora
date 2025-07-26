#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

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
  BOOL m_Continue; 
};

#endif
