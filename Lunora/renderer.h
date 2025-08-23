#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdint.h>

using namespace DirectX;

struct Win32WindowProperties
{
  HWND hwnd;
  int32_t Width;
  int32_t Height;
};

struct CoreRenderBuffers 
{
  IDXGISwapChain* SwapChain;
  ID3D11Device* Device;
  ID3D11DeviceContext* DeviceContext;
  ID3D11Texture2D* BackBuffer;
  ID3D11RenderTargetView* RenderTargetView;
  ID3D11Texture2D* DepthStencilBuffer;
  ID3D11DepthStencilState* DepthStencilState; 
  ID3D11DepthStencilView* DepthStencilView; 
  D3D11_VIEWPORT Viewport; 
};

struct HexMeshVertexBuffer
{
  XMFLOAT3 position;
  XMFLOAT2 texture; 
  XMFLOAT3 normal;
  XMFLOAT3 tangent;
  XMFLOAT3 binormal; 
  XMFLOAT3 color; 
};

struct MeshVertexBuffer
{
  XMFLOAT3 position;
  XMFLOAT2 texture; 
  XMFLOAT3 normal;
  XMFLOAT3 tangent;
  XMFLOAT3 binormal; 
  XMFLOAT3 color; 
};

CoreRenderBuffers InitializeD3D(Win32WindowProperties& Window, 
				bool fullscreen);
void ShutdownD3D(CoreRenderBuffers& Renderer);
void BeginScene(CoreRenderBuffers& Renderer); 


#endif
