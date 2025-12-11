#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

struct TargaHeader
{
  unsigned char data1[12];
  unsigned short width;
  unsigned short height;
  unsigned char bpp;
  unsigned char data2; 
};

struct Texture
{
  unsigned char* m_targaData;
  ID3D11Texture2D* m_texture;
  ID3D11ShaderResourceView* m_textureView;
  int m_width, m_height; 
};

bool InitializeTexture(ID3D11Device*, ID3D11DeviceContext*, Texture*, const char*);
void ReleaseTexture(Texture*);

bool LoadTarga32Bit(Texture*, const char*); 
bool LoadTarga24Bit(Texture* texture, const char* filename);

#endif
