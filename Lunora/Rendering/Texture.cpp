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
}

void ReleaseTexture(Texture*)
{
  
}

bool LoadTarga32Bit(char*)
{
  
} 

