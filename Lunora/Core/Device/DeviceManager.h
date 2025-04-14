#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#define DeleteObject(object) if ((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if ((objects) != NULL) { delete[] objects; objects = NULL; }
#define ReleaseObject(object) if ((object) != NULL) { object->Release(); object = NULL; }

struct DeviceManager
{
  ID3D11Device* Device;
  ID3D11DeviceContext* DeviceContext;
  IDXGISwapChain* SwapChain = nullptr;

  void Initialize(HWND Window, int Width, int Height);
  void SetViewport(int Width, int Height);
  void Cleanup();
  
  UINT ScreenWidth = 1280; 
  UINT ScreenHeight = 720;

  IDXGIFactory *DXGIFactory;
  IDXGIAdapter *Adapter;
  IDXGIOutput *Display;

  UINT MultiSamplingCount = 4;
  UINT MultiSamplingQualityLevels = 0;
  bool MultiSamplingEnabled = false;
};

#endif
