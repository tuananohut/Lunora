#include "renderer.h"

static void InitializeDX11(DeviceManager& DeviceManager,
			   RenderTargetManager& RenderTargetManager,
			   PipelineStateManager& PipelineStateManager,
			   HWND Window)
{
  HRESULT Result;

  DeviceManager.SetViewport(ScreenWidth, ScreenHeight);
  
  DeviceManager.DeviceContext->OMSetRenderTargets(1, &RenderTargetManager.RenderTargetView, RenderTargetManager.DepthStencilView); 
}
