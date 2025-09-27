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
  ID3D11Device *Device;
  ID3D11DeviceContext *DeviceContext;
};

CoreRenderBuffers InitializeD3D(Win32WindowProperties& Window, 
				bool fullscreen);
void ShutdownD3D(CoreRenderBuffers& Renderer);
void BeginScene(CoreRenderBuffers& Renderer); 


#endif
