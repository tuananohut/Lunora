#include "texturemanager.h"

TextureManager::TextureManager()
{
  m_TextureArray = nullptr;
}

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {}

bool TextureManager::Initialize(int count)
{
  m_textureCount = count;

  m_TextureArray = new Texture[m_textureCount];
  if (!m_textureArray)
    {
      return false;
    }

  return true; 
}

void TextureManager::Shutdown()
{
  int i;

  if (m_TextureArray)
    {
      for (i = 0; i < m_textureCount; i++)
	{
	  m_TextureArray[i].Shutdown();
	}

      delete[] m_TextureArray;
      m_TextureArray = nullptr; 
    }
}

bool TextureManager::LoadTexture(ID3D11Device* device,
				 ID3D11DeviceContext* deviceContext,
				 char* filename,
				 int location)
{
  bool result;

  result = m_TextureArray[location].Initialize(device, deviceContext, filename);
  if (!result)
    {
      return false; 
    }

  return true; 
}

ID3D11ShaderResourceView* TextureManager::GetTexture(int id)
{
  return m_TextureArray[id].GetTexture();
}
