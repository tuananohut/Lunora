#include "Texture.h"

bool InitializeTexture(ID3D11Device* device,
		       ID3D11DeviceContext* deviceContext,
		       Texture* texture, const char* filename)
{
  bool result;
  int height, width;
  D3D11_TEXTURE2D_DESC textureDesc;
  HRESULT hr;
  unsigned int rowPitch;
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

  result = LoadTarga24Bit(texture, filename);
  if (!result)
    {
      result = LoadTarga32Bit(texture, filename);
      if (!result)
	{
	  return false; 
	}
    }
  
  assert(texture != nullptr);
  
  textureDesc.Height = texture->m_height;
  textureDesc.Width = texture->m_width;
  textureDesc.MipLevels = 0;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
  
  hr = device->CreateTexture2D(&textureDesc, NULL, &texture->m_texture);
  if (FAILED(hr))
    return false;

  rowPitch = (texture->m_width * 4) * sizeof(unsigned char);

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

void ReleaseTexture(Texture* texture)
{
  if (texture->m_textureView)
    {
      texture->m_textureView->Release();
      texture->m_textureView = nullptr; 
    }

  if (texture->m_texture)
    {
      texture->m_texture->Release();
      texture->m_texture = nullptr; 
    }

  if (texture->m_targaData)
    {
      delete[] texture->m_targaData;
      texture->m_targaData = nullptr; 
    }
}

bool LoadTarga32Bit(Texture* texture, const char* filename)
{
  int error, bpp, imageSize, index = 0, i, j, k;
  FILE* filePtr;
  unsigned int count;
  TargaHeader targaFileHeader;
  unsigned char* targaImage;
  
  error = fopen_s(&filePtr, filename, "rb");
  if (error != 0)
    {
      return false; 
    }
  
  count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
  if (count != 1)
    {
      return false; 
    }
  
  texture->m_height = (int)targaFileHeader.height;
  texture->m_width = (int)targaFileHeader.width;
  bpp = (int)targaFileHeader.bpp;

  if (bpp != 32)
    {
      return false; 
    }

  imageSize = texture->m_width * texture->m_height * 4;

  targaImage = new unsigned char[imageSize];

  count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
  if (count != imageSize)
    {
      return false; 
    }

  error = fclose(filePtr);
  if (error != 0)
    {
      return false; 
    }

  texture->m_targaData = new unsigned char[imageSize];

  index = 0;

  k = (texture->m_width * texture->m_height * 4) - (texture->m_width * 4);

  
  for (j = 0; j < texture->m_height; j++)
    {
      for (i = 0; i < texture->m_width; i++)
	{
	  texture->m_targaData[index + 0] = targaImage[k + 2];
	  texture->m_targaData[index + 1] = targaImage[k + 1];
	  texture->m_targaData[index + 2] = targaImage[k + 0];
	  texture->m_targaData[index + 3] = targaImage[k + 3];
	  
	  k += 4;
	  index += 4;
	}

      k -= (texture->m_width * 8);
    }

  delete[] targaImage;
  targaImage = nullptr;

  return true;
} 

bool LoadTarga24Bit(Texture* texture, const char* filename)
{
  int error, bpp, imageSize, index = 0, i, j, k;
  FILE* filePtr;
  unsigned int count;
  TargaHeader targaFileHeader;
  unsigned char* targaImage;
  
  error = fopen_s(&filePtr, filename, "rb");
  if (error != 0)
    {
      return false; 
    }
  
  count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
  if (count != 1)
    {
      return false; 
    }
  
  texture->m_height = (int)targaFileHeader.height;
  texture->m_width = (int)targaFileHeader.width;
  bpp = (int)targaFileHeader.bpp;

  if (bpp != 24)
    {
      return false; 
    }

  imageSize = texture->m_width * texture->m_height * 4;

  targaImage = new unsigned char[imageSize];

  count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
  if (count != imageSize)
    {
      return false; 
    }

  error = fclose(filePtr);
  if (error != 0)
    {
      return false; 
    }

  texture->m_targaData = new unsigned char[imageSize];

  index = 0;

  k = (texture->m_width * texture->m_height * 3) - (texture->m_width * 3);

  
  for (j = 0; j < texture->m_height; j++)
    {
      for (i = 0; i < texture->m_width; i++)
	{
	  texture->m_targaData[index + 0] = targaImage[k + 2];
	  texture->m_targaData[index + 1] = targaImage[k + 1];
	  texture->m_targaData[index + 2] = targaImage[k + 0];
	  texture->m_targaData[index + 3] = 255;
	  
	  k += 3;
	  index += 4;
	}

      k -= (texture->m_width * 6);
    }

  delete[] targaImage;
  targaImage = nullptr;

  return true;
} 

