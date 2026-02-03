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
    ID3D11Device *Device = nullptr;
    ID3D11DeviceContext *DeviceContext = nullptr;
    ID3D11RenderTargetView *RenderTargetView = nullptr;
    ID3D11Texture2D *DepthStencilBuffer = nullptr;
    ID3D11DepthStencilView *DepthStencilView = nullptr;
    ID3D11DepthStencilState *DepthStencilState = nullptr;
    ID3D11DepthStencilState *DepthDisabledStencilState = nullptr;  
    IDXGISwapChain *SwapChain = nullptr;
    ID3D11Texture2D *BackBuffer = nullptr;
    D3D11_VIEWPORT Viewport;
  };

  bool InitializeRenderer(RendererContext& context,
			  HWND hwnd,
			  int width,
			  int height);
  
  void ShutdownRenderer(RendererContext& context);

  bool ResizeRenderer(RendererContext& context, int width, int height);

  void TurnZBufferOn(ID3D11DeviceContext* DeviceContext, ID3D11DepthStencilState* DepthStencilState);
  void TurnZBufferOff(ID3D11DeviceContext* DeviceContext, ID3D11DepthStencilState *DepthDisabledStencilState);
  
  void RendererBeginScene(RendererContext& context, float r, float g, float b, float a); 
  void RendererEndScene(RendererContext& context); 
};

#endif
