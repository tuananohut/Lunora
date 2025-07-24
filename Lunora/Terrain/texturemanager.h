#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "texture.h"

class TextureManager
{
 public:
  TextureManager();
  TextureManager(const TextureManager&);
  ~TextureManager();

  bool Initialize(int);
  void Shutdown();

  bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*, char*);

  ID3D11ShaderResourceView* GetTexture(int);

 private:
  Texture* m_TextureArray;
  int m_textureCount; 
};

#endif
