#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct Win32WindowProperties
{
  HWND hwnd;
  long Width;
  long Height;
};

struct CoreRenderBuffers 
{
  IDXGISwapChain* SwapChain;
  ID3D11Device* Device;
  ID3D11DeviceContext* DeviceContext;
  ID3D11Texture2D* BackBuffer;
  ID3D11RenderTargetView* RenderTargetView;
  ID3D11Texture2D* DepthStencilBuffer;
  ID3D11DepthStencilView* DepthStencilView; 
  D3D11_VIEWPORT Viewport; 
};

CoreRenderBuffers InitializeD3D(Win32WindowProperties Window, 
				bool fullscreen);
void ShutdownD3D(CoreRenderBuffers& Renderer);

bool Render(); 

#endif
