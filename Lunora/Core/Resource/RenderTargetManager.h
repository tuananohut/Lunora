#ifndef RENDERTARGETMANAGER_H
#define RENDERTARGETMANAGER_H

#include "../Device/DeviceManager.h"

struct RenderTargetManager
{
  ID3D11RenderTargetView* RenderTargetView = nullptr;
  ID3D11DepthStencilView* DepthStencilView = nullptr;

  void Initialize(DeviceManager& DeviceManager, int ScreenWidth, int ScreenHeight);
  void Cleanup();
};

#endif
