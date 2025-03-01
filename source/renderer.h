#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#define DeleteObject(object) if ((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if ((objects) != NULL) { delete[] objects; objects = NULL; }
#define ReleaseObject(object) if ((object) != NULL) { object->Release(); object = NULL; }

static UINT ScreenWidth = 1280;
static UINT ScreenHeight = 720;

static XMMATRIX WorldMatrix = XMMatrixIdentity();
static XMMATRIX ViewMatrix = XMMatrixLookAtLH
  (
   XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f),
   XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
   XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
   );
static XMMATRIX ProjectionMatrix = XMMatrixPerspectiveFovLH
  (
   XM_PIDIV4,
   ScreenWidth / ScreenHeight,
   0.1f,
   1000.0f
   );

static ID3D11Device* Device = nullptr;
static ID3D11DeviceContext* DeviceContext = nullptr;
static IDXGISwapChain* SwapChain;
static ID3D11RenderTargetView* RenderTargetView = nullptr;  
static ID3D11DepthStencilView* DepthStencilView = nullptr;
static FLOAT BackgroundColor[4] = {0.141f, 0.137f, 0.365f, 1.f};

// Default colors 
LPCWSTR VSFileName = L"../source/color.vs";
LPCWSTR PSFileName = L"../source/color.ps";

static void InitializeDX11(HWND Window);
static void CreateCube(HWND Window, LPCWSTR VSFilename, LPCWSTR PSFilename);

#endif
