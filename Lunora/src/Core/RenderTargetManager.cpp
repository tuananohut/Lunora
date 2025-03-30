#include "RenderTargetManager.h"

void RenderTargetManager::Initialize(DeviceManager& DeviceManager,
				     int ScreenWidth,
				     int ScreenHeight)
{
  HRESULT Result;
  
  ID3D11Texture2D* BackBuffer;
  Result = DeviceManager.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
  if (FAILED(Result))
    {
      OutputDebugStringA("Back Buffer failed");
    }

  Result = DeviceManager.Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderTargetView); 
  if (FAILED(Result))
    {
      OutputDebugStringA("Render target view failed");
    }
  
  ReleaseObject(BackBuffer);
  
  ID3D11Texture2D* DepthStencilBuffer = nullptr;
  
  D3D11_TEXTURE2D_DESC DepthStencilDesc;
  ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));
  DepthStencilDesc.Width = ScreenWidth;
  DepthStencilDesc.Height = ScreenHeight;
  DepthStencilDesc.MipLevels = 1;
  DepthStencilDesc.ArraySize = 1;
  DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

  if (DeviceManager.MultiSamplingEnabled)
    {
      DepthStencilDesc.SampleDesc.Count = DeviceManager.MultiSamplingCount;
      DepthStencilDesc.SampleDesc.Quality = DeviceManager.MultiSamplingQualityLevels - 1;
    }
  else
    {
      DepthStencilDesc.SampleDesc.Count = 1;
      DepthStencilDesc.SampleDesc.Quality = 0;
    }
  
  Result = DeviceManager.Device->CreateTexture2D(&DepthStencilDesc, nullptr, &DepthStencilBuffer);
  if (FAILED(Result))
    {
      OutputDebugStringA("Depth Stencil View failed");
    }

  
  Result = DeviceManager.Device->CreateDepthStencilView(DepthStencilBuffer, nullptr, &DepthStencilView);
  if (FAILED(Result))
    {
      OutputDebugStringA("Depth stencil buffer failed");
    }
}

void RenderTargetManager::Cleanup()
{
  if (RenderTargetView)
    {
      RenderTargetView->Release();
      RenderTargetView = nullptr; 
    }

  if (DepthStencilView)
    {
      DepthStencilView->Release();
      DepthStencilView = nullptr; 
    }
}
