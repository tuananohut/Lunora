#include "D3d.h"

D3D::D3D()
{
  m_swapChain = nullptr;
  m_device = nullptr;
  m_deviceContext = nullptr;
  m_renderTargetView = nullptr;
  m_depthStencilBuffer = nullptr;
  m_depthStencilState = nullptr;
  m_depthStencilView = nullptr;
  m_rasterState = nullptr; 
}

D3D::D3D(const D3D& other) {}

D3D::~D3D() {}

bool D3D::Initialize(int screenWidth, int screenHeight,
		     bool vsync, HWND hwnd, bool fullscreen,
		     float screenDepth, float screenNear)
{
  HRESULT result;
  IDXGIFactory *factory;
  IDXGIAdapter *adapter;
  IDXGIOutput *adapterOutput;
  unsigned int numModes, i, numerator, denominator;
  unsigned long long stringLength;
  DXGI_MODE_DESC *displayModeList;
  DXGI_ADAPTER_DESC adapterDesc;
  int error;
  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  D3D_FEATURE_LEVEL featureLevel;
  ID3D11Texture2D *backBufferPtr;
  D3D11_TEXTURE2D_DESC depthBufferDesc;
  D3D11_DEPTH_STENCIL_DESC depthStencilDec; 
  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
  D3D11_RASTERIZER_DESC rasterDesc;
  float fieldOfView, screenAspect;

  m_vsync_enabled = vsync;

  result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
  if (FAILED(result))
    {
      return false; 
    }

  result = factory->EnumAdapters(0, &adapter);
  if (FAILED(result)) 
    {
      return false; 
    }

}
