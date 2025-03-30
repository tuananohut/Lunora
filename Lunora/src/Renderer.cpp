#include "renderer.h"

static void InitializeDX11(DeviceManager& DeviceManager,
			   RenderTargetManager& RenderTargetManager,
			   HWND Window)
{
  HRESULT Result;

  DeviceManager.SetViewport(ScreenWidth, ScreenHeight);
  
  DeviceManager.DeviceContext->OMSetRenderTargets(1, &RenderTargetManager.RenderTargetView, RenderTargetManager.DepthStencilView);
  
  ID3D11RasterizerState* RasterizerState = nullptr;

  D3D11_RASTERIZER_DESC RasterizerDesc;
  ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

  RasterizerDesc.FillMode = D3D11_FILL_SOLID;
  RasterizerDesc.CullMode = D3D11_CULL_NONE;  
  RasterizerDesc.FrontCounterClockwise = FALSE; 
  RasterizerDesc.DepthBias = 0;
  RasterizerDesc.DepthBiasClamp = 0;
  RasterizerDesc.SlopeScaledDepthBias = 0;
  RasterizerDesc.DepthClipEnable = TRUE;
  RasterizerDesc.ScissorEnable = FALSE;
  RasterizerDesc.MultisampleEnable = FALSE;
  RasterizerDesc.AntialiasedLineEnable = TRUE;

  Result = DeviceManager.Device->CreateRasterizerState(&RasterizerDesc, &RasterizerState);
  if (FAILED(Result))
    {
      OutputDebugStringA("Rasterizer failed");
    }
  
  DeviceManager.DeviceContext->RSSetState(RasterizerState);
}
