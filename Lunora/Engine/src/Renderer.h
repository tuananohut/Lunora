#ifndef RENDERER_H
#define RENDERER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdint.h>

using namespace DirectX;

namespace LunoraEngine {
  static int SCREEN_WIDTH  = 800;
  static int SCREEN_HEIGHT = 600; 

  struct RendererContext
  {
    ID3D11Device *Device;
    ID3D11DeviceContext *DeviceContext;
    ID3D11RenderTargetView *RenderTargetView;
    ID3D11Texture2D *DepthStencilBuffer;
    ID3D11DepthStencilView *DepthStencilView;
    IDXGISwapChain *SwapChain;
    ID3D11Texture2D *BackBuffer;
    D3D11_VIEWPORT Viewport;
  };

  bool InitializeRenderer(RendererContext& context,
			  HWND hwnd,
			  int width,
			  int height);
  
  void ShutdownRenderer(RendererContext& context);

  bool ResizeRenderer(RendererContext& context, int& width, int& height);

  void RendererBeginScene(RendererContext& context, float r, float g, float b, float a); 
  void RendererEndScene(RendererContext& context); 
};
#endif
