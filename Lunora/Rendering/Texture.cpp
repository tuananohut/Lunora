#include "Texture.h"

bool InitializeTexture(ID3D11Device* device,
		       ID3D11DeviceContext* deviceContext,
		       Texture* texture, char* filename)
{
  bool result;
  int height, width;
  D3D11_TEXTURE2D_DESC textureDesc;
  HRESULT hr;
  unsigned int rowPitch;
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

  result = LoadTarga32Bit(filename);
  if (!result)
    {
      return false; 
    }

  textureDesc.Height = texture.m_height;
  textureDesc.Width = texture.m_width;
  textureDesc.MipLevels = 0;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

  hr = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
  if (FAILED(hr))
    return false;

  rowPitch = (texture.m_width * 4) * sizeof(unsigned char);

  deviceContext->UpdateSubresource(texture->m_texture, 0, NULL, texture->m_targaData, rowPitch, 0);

  srvDesc.Format = textureDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.Texture2D.MipLevels = -1;

  hr = device->CreateShaderResourceView(texture->m_texture, &srvDesc, &texture->m_textureView);
  if (FAILED(hr))
    return false;

  deviceContext->GenerateMips(texture->m_textureView);

  delete[] texture->m_targaData;
  texture->m_targaData = nullptr;

  return true;
}

void ReleaseTexture(Texture*)
{
  
}

bool LoadTarga32Bit(char*)
{
  
} 

